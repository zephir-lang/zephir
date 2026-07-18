<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Parser\Php;

use function strlen;
use function strspn;

/**
 * Pure-PHP recursive-descent + Pratt parser producing the exact IR the C
 * extension (php-zephir-parser) emits — see PURE_PHP_PARSER_DESIGN.md.
 *
 * Node arrays are built in the precise field order of the `xx_ret_*` builders
 * in `parser/parser.h`; optional fields are omitted (never null); string values
 * are run through {@see remap()} ("return_value"/"this_ptr"). Positions follow
 * the lookahead-stamping invariant: a node's `line`/`char` is the scanner state
 * after the current lookahead token, captured by reading {@see peek()} at the
 * moment the node is built. `class`/`interface`/`method`/`function` instead use
 * their own keyword token's position; `method` also records `last-line`.
 *
 * This is an in-progress vertical slice (namespace/use/class/trait/method/
 * property/const/let/return/echo/calls + full expression grammar). Constructs
 * outside the slice raise {@see SyntaxError} via unsupported(); such inputs
 * are not yet part of the byte-identical golden corpus.
 */
final class PhpParser
{
    /** Binary infix operator token => [binding power, expr type, right-assoc?]. */
    private const INFIX = [
        TokenType::T_OR                  => [70,  'or',                false],
        TokenType::T_AND                 => [80,  'and',               false],
        TokenType::T_BITWISE_OR          => [90,  'bitwise_or',        false],
        TokenType::T_BITWISE_XOR         => [100, 'bitwise_xor',       false],
        // `expr & expr` carries a `%prec BITWISE_OR` hint in the grammar, which
        // makes same-operator chains right-associative (`a & b & c` => a & (b & c)).
        TokenType::T_BITWISE_AND         => [110, 'bitwise_and',       true],
        TokenType::T_EQUALS              => [120, 'equals',            false],
        TokenType::T_IDENTICAL           => [120, 'identical',         false],
        TokenType::T_NOTIDENTICAL        => [120, 'not-identical',     false],
        TokenType::T_NOTEQUALS           => [120, 'not-equals',        false],
        TokenType::T_LESS                => [130, 'less',              false],
        TokenType::T_GREATER             => [130, 'greater',           false],
        TokenType::T_LESSEQUAL           => [130, 'less-equal',        false],
        TokenType::T_GREATEREQUAL        => [130, 'greater-equal',     false],
        TokenType::T_BITWISE_SHIFTLEFT   => [140, 'bitwise_shiftleft', false],
        TokenType::T_BITWISE_SHIFTRIGHT  => [140, 'bitwise_shiftright', false],
        TokenType::T_ADD                 => [150, 'add',               false],
        TokenType::T_SUB                 => [150, 'sub',               false],
        TokenType::T_DOT                 => [150, 'concat',            false],
        TokenType::T_MUL                 => [160, 'mul',               false],
        TokenType::T_DIV                 => [160, 'div',               false],
        TokenType::T_MOD                 => [160, 'mod',               false],
        TokenType::T_INCLUSIVE_RANGE     => [180, 'irange',            false],
        TokenType::T_EXCLUSIVE_RANGE     => [180, 'erange',            false],
        TokenType::T_INSTANCEOF          => [230, 'instanceof',        false],
    ];

    /** Assignment operator token => operator string. */
    private const ASSIGN_OPS = [
        TokenType::T_ASSIGN                    => 'assign',
        TokenType::T_ASSIGN_ADD                => 'add-assign',
        TokenType::T_ASSIGN_SUB                => 'sub-assign',
        TokenType::T_ASSIGN_MUL                => 'mul-assign',
        TokenType::T_ASSIGN_DIV                => 'div-assign',
        TokenType::T_ASSIGN_CONCAT             => 'concat-assign',
        TokenType::T_ASSIGN_MOD                => 'mod-assign',
        TokenType::T_ASSIGN_BITWISE_AND        => 'bitwise-and-assign',
        TokenType::T_ASSIGN_BITWISE_OR         => 'bitwise-or-assign',
        TokenType::T_ASSIGN_BITWISE_XOR        => 'bitwise-xor-assign',
        TokenType::T_ASSIGN_BITWISE_SHIFTLEFT  => 'bitwise-shiftleft-assign',
        TokenType::T_ASSIGN_BITWISE_SHIFTRIGHT => 'bitwise-shiftright-assign',
    ];

    /** Type keyword token => type string (xx_ret_type). */
    private const TYPE_NAMES = [
        TokenType::T_TYPE_INTEGER  => 'int',
        TokenType::T_TYPE_UINTEGER => 'uint',
        TokenType::T_TYPE_LONG     => 'long',
        TokenType::T_TYPE_ULONG    => 'ulong',
        TokenType::T_TYPE_CHAR     => 'char',
        TokenType::T_TYPE_UCHAR    => 'uchar',
        TokenType::T_TYPE_DOUBLE   => 'double',
        TokenType::T_TYPE_BOOL     => 'bool',
        TokenType::T_TYPE_STRING   => 'string',
        TokenType::T_TYPE_ARRAY    => 'array',
        TokenType::T_TYPE_VAR      => 'variable',
        TokenType::T_TYPE_OBJECT   => 'object',
        TokenType::T_TYPE_CALLABLE => 'callable',
        TokenType::T_TYPE_RESOURCE => 'resource',
        TokenType::T_TYPE_MIXED    => 'mixed',
    ];

    private const VISIBILITY = [
        TokenType::T_INTERNAL   => 'internal',
        TokenType::T_PUBLIC     => 'public',
        TokenType::T_PROTECTED  => 'protected',
        TokenType::T_PRIVATE    => 'private',
        TokenType::T_STATIC     => 'static',
        TokenType::T_INLINE     => 'inline',
        TokenType::T_DEPRECATED => 'deprecated',
        TokenType::T_ABSTRACT   => 'abstract',
        TokenType::T_FINAL      => 'final',
    ];

    /**
     * Max depth for nested expressions. The C extension turns very deep nesting
     * into an error node, because its parser stack only grows up to the process
     * memory limit. This does the same, so wild input returns an error node
     * instead of using up the PHP call stack and causing a fatal that cannot be
     * caught. The limit is far above any real expression nesting, so normal code
     * never reaches it.
     */
    private const MAX_EXPR_DEPTH = 1000;

    /** @var list<Token> */
    private array $tokens = [];
    private int $pos = 0;
    private string $file = '';

    /** Current depth of nested expressions, capped by {@see MAX_EXPR_DEPTH}. */
    private int $exprDepth = 0;

    /**
     * Parse Zephir source into the IR array. Never throws on malformed input —
     * returns the C extension's error-node array instead.
     */
    public function parse(string $code, string $filePath): array
    {
        $this->file = $filePath;

        // base.c: program_length < 2 || all-whitespace => empty array.
        if (strlen($code) < 2 || strspn($code, " \t\n\v\f\r") === strlen($code)) {
            return [];
        }

        $lexer        = new Lexer($code);
        $this->tokens = $lexer->tokenize();
        $this->pos    = 0;

        if ($lexer->hasScannerError()) {
            return $this->scannerErrorNode($lexer);
        }

        try {
            $statements = $this->parseTopStatementList();
            if ($this->peekType() !== 0) {
                $this->syntaxError();
            }

            // base.c: a program of only IGNORE tokens yields [].
            return $statements ?? [];
        } catch (\Throwable $e) {
            // The C extension never throws to the caller. It returns an error
            // node instead. Keep that contract for every failure: the internal
            // SyntaxError signal and any other unexpected runtime error (this
            // includes the depth guard in parseExpr()).
            return $this->syntaxErrorNode();
        }
    }

    /* ===================================================================== *
     *  Token cursor helpers
     * ===================================================================== */

    private function peek(): Token
    {
        return $this->tokens[$this->pos];
    }

    private function peekType(): int
    {
        return $this->tokens[$this->pos]->opcode;
    }

    private function la(int $k): Token
    {
        return $this->tokens[$this->pos + $k] ?? $this->tokens[count($this->tokens) - 1];
    }

    private function laType(int $k): int
    {
        return $this->la($k)->opcode;
    }

    private function advance(): Token
    {
        return $this->tokens[$this->pos++];
    }

    private function check(int $opcode): bool
    {
        return $this->peekType() === $opcode;
    }

    private function accept(int $opcode): bool
    {
        if ($this->peekType() === $opcode) {
            $this->pos++;

            return true;
        }

        return false;
    }

    private function expect(int $opcode): Token
    {
        if ($this->peekType() !== $opcode) {
            $this->syntaxError();
        }

        return $this->tokens[$this->pos++];
    }

    /** Current lookahead position — the value stamped onto a node built now. */
    private function line(): int
    {
        return $this->tokens[$this->pos]->line;
    }

    private function char(): int
    {
        return $this->tokens[$this->pos]->char;
    }

    /** Saved class_* snapshot of the current lookahead — used to stamp class/interface nodes. */
    private function classLine(): int
    {
        return $this->tokens[$this->pos]->classLine;
    }

    private function classChar(): int
    {
        return $this->tokens[$this->pos]->classChar;
    }

    /** Saved method_* snapshot of the current lookahead — used to stamp function/method nodes. */
    private function methodLine(): int
    {
        return $this->tokens[$this->pos]->methodLine;
    }

    private function methodChar(): int
    {
        return $this->tokens[$this->pos]->methodChar;
    }

    private function syntaxError(): void
    {
        throw new SyntaxError();
    }

    private function unsupported(): void
    {
        // Construct outside the current vertical slice; surfaces as a parse
        // failure so out-of-scope inputs are excluded from golden parity.
        throw new SyntaxError();
    }

    private function remap(string $value): string
    {
        if ($value === 'return_value') {
            return '_zephir_return_value';
        }
        if ($value === 'this_ptr') {
            return '_zephir_this_ptr';
        }

        return $value;
    }

    private function syntaxErrorNode(): array
    {
        $tok = $this->tokens[$this->pos] ?? $this->tokens[count($this->tokens) - 1];

        return [
            'type'    => 'error',
            'message' => $tok->opcode === 0 ? 'Unexpected EOF' : 'Syntax error',
            'file'    => $this->file,
            'line'    => $tok->line,
            'char'    => $tok->char,
        ];
    }

    private function scannerErrorNode(Lexer $lexer): array
    {
        return [
            'type'    => 'error',
            'message' => $lexer->getScannerErrorMessage(),
            'file'    => $this->file,
            'line'    => $lexer->getActiveLine(),
            'char'    => $lexer->getActiveChar(),
        ];
    }

    /* ===================================================================== *
     *  Top-level statements
     * ===================================================================== */

    /** @return list<array>|null */
    private function parseTopStatementList(): ?array
    {
        $list = [];
        while ($this->peekType() !== 0) {
            $list[] = $this->parseTopStatement();
        }

        return $list === [] ? null : $list;
    }

    private function parseTopStatement(): array
    {
        switch ($this->peekType()) {
            case TokenType::T_COMMENT:
                return $this->parseComment();
            case TokenType::T_CBLOCK:
                return $this->parseCblock();
            case TokenType::T_NAMESPACE:
                return $this->parseNamespaceOrUse();
            case TokenType::T_USE:
                return $this->parseUse();
            case TokenType::T_ABSTRACT:
            case TokenType::T_FINAL:
            case TokenType::T_CLASS:
                return $this->parseClass();
            case TokenType::T_FUNCTION:
                return $this->parseFunction();
            case TokenType::T_INTERFACE:
                return $this->parseInterface();
            case TokenType::T_TRAIT:
                return $this->parseTrait();
            default:
                break;
        }

        $this->syntaxError();
    }

    private function parseComment(): array
    {
        $tok = $this->expect(TokenType::T_COMMENT);

        return [
            'type'  => 'comment',
            'value' => $this->remap((string) $tok->value),
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];
    }

    private function parseCblock(): array
    {
        $tok = $this->expect(TokenType::T_CBLOCK);

        return [
            'type'  => 'cblock',
            'value' => $this->remap((string) $tok->value),
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];
    }

    private function parseNamespaceOrUse(): array
    {
        $this->expect(TokenType::T_NAMESPACE);
        $name = $this->expectNameToken();
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type' => 'namespace',
            'name' => $this->remap((string) $name->value),
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function parseUse(): array
    {
        $this->expect(TokenType::T_USE);
        $aliases = [$this->parseUseAlias()];
        while ($this->accept(TokenType::T_COMMA)) {
            $aliases[] = $this->parseUseAlias();
        }
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type'    => 'use',
            'aliases' => $aliases,
            'file'    => $this->file,
            'line'    => $this->line(),
            'char'    => $this->char(),
        ];
    }

    private function parseUseAlias(): array
    {
        $name  = $this->expectNameToken();
        $alias = null;
        if ($this->accept(TokenType::T_AS)) {
            $alias = $this->expectNameToken();
        }

        $node = ['name' => $this->remap((string) $name->value)];
        if ($alias !== null) {
            $node['alias'] = $this->remap((string) $alias->value);
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /* ===================================================================== *
     *  Class / interface / function definitions
     * ===================================================================== */

    private function parseClass(): array
    {
        $abstract = 0;
        $final    = 0;
        if ($this->accept(TokenType::T_ABSTRACT)) {
            $abstract = 1;
        } elseif ($this->accept(TokenType::T_FINAL)) {
            $final = 1;
        }

        $this->expect(TokenType::T_CLASS);
        $name = $this->expectNameToken();

        $extends = null;
        if ($this->accept(TokenType::T_EXTENDS)) {
            $extends = $this->expectNameToken();
        }

        $implements = null;
        if ($this->accept(TokenType::T_IMPLEMENTS)) {
            $implements = [$this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken())];
            while ($this->accept(TokenType::T_COMMA)) {
                $implements[] = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken());
            }
        }

        $definition = $this->parseClassBody();

        $node = [
            'type'     => 'class',
            'name'     => $this->remap((string) $name->value),
            'abstract' => $abstract,
            'final'    => $final,
        ];
        if ($extends !== null) {
            $node['extends'] = $this->remap((string) $extends->value);
        }
        if ($implements !== null) {
            $node['implements'] = $implements;
        }
        if ($definition !== null) {
            $node['definition'] = $definition;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->classLine();
        $node['char'] = $this->classChar();

        return $node;
    }

    /**
     * Parse a trait definition: `trait Name { members }` (xx_ret_trait).
     * Traits take no modifiers and cannot extends/implements, as in PHP.
     * The node is stamped from the saved class_* state set by the `trait`
     * keyword, exactly like `class`.
     */
    private function parseTrait(): array
    {
        $this->expect(TokenType::T_TRAIT);
        $name = $this->expectNameToken();

        $definition = $this->parseClassBody();

        $node = [
            'type' => 'trait',
            'name' => $this->remap((string) $name->value),
        ];
        if ($definition !== null) {
            $node['definition'] = $definition;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->classLine();
        $node['char'] = $this->classChar();

        return $node;
    }

    /**
     * Parse `{ members }` and bucket members into properties/methods/constants
     * (xx_ret_class_definition_from_list). The definition node uses the saved
     * class_* state at its reduce (lookahead = the closing `}`).
     */
    private function parseClassBody(): ?array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->accept(TokenType::T_BRACKET_CLOSE)) {
            return null; // empty body => no "definition" key
        }

        $members = [];
        while (!$this->check(TokenType::T_BRACKET_CLOSE)) {
            $members[] = $this->parseClassMember();
        }

        // The class_definition reduces with the closing `}` as lookahead.
        $defLine = $this->classLine();
        $defChar = $this->classChar();
        $this->expect(TokenType::T_BRACKET_CLOSE);

        $properties = [];
        $methods    = [];
        $constants  = [];
        $uses       = [];
        foreach ($members as $m) {
            switch ($m['type']) {
                case 'property':
                    $properties[] = $m;
                    break;
                case 'method':
                    $methods[] = $m;
                    break;
                case 'const':
                    $constants[] = $m;
                    break;
                case 'use-trait':
                    $uses[] = $m;
                    break;
                default:
                    break;
            }
        }

        $def = [];
        if ($properties !== []) {
            $def['properties'] = $properties;
        }
        if ($methods !== []) {
            $def['methods'] = $methods;
        }
        if ($constants !== []) {
            $def['constants'] = $constants;
        }
        if ($uses !== []) {
            $def['uses'] = $uses;
        }
        $def['file'] = $this->file;
        $def['line'] = $defLine;
        $def['char'] = $defChar;

        return $def;
    }

    private function parseClassMember(): array
    {
        $docblock = null;
        if ($this->check(TokenType::T_COMMENT)) {
            $docblock = $this->advance();
        }

        if ($this->check(TokenType::T_CONST)) {
            return $this->parseClassConst($docblock);
        }

        if ($this->check(TokenType::T_USE)) {
            return $this->parseUseTrait($docblock);
        }

        $visibility = $this->parseVisibilityList();

        if ($this->check(TokenType::T_FUNCTION)) {
            return $this->parseMethod($visibility, $docblock);
        }

        return $this->parseProperty($visibility, $docblock);
    }

    /**
     * In-class trait use: `use A;` / `use A, B;` (xx_ret_use_trait). The name
     * list reuses the implements-item literal shape. Stamped at reduce time
     * (lookahead after the closing `;`), matching the C grammar.
     */
    private function parseUseTrait(?Token $docblock): array
    {
        $this->expect(TokenType::T_USE);
        $traits = [$this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken())];
        while ($this->accept(TokenType::T_COMMA)) {
            $traits[] = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken());
        }
        $this->expect(TokenType::T_DOTCOMMA);

        $node = [
            'type'   => 'use-trait',
            'traits' => $traits,
        ];
        if ($docblock !== null) {
            $node['docblock'] = $this->remap((string) $docblock->value);
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /**
     * Parse an interface definition. The node is stamped from the saved
     * `class_*` scanner state (0 unless a class preceded it), carried on the
     * INTERFACE token, matching xx_ret_interface which reads `class_line`.
     */
    private function parseInterface(): array
    {
        $this->expect(TokenType::T_INTERFACE);
        $name = $this->expectNameToken();

        $extends = null;
        if ($this->accept(TokenType::T_EXTENDS)) {
            $extends = [$this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken())];
            while ($this->accept(TokenType::T_COMMA)) {
                $extends[] = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->expectNameToken());
            }
        }

        $definition = $this->parseInterfaceBody();

        $node = [
            'type' => 'interface',
            'name' => $this->remap((string) $name->value),
        ];
        if ($extends !== null) {
            $node['extends'] = $extends;
        }
        if ($definition !== null) {
            $node['definition'] = $definition;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->classLine();
        $node['char'] = $this->classChar();

        return $node;
    }

    /**
     * Parse an interface body: constants then method signatures (bucketed into
     * methods/constants like xx_ret_interface_definition, which emits methods
     * before constants). The definition node uses the active lookahead position.
     */
    private function parseInterfaceBody(): ?array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->accept(TokenType::T_BRACKET_CLOSE)) {
            return null;
        }

        $methods   = [];
        $constants = [];
        while (!$this->check(TokenType::T_BRACKET_CLOSE)) {
            $docblock = null;
            if ($this->check(TokenType::T_COMMENT)) {
                $docblock = $this->advance();
            }
            if ($this->check(TokenType::T_CONST)) {
                $constants[] = $this->parseClassConst($docblock);
                continue;
            }
            $visibility = $this->parseVisibilityList();
            $methods[]  = $this->parseInterfaceMethod($visibility, $docblock);
        }

        // The interface_definition non-terminal reduces with the closing `}` as
        // lookahead, so it is stamped at the `}` position (before consuming it).
        $defLine = $this->line();
        $defChar = $this->char();
        $this->expect(TokenType::T_BRACKET_CLOSE);

        $def = [];
        if ($methods !== []) {
            $def['methods'] = $methods;
        }
        if ($constants !== []) {
            $def['constants'] = $constants;
        }
        $def['file'] = $this->file;
        $def['line'] = $defLine;
        $def['char'] = $defChar;

        return $def;
    }

    /** Interface method signature: visibility FUNCTION name(params) [-> rettype] ; */
    private function parseInterfaceMethod(array $visibility, ?Token $docblock): array
    {
        $this->expect(TokenType::T_FUNCTION);
        $name = $this->expectNameToken();

        $parameters = $this->parseParameterParens();
        $returnType = null;
        if ($this->accept(TokenType::T_ARROW)) {
            $returnType = $this->parseReturnType();
        }
        $this->expect(TokenType::T_DOTCOMMA);

        $node = [
            'visibility' => $visibility,
            'type'       => 'method',
            'name'       => $this->remap((string) $name->value),
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        if ($docblock !== null) {
            $node['docblock'] = $this->remap((string) $docblock->value);
        }
        if ($returnType !== null) {
            $node['return-type'] = $returnType;
        }
        $node['file']      = $this->file;
        $node['line']      = $this->methodLine();
        $node['last-line'] = $this->line();
        $node['char']      = $this->methodChar();

        return $node;
    }

    /** @return list<string> */
    private function parseVisibilityList(): array
    {
        $list = [];
        while (isset(self::VISIBILITY[$this->peekType()])) {
            $list[] = self::VISIBILITY[$this->advance()->opcode];
        }
        if ($list === []) {
            $this->syntaxError();
        }

        return $list;
    }

    private function parseProperty(array $visibility, ?Token $docblock): array
    {
        $name = $this->expectNameToken();

        $default   = null;
        $shortcuts = null;
        if ($this->accept(TokenType::T_ASSIGN)) {
            $default = $this->parseLiteralExpr();
        }
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            $shortcuts = $this->parsePropertyShortcuts();
        }
        $this->expect(TokenType::T_DOTCOMMA);

        $node = [
            'visibility' => $visibility,
            'type'       => 'property',
            'name'       => $this->remap((string) $name->value),
        ];
        if ($default !== null) {
            $node['default'] = $default;
        }
        if ($docblock !== null) {
            $node['docblock'] = $this->remap((string) $docblock->value);
        }
        if ($shortcuts !== null) {
            $node['shortcuts'] = $shortcuts;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parsePropertyShortcuts(): ?array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->accept(TokenType::T_BRACKET_CLOSE)) {
            return null;
        }

        $list = [$this->parsePropertyShortcut()];
        while ($this->accept(TokenType::T_COMMA)) {
            $list[] = $this->parsePropertyShortcut();
        }
        $this->expect(TokenType::T_BRACKET_CLOSE);

        return $list;
    }

    private function parsePropertyShortcut(): array
    {
        $doc = null;
        if ($this->check(TokenType::T_COMMENT)) {
            $doc = $this->advance();
        }
        $name = $this->expect(TokenType::T_IDENTIFIER);

        $node = ['type' => 'shortcut'];
        if ($doc !== null) {
            $node['docblock'] = $this->remap((string) $doc->value);
        }
        $node['name'] = $this->remap((string) $name->value);
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseClassConst(?Token $docblock): array
    {
        $this->expect(TokenType::T_CONST);
        $name = $this->expectNameToken();
        $this->expect(TokenType::T_ASSIGN);
        $default = $this->parseLiteralExpr();
        $this->expect(TokenType::T_DOTCOMMA);

        $node = [
            'type'    => 'const',
            'name'    => $this->remap((string) $name->value),
            'default' => $default,
        ];
        if ($docblock !== null) {
            $node['docblock'] = $this->remap((string) $docblock->value);
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseMethod(array $visibility, ?Token $docblock): array
    {
        $this->expect(TokenType::T_FUNCTION);
        $name = $this->expectNameToken();

        $parameters = $this->parseParameterParens();
        $returnType = null;
        if ($this->accept(TokenType::T_ARROW)) {
            $returnType = $this->parseReturnType();
        }
        $statements = $this->parseMethodBody();

        $node = [
            'visibility' => $visibility,
            'type'       => 'method',
            'name'       => $this->remap((string) $name->value),
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        if ($statements !== null) {
            $node['statements'] = $statements;
        }
        if ($docblock !== null) {
            $node['docblock'] = $this->remap((string) $docblock->value);
        }
        if ($returnType !== null) {
            $node['return-type'] = $returnType;
        }
        $node['file']      = $this->file;
        $node['line']      = $this->methodLine();
        $node['last-line'] = $this->line();
        $node['char']      = $this->methodChar();

        return $node;
    }

    private function parseFunction(): array
    {
        $this->expect(TokenType::T_FUNCTION);
        $name = $this->expectNameToken();

        $parameters = $this->parseParameterParens();
        $returnType = null;
        if ($this->accept(TokenType::T_ARROW)) {
            $returnType = $this->parseReturnType();
        }
        $statements = $this->parseMethodBody();

        $node = [
            'type' => 'function',
            'name' => $this->remap((string) $name->value),
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        if ($statements !== null) {
            $node['statements'] = $statements;
        }
        if ($returnType !== null) {
            $node['return-type'] = $returnType;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->methodLine();
        $node['char'] = $this->methodChar();

        return $node;
    }

    /** @return list<array>|null parameters, or null when the parens are empty/absent */
    private function parseParameterParens(): ?array
    {
        $this->expect(TokenType::T_PARENTHESES_OPEN);
        if ($this->accept(TokenType::T_PARENTHESES_CLOSE)) {
            return null;
        }

        $params = [$this->parseParameter()];
        while ($this->accept(TokenType::T_COMMA)) {
            $params[] = $this->parseParameter();
        }
        $this->expect(TokenType::T_PARENTHESES_CLOSE);

        return $params;
    }

    /** @return list<array>|null statement list, or null for an empty `{}` / `;` body */
    private function parseMethodBody(): ?array
    {
        if ($this->accept(TokenType::T_DOTCOMMA)) {
            return null;
        }
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->accept(TokenType::T_BRACKET_CLOSE)) {
            return null;
        }
        $statements = $this->parseStatementList();
        $this->expect(TokenType::T_BRACKET_CLOSE);

        return $statements;
    }

    private function parseParameter(): array
    {
        $const     = 0;
        $reference = 0;
        $mandatory = 0;
        $variadic  = 0;
        $type      = null;
        $cast      = null;
        $default   = null;

        if ($this->accept(TokenType::T_CONST)) {
            $const = 1;
        }

        if (isset(self::TYPE_NAMES[$this->peekType()])) {
            $type = self::TYPE_NAMES[$this->advance()->opcode];
        } elseif ($this->check(TokenType::T_LESS)) {
            $cast = $this->parseCast();
        }

        if ($type !== null && $this->accept(TokenType::T_NOT)) {
            $mandatory = 1;
        }
        if ($this->accept(TokenType::T_EXCLUSIVE_RANGE)) {
            $variadic = 1;
        }
        if ($this->accept(TokenType::T_BITWISE_AND)) {
            $reference = 1;
        }

        $name = $this->expectNameToken();

        if ($this->accept(TokenType::T_ASSIGN)) {
            $default = $this->parseLiteralExpr();
        }

        $node = [
            'type' => 'parameter',
            'name' => $this->remap((string) $name->value),
            'const' => $const,
        ];
        if ($type !== null) {
            $node['data-type'] = $type;
            $node['mandatory'] = $mandatory;
        } else {
            $node['data-type'] = 'variable';
            $node['mandatory'] = 0;
        }
        if ($cast !== null) {
            $node['cast'] = $cast;
        }
        if ($default !== null) {
            $node['default'] = $default;
        }
        $node['reference'] = $reference;
        $node['file']      = $this->file;
        $node['line']      = $this->line();
        $node['char']      = $this->char();
        if ($variadic === 1) {
            $node['variadic'] = 1;
        }

        return $node;
    }

    /**
     * Parse a `<Class>` / `<static>` / `<Class[]>` cast literal. Returns
     * [node, isCollection].
     *
     * @return array{0: array, 1: bool}
     */
    private function parseCastRaw(): array
    {
        $this->expect(TokenType::T_LESS);

        if ($this->check(TokenType::T_STATIC)) {
            $this->advance();
            $this->expect(TokenType::T_GREATER);
            $node = [
                'type'  => 'variable',
                'value' => 'static',
                'file'  => $this->file,
                'line'  => $this->line(),
                'char'  => $this->char(),
            ];

            return [$node, false];
        }

        $ident      = $this->expect(TokenType::T_IDENTIFIER);
        $collection = false;
        if ($this->accept(TokenType::T_SBRACKET_OPEN)) {
            $this->expect(TokenType::T_SBRACKET_CLOSE);
            $collection = true;
        }
        $this->expect(TokenType::T_GREATER);

        return [$this->literalFromToken(TokenType::T_IDENTIFIER, $ident), $collection];
    }

    private function parseCast(): array
    {
        return $this->parseCastRaw()[0];
    }

    private function parseReturnType(): array
    {
        if ($this->accept(TokenType::T_VOID)) {
            return [
                'type' => 'return-type',
                'void' => 1,
                'file' => $this->file,
                'line' => $this->line(),
                'char' => $this->char(),
            ];
        }

        $list = [$this->parseReturnTypeItem()];
        while ($this->accept(TokenType::T_BITWISE_OR)) {
            $list[] = $this->parseReturnTypeItem();
        }

        return [
            'type' => 'return-type',
            'list' => $list,
            'void' => 0,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function parseReturnTypeItem(): array
    {
        $dataType   = null;
        $cast       = null;
        $mandatory  = 0;
        $collection = 0;

        if (isset(self::TYPE_NAMES[$this->peekType()])) {
            $dataType = self::TYPE_NAMES[$this->advance()->opcode];
            if ($this->accept(TokenType::T_NOT)) {
                $mandatory = 1;
            }
        } elseif ($this->accept(TokenType::T_NULL)) {
            $dataType = 'null';
        } elseif ($this->accept(TokenType::T_TYPE_THIS) || $this->acceptTypeThisKeyword()) {
            $dataType = 'this';
        } elseif ($this->accept(TokenType::T_FALSE)) {
            $dataType = 'false';
        } elseif ($this->check(TokenType::T_LESS)) {
            [$cast, $isCollection] = $this->parseCastRaw();
            $collection            = $isCollection ? 1 : 0;
        } else {
            $this->syntaxError();
        }

        $node = ['type' => 'return-type-parameter'];
        if ($dataType !== null) {
            $node['data-type'] = $dataType;
            $node['mandatory'] = $mandatory;
        }
        if ($cast !== null) {
            $node['cast']       = $cast;
            $node['collection'] = $collection;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function acceptTypeThisKeyword(): bool
    {
        // `this` is lexed as an IDENTIFIER (not a dedicated keyword). The
        // grammar's THIS return-type item accepts the identifier "this".
        if ($this->check(TokenType::T_IDENTIFIER) && $this->peek()->value === 'this') {
            $this->advance();

            return true;
        }

        return false;
    }

    /* ===================================================================== *
     *  Statements
     * ===================================================================== */

    /** @return list<array> */
    private function parseStatementList(): array
    {
        $list = [];
        while (!$this->check(TokenType::T_BRACKET_CLOSE) && $this->peekType() !== 0) {
            $list[] = $this->parseStatement();
        }

        return $list;
    }

    private function parseStatement(): array
    {
        $type = $this->peekType();

        if (isset(self::TYPE_NAMES[$type])) {
            return $this->parseDeclare();
        }

        switch ($type) {
            case TokenType::T_COMMENT:
                return $this->parseComment();
            case TokenType::T_CBLOCK:
                return $this->parseCblock();
            case TokenType::T_LET:
                return $this->parseLet();
            case TokenType::T_RETURN:
                return $this->parseReturn();
            case TokenType::T_ECHO:
                return $this->parseEcho();
            case TokenType::T_IF:
                return $this->parseIf();
            case TokenType::T_LOOP:
                return $this->parseLoop();
            case TokenType::T_WHILE:
                return $this->parseWhile();
            case TokenType::T_DO:
                return $this->parseDoWhile();
            case TokenType::T_FOR:
                return $this->parseFor();
            case TokenType::T_SWITCH:
                return $this->parseSwitch();
            case TokenType::T_TRY:
                return $this->parseTryCatch();
            case TokenType::T_BREAK:
                $this->advance();
                $this->expect(TokenType::T_DOTCOMMA);

                return $this->simpleStatement('break');
            case TokenType::T_CONTINUE:
                $this->advance();
                $this->expect(TokenType::T_DOTCOMMA);

                return $this->simpleStatement('continue');
            case TokenType::T_THROW:
                return $this->parseExprStatement('throw');
            case TokenType::T_UNSET:
                return $this->parseExprStatement('unset');
            case TokenType::T_REQUIRE:
                return $this->parseExprStatement('require');
            case TokenType::T_REQUIRE_ONCE:
                return $this->parseExprStatement('require_once');
            case TokenType::T_FETCH:
                return $this->parseFetchStatement();
            case TokenType::T_YIELD:
                return $this->parseYield();
            case TokenType::T_DOTCOMMA:
                $this->advance();

                return $this->simpleStatement('empty');
            default:
                break;
        }

        // Expression statement: must be a function/method/static call.
        return $this->parseCallStatement();
    }

    private function parseReturn(): array
    {
        $this->expect(TokenType::T_RETURN);
        $expr = null;
        if (!$this->check(TokenType::T_DOTCOMMA)) {
            $expr = $this->parseExpr(0);
        }
        $this->expect(TokenType::T_DOTCOMMA);

        $node = ['type' => 'return'];
        if ($expr !== null) {
            $node['expr'] = $expr;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseEcho(): array
    {
        $this->expect(TokenType::T_ECHO);
        $expressions = [$this->parseExpr(0)];
        while ($this->accept(TokenType::T_COMMA)) {
            $expressions[] = $this->parseExpr(0);
        }
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type'        => 'echo',
            'expressions' => $expressions,
            'file'        => $this->file,
            'line'        => $this->line(),
            'char'        => $this->char(),
        ];
    }

    private function parseCallStatement(): array
    {
        $expr = $this->parseExpr(0);

        // Only call expressions are valid statements. The C parser rejects a
        // bare expression at the terminating `;` lookahead (before consuming
        // it), so raise the error here for matching error-node positions.
        $type = $expr['type'] ?? '';
        if ($type !== 'fcall' && $type !== 'mcall' && $type !== 'scall') {
            $this->syntaxError();
        }

        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type' => $type,
            'expr' => $expr,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function simpleStatement(string $type): array
    {
        return [
            'type' => $type,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    /** throw/unset/require/require_once: KEYWORD common_expr `;` => {type, expr, ...}. */
    private function parseExprStatement(string $type): array
    {
        $this->advance();
        $expr = $this->parseExpr(0);
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type' => $type,
            'expr' => $expr,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function parseFetchStatement(): array
    {
        $expr = $this->parseFetch();
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type' => 'fetch',
            'expr' => $expr,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function parseYield(): array
    {
        $this->expect(TokenType::T_YIELD);

        $node = ['type' => 'yield'];
        if (!$this->check(TokenType::T_DOTCOMMA)) {
            $first = $this->parseExpr(0);
            if ($this->accept(TokenType::T_COMMA)) {
                $value         = $this->parseExpr(0);
                $node['key']   = $first;
                $node['value'] = $value;
            } else {
                $node['expr'] = $first;
            }
        }
        $this->expect(TokenType::T_DOTCOMMA);
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /* ----- control flow ------------------------------------------------- */

    /** Parse `{ [statements] }`, returning null for an empty block. */
    private function parseBlock(): ?array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->accept(TokenType::T_BRACKET_CLOSE)) {
            return null;
        }
        $statements = $this->parseStatementList();
        $this->expect(TokenType::T_BRACKET_CLOSE);

        return $statements;
    }

    private function parseIf(): array
    {
        $this->expect(TokenType::T_IF);
        $expr = $this->parseExpr(0);
        $body = $this->parseBlock();

        $elseifs = null;
        if ($this->check(TokenType::T_ELSEIF)) {
            $elseifs = [];
            while ($this->check(TokenType::T_ELSEIF)) {
                $elseifs[] = $this->parseElseif();
            }
        }

        $hasElse   = false;
        $elseBody  = null;
        if ($this->accept(TokenType::T_ELSE)) {
            $hasElse  = true;
            $elseBody = $this->parseBlock();
        }

        // Mirror the C builder's argument routing, including its quirk: an
        // empty if-body with elseif clauses and no else stores the elseif list
        // in the else_statements slot.
        if ($elseifs !== null && $body === null && !$hasElse) {
            $statements        = null;
            $elseifStatements  = null;
            $elseStatements    = $elseifs;
        } else {
            $statements       = $body;
            $elseifStatements = $elseifs;
            $elseStatements   = $hasElse ? $elseBody : null;
        }

        $node = ['type' => 'if', 'expr' => $expr];
        if ($statements !== null) {
            $node['statements'] = $statements;
        }
        if ($elseifStatements !== null) {
            $node['elseif_statements'] = $elseifStatements;
        }
        if ($elseStatements !== null) {
            $node['else_statements'] = $elseStatements;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseElseif(): array
    {
        $this->expect(TokenType::T_ELSEIF);
        $expr = $this->parseExpr(0);
        $body = $this->parseBlock();

        $node = ['type' => 'if', 'expr' => $expr];
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseLoop(): array
    {
        $this->expect(TokenType::T_LOOP);
        $body = $this->parseBlock();

        $node = ['type' => 'loop'];
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseWhile(): array
    {
        $this->expect(TokenType::T_WHILE);
        $expr = $this->parseExpr(0);
        $body = $this->parseBlock();

        $node = ['type' => 'while', 'expr' => $expr];
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseDoWhile(): array
    {
        $this->expect(TokenType::T_DO);
        $body = $this->parseBlock();
        $this->expect(TokenType::T_WHILE);
        $expr = $this->parseExpr(0);
        $this->expect(TokenType::T_DOTCOMMA);

        $node = ['type' => 'do-while', 'expr' => $expr];
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseFor(): array
    {
        $this->expect(TokenType::T_FOR);
        $paren = $this->accept(TokenType::T_PARENTHESES_OPEN);

        $key   = null;
        $value = $this->expect(TokenType::T_IDENTIFIER);
        if ($this->accept(TokenType::T_COMMA)) {
            $key   = $value;
            $value = $this->expect(TokenType::T_IDENTIFIER);
        }

        $this->expect(TokenType::T_IN);
        $reverse = $this->accept(TokenType::T_REVERSE) ? 1 : 0;
        $expr    = $this->parseExpr(0);

        if ($paren) {
            $this->expect(TokenType::T_PARENTHESES_CLOSE);
        }
        $body = $this->parseBlock();

        $node = ['type' => 'for', 'expr' => $expr];
        if ($key !== null) {
            $node['key'] = $this->remap((string) $key->value);
        }
        $node['value']   = $this->remap((string) $value->value);
        $node['reverse'] = $reverse;
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseSwitch(): array
    {
        $this->expect(TokenType::T_SWITCH);
        $expr = $this->parseExpr(0);
        $this->expect(TokenType::T_BRACKET_OPEN);

        $clauses = null;
        if (!$this->check(TokenType::T_BRACKET_CLOSE)) {
            $clauses = [];
            while ($this->check(TokenType::T_CASE) || $this->check(TokenType::T_DEFAULT)) {
                $clauses[] = $this->parseCaseClause();
            }
        }
        $this->expect(TokenType::T_BRACKET_CLOSE);

        $node = ['type' => 'switch', 'expr' => $expr];
        if ($clauses !== null) {
            $node['clauses'] = $clauses;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseCaseClause(): array
    {
        if ($this->accept(TokenType::T_DEFAULT)) {
            $this->expect(TokenType::T_COLON);
            $statements = $this->parseCaseBody();

            $node = ['type' => 'default'];
            if ($statements !== null) {
                $node['statements'] = $statements;
            }
            $node['file'] = $this->file;
            $node['line'] = $this->line();
            $node['char'] = $this->char();

            return $node;
        }

        $this->expect(TokenType::T_CASE);
        $expr = $this->parseExpr(0);
        $this->expect(TokenType::T_COLON);
        $statements = $this->parseCaseBody();

        $node = ['type' => 'case', 'expr' => $expr];
        if ($statements !== null) {
            $node['statements'] = $statements;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /** Statements within a case/default clause, up to the next clause or `}`. */
    private function parseCaseBody(): ?array
    {
        $list = [];
        while (
            !$this->check(TokenType::T_CASE)
            && !$this->check(TokenType::T_DEFAULT)
            && !$this->check(TokenType::T_BRACKET_CLOSE)
            && $this->peekType() !== 0
        ) {
            $list[] = $this->parseStatement();
        }

        return $list === [] ? null : $list;
    }

    private function parseTryCatch(): array
    {
        $this->expect(TokenType::T_TRY);
        $body = $this->parseBlock();

        $catches = null;
        if ($this->check(TokenType::T_CATCH)) {
            $catches = [];
            while ($this->check(TokenType::T_CATCH)) {
                $catches[] = $this->parseCatch();
            }
        }

        $node = ['type' => 'try-catch'];
        if ($body !== null) {
            $node['statements'] = $body;
        }
        if ($catches !== null) {
            $node['catches'] = $catches;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseCatch(): array
    {
        $this->expect(TokenType::T_CATCH);

        $classes = [$this->literalFromToken(TokenType::T_IDENTIFIER, $this->expect(TokenType::T_IDENTIFIER))];
        while ($this->accept(TokenType::T_BITWISE_OR)) {
            $classes[] = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->expect(TokenType::T_IDENTIFIER));
        }

        $variableTok = null;
        if ($this->accept(TokenType::T_COMMA)) {
            $variableTok = $this->expect(TokenType::T_IDENTIFIER);
        }

        $body = $this->parseBlock();

        $node = ['classes' => $classes];
        if ($variableTok !== null) {
            // The C builder constructs this literal in the catch reduce action,
            // so it is stamped at the post-block lookahead, not where consumed.
            $node['variable'] = $this->literalFromToken(TokenType::T_IDENTIFIER, $variableTok);
        }
        if ($body !== null) {
            $node['statements'] = $body;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseDeclare(): array
    {
        $dataType = self::TYPE_NAMES[$this->advance()->opcode];

        $variables = [$this->parseDeclareVariable()];
        while ($this->accept(TokenType::T_COMMA)) {
            $variables[] = $this->parseDeclareVariable();
        }
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type'      => 'declare',
            'data-type' => $dataType,
            'variables' => $variables,
            'file'      => $this->file,
            'line'      => $this->line(),
            'char'      => $this->char(),
        ];
    }

    private function parseDeclareVariable(): array
    {
        $name = $this->expectNameToken();
        $expr = null;
        if ($this->accept(TokenType::T_ASSIGN)) {
            $expr = $this->parseExpr(0);
        }

        $node = ['variable' => $this->remap((string) $name->value)];
        if ($expr !== null) {
            $node['expr'] = $expr;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /* ----- let statement & assignment targets --------------------------- */

    private function parseLet(): array
    {
        $this->expect(TokenType::T_LET);
        $assignments = [$this->parseLetAssignment()];
        while ($this->accept(TokenType::T_COMMA)) {
            $assignments[] = $this->parseLetAssignment();
        }
        $this->expect(TokenType::T_DOTCOMMA);

        return [
            'type'        => 'let',
            'assignments' => $assignments,
            'file'        => $this->file,
            'line'        => $this->line(),
            'char'        => $this->char(),
        ];
    }

    private function parseLetAssignment(): array
    {
        // {y} = expr  /  {"y"} = expr  (dynamic variable target)
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            return $this->parseLetDynamicVariable();
        }

        // [a, b, ...] = expr  (destructuring assignment)
        if ($this->check(TokenType::T_SBRACKET_OPEN)) {
            return $this->parseLetDestructure();
        }

        if (!$this->check(TokenType::T_IDENTIFIER) && !$this->check(TokenType::T_CONSTANT)) {
            $this->unsupported();
        }

        $variable = $this->advance();

        // y->... forms
        if ($this->check(TokenType::T_ARROW)) {
            return $this->parseLetObjectProperty($variable);
        }

        // y::prop forms
        if ($this->check(TokenType::T_DOUBLECOLON)) {
            return $this->parseLetStaticProperty($variable);
        }

        // y[] / y[x] / y[x][] forms
        if ($this->check(TokenType::T_SBRACKET_OPEN)) {
            return $this->parseIndexedAssignment('variable-append', 'array-index', 'array-index-append', $variable, null);
        }

        if ($this->accept(TokenType::T_INCR)) {
            return $this->letAssignment('incr', null, $variable, null, null, null);
        }
        if ($this->accept(TokenType::T_DECR)) {
            return $this->letAssignment('decr', null, $variable, null, null, null);
        }

        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return $this->letAssignment('variable', $operator, $variable, null, null, $expr);
    }

    private function parseLetDynamicVariable(): array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        if ($this->check(TokenType::T_IDENTIFIER)) {
            $variable   = $this->advance();
            $assignType = 'dynamic-variable';
        } elseif ($this->check(TokenType::T_STRING)) {
            $variable   = $this->advance();
            $assignType = 'dynamic-variable-string';
        } else {
            $this->syntaxError();
        }
        $this->expect(TokenType::T_BRACKET_CLOSE);
        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return $this->letAssignment($assignType, $operator, $variable, null, null, $expr);
    }

    /**
     * Destructuring target: `[a, b, , c] = expr`. Each slot is an identifier
     * literal (type "variable") or null for a skipped slot. operator is the
     * hardcoded string "assign" (only `=` is grammatical here).
     */
    private function parseLetDestructure(): array
    {
        $this->expect(TokenType::T_SBRACKET_OPEN);

        $variables = [$this->parseDestructureVar()];
        while ($this->accept(TokenType::T_COMMA)) {
            $variables[] = $this->parseDestructureVar();
        }
        $this->expect(TokenType::T_SBRACKET_CLOSE);
        $this->expect(TokenType::T_ASSIGN);
        $expr = $this->parseExpr(0);

        return [
            'assign-type' => 'destructure',
            'operator'    => 'assign',
            'variables'   => $variables,
            'expr'        => $expr,
            'file'        => $this->file,
            'line'        => $this->line(),
            'char'        => $this->char(),
        ];
    }

    /** A destructure slot: identifier/constant literal, or null for an empty slot. */
    private function parseDestructureVar(): ?array
    {
        if ($this->check(TokenType::T_IDENTIFIER) || $this->check(TokenType::T_CONSTANT)) {
            return $this->literalFromToken(TokenType::T_IDENTIFIER, $this->advance());
        }

        return null;
    }

    private function parseLetObjectProperty(Token $variable): array
    {
        $this->expect(TokenType::T_ARROW);

        // y->{x} / y->{"x"}
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            $this->advance();
            if ($this->check(TokenType::T_IDENTIFIER)) {
                $prop = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);
                $operator = $this->parseAssignmentOperator();
                $expr     = $this->parseExpr(0);

                return $this->letAssignment('variable-dynamic-object-property', $operator, $variable, $prop, null, $expr);
            }
            if ($this->check(TokenType::T_STRING)) {
                $prop = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);
                $operator = $this->parseAssignmentOperator();
                $expr     = $this->parseExpr(0);

                return $this->letAssignment('string-dynamic-object-property', $operator, $variable, $prop, null, $expr);
            }
            $this->syntaxError();
        }

        $prop = $this->expectNameToken();

        // Nested chains: a->b->c[->...] = expr  (assign-type "property-access").
        if ($this->check(TokenType::T_ARROW)) {
            return $this->parseLetNestedProperty($variable, $prop);
        }

        if ($this->accept(TokenType::T_INCR)) {
            return $this->letAssignment('object-property-incr', null, $variable, $prop, null, null);
        }
        if ($this->accept(TokenType::T_DECR)) {
            return $this->letAssignment('object-property-decr', null, $variable, $prop, null, null);
        }

        if ($this->check(TokenType::T_SBRACKET_OPEN)) {
            return $this->parseIndexedAssignment(
                'object-property-append',
                'object-property-array-index',
                'object-property-array-index-append',
                $variable,
                $prop
            );
        }

        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return $this->letAssignment('object-property', $operator, $variable, $prop, null, $expr);
    }

    /**
     * Parse a nested property-access assignment target: receiver->...->prop = expr.
     * The receiver chain is built as nested property-access expr nodes; the
     * final `->prop` becomes the assignment's "property". The receiver literals
     * and nodes are stamped at the *next* ARROW (which is the current lookahead
     * when each level is built), matching the C parser's reduce timing.
     */
    private function parseLetNestedProperty(Token $variable, Token $firstProp): array
    {
        // Base receiver `variable->firstProp`, stamped at the upcoming ARROW.
        $recv = $this->expr(
            'property-access',
            $this->literalFromToken(TokenType::T_IDENTIFIER, $variable),
            $this->literalFromToken(TokenType::T_IDENTIFIER, $firstProp),
            null
        );

        $finalProp = null;
        while ($this->check(TokenType::T_ARROW)) {
            $this->advance();
            $member = $this->expectNameToken();
            if ($this->check(TokenType::T_ARROW)) {
                $recv = $this->expr(
                    'property-access',
                    $recv,
                    $this->literalFromToken(TokenType::T_IDENTIFIER, $member),
                    null
                );
                continue;
            }
            $finalProp = $member;
            break;
        }

        if ($finalProp === null) {
            $this->syntaxError();
        }

        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return [
            'assign-type' => 'property-access',
            'operator'    => $operator,
            'left'        => $recv,
            'property'    => $this->remap((string) $finalProp->value),
            'expr'        => $expr,
            'file'        => $this->file,
            'line'        => $this->line(),
            'char'        => $this->char(),
        ];
    }

    private function parseLetStaticProperty(Token $variable): array
    {
        $this->expect(TokenType::T_DOUBLECOLON);
        $prop = $this->expectNameToken();

        if ($this->check(TokenType::T_SBRACKET_OPEN)) {
            return $this->parseIndexedAssignment(
                'static-property-append',
                'static-property-array-index',
                'static-property-array-index-append',
                $variable,
                $prop
            );
        }

        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return $this->letAssignment('static-property', $operator, $variable, $prop, null, $expr);
    }

    /** @return list<array> */
    private function parseArrayOffsetList(): array
    {
        $offsets = [];
        while ($this->check(TokenType::T_SBRACKET_OPEN) && $this->laType(1) !== TokenType::T_SBRACKET_CLOSE) {
            $this->advance(); // [
            $offsets[] = $this->parseExpr(0);
            $this->expect(TokenType::T_SBRACKET_CLOSE);
        }

        return $offsets;
    }

    /** Consume an empty `[]` pair at the cursor, returning whether it was present. */
    private function acceptEmptyBrackets(): bool
    {
        if ($this->check(TokenType::T_SBRACKET_OPEN) && $this->laType(1) === TokenType::T_SBRACKET_CLOSE) {
            $this->advance();
            $this->advance();

            return true;
        }

        return false;
    }

    /**
     * Parse the index suffix of a let target (`[]`, `[x]`, `[x][]`) followed by
     * `op expr`, shared by the variable, object-property and static-property
     * forms. The three assign-type strings select the variant.
     */
    private function parseIndexedAssignment(
        string $appendType,
        string $indexType,
        string $indexAppendType,
        Token $variable,
        ?Token $property
    ): array {
        if ($this->acceptEmptyBrackets()) {
            $operator = $this->parseAssignmentOperator();
            $expr     = $this->parseExpr(0);

            return $this->letAssignment($appendType, $operator, $variable, $property, null, $expr);
        }

        $offsets  = $this->parseArrayOffsetList();
        $append   = $this->acceptEmptyBrackets();
        $operator = $this->parseAssignmentOperator();
        $expr     = $this->parseExpr(0);

        return $this->letAssignment(
            $append ? $indexAppendType : $indexType,
            $operator,
            $variable,
            $property,
            $offsets,
            $expr
        );
    }

    private function parseAssignmentOperator(): string
    {
        $op = self::ASSIGN_OPS[$this->peekType()] ?? null;
        if ($op === null) {
            $this->syntaxError();
        }
        $this->advance();

        return $op;
    }

    /**
     * Build a let-assignment node (xx_ret_let_assignment field order:
     * assign-type, operator?, variable, property?, index-expr?, expr?, file, line, char).
     */
    private function letAssignment(
        string $assignType,
        ?string $operator,
        Token $variable,
        ?Token $property,
        ?array $indexExpr,
        ?array $expr
    ): array {
        $node = ['assign-type' => $assignType];
        if ($operator !== null) {
            $node['operator'] = $operator;
        }
        $node['variable'] = $this->remap((string) $variable->value);
        if ($property !== null) {
            $node['property'] = $this->remap((string) $property->value);
        }
        if ($indexExpr !== null) {
            $node['index-expr'] = $indexExpr;
        }
        if ($expr !== null) {
            $node['expr'] = $expr;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /* ===================================================================== *
     *  Expressions (Pratt)
     * ===================================================================== */

    private function parseExpr(int $minBp): array
    {
        // Depth guard, see MAX_EXPR_DEPTH. Very deep nesting becomes an error
        // node (through SyntaxError, caught by parse()) instead of a fatal that
        // runs out of stack. Undo the counter before throwing so the guard does
        // not leak depth.
        if (++$this->exprDepth > self::MAX_EXPR_DEPTH) {
            --$this->exprDepth;
            $this->syntaxError();
        }

        try {
            $left = $this->parseUnary();

            while (true) {
                $type = $this->peekType();

                // ternary (`?:`) and closure-arrow (`=>`) bind looser than the
                // tabled infix operators; fold their precedence guards into the
                // conditions so an out-of-range operator falls through to `break`.
                if ($type === TokenType::T_QUESTION && $minBp < 50) {
                    $left = $this->parseTernary($left);
                    continue;
                }
                // The grammar's closure-arrow rule is `IDENTIFIER DOUBLEARROW
                // xx_common_expr` — a bare identifier only. Any other left
                // operand (literal, list, call, ...) must fall through so the
                // dangling `=>` produces the same syntax error the C parser emits.
                if ($type === TokenType::T_DOUBLEARROW && $minBp < 40 && 'variable' === ($left['type'] ?? null)) {
                    $left = $this->parseClosureArrow($left);
                    continue;
                }

                if (!isset(self::INFIX[$type])) {
                    break;
                }
                [$bp, $name, $rightAssoc] = self::INFIX[$type];
                if ($bp <= $minBp) {
                    break;
                }
                $this->advance();
                $right = $this->parseExpr($rightAssoc ? $bp - 1 : $bp);
                $left  = $this->expr($name, $left, $right, null);
            }

            return $left;
        } finally {
            --$this->exprDepth;
        }
    }

    private function parseClosureArrow(array $left): array
    {
        $this->expect(TokenType::T_DOUBLEARROW);
        $right = $this->parseExpr(39);

        // The grammar's `IDENTIFIER DOUBLEARROW expr` builds a fresh identifier
        // literal in the reduce action, so the left literal is stamped at the
        // post-RHS lookahead — rebuild it here.
        $leftLit = [
            'type'  => 'variable',
            'value' => $left['value'] ?? '',
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];

        return $this->expr('closure-arrow', $leftLit, $right, null);
    }

    private function parseTernary(array $left): array
    {
        $this->expect(TokenType::T_QUESTION);
        if ($this->accept(TokenType::T_COLON)) {
            $right = $this->parseExpr(49);

            return $this->expr('short-ternary', $left, null, $right);
        }

        $middle = $this->parseExpr(0);
        $this->expect(TokenType::T_COLON);
        $right = $this->parseExpr(49);

        return $this->expr('ternary', $left, $middle, $right);
    }

    /** Prefix operators and primaries, followed by the postfix chain. */
    private function parseUnary(): array
    {
        $type = $this->peekType();

        switch ($type) {
            case TokenType::T_BITWISE_AND:
                $this->advance();

                return $this->expr('reference', $this->parseExpr(240), null, null);
            case TokenType::T_NOT:
                $this->advance();

                return $this->expr('not', $this->parseExpr(220), null, null);
            case TokenType::T_BITWISE_NOT:
                $this->advance();

                return $this->expr('bitwise_not', $this->parseExpr(240), null, null);
            case TokenType::T_SUB:
                $this->advance();

                return $this->expr('minus', $this->parseExpr(220), null, null);
            // No case for unary plus. zephir.lemon has a `PLUS xx_common_expr`
            // rule, but the compiled LALR parser hides it (the `[NOT]` precedence
            // resolves the conflict against a leading `+`), so the C extension
            // returns a "Syntax error" for `+expr`. Sending it to unsupported()
            // keeps the pure PHP parser identical to the ext. Checked against
            // ext 2.0.4 by comparing output directly.
            case TokenType::T_ISSET:
                $this->advance();

                return $this->expr('isset', $this->parseExpr(170), null, null);
            case TokenType::T_REQUIRE_ONCE:
                $this->advance();

                return $this->expr('require_once', $this->parseExpr(20), null, null);
            case TokenType::T_REQUIRE:
                $this->advance();

                return $this->expr('require', $this->parseExpr(30), null, null);
            case TokenType::T_CLONE:
                $this->advance();

                return $this->expr('clone', $this->parseExpr(200), null, null);
            case TokenType::T_EMPTY:
                $this->advance();

                return $this->expr('empty', $this->parseExpr(170), null, null);
            case TokenType::T_LIKELY:
                $this->advance();

                return $this->expr('likely', $this->parseExpr(60), null, null);
            case TokenType::T_UNLIKELY:
                $this->advance();

                return $this->expr('unlikely', $this->parseExpr(60), null, null);
            case TokenType::T_TYPEOF:
                $this->advance();

                return $this->expr('typeof', $this->parseExpr(190), null, null);
            case TokenType::T_FETCH:
                return $this->parseFetch();
            case TokenType::T_NEW:
                return $this->parsePostfix($this->parseNew());
            default:
                break;
        }

        return $this->parsePostfix($this->parsePrimary());
    }

    private function parseFetch(): array
    {
        $this->expect(TokenType::T_FETCH);
        $ident = $this->expect(TokenType::T_IDENTIFIER);
        $this->expect(TokenType::T_COMMA);
        $rhs = $this->parseExpr(170);

        return $this->expr('fetch', $this->literalFromToken(TokenType::T_IDENTIFIER, $ident), $rhs, null);
    }

    private function parsePrimary(): array
    {
        $type = $this->peekType();

        switch ($type) {
            case TokenType::T_INTEGER:
                return $this->literalFromToken(TokenType::T_INTEGER, $this->advance());
            case TokenType::T_DOUBLE:
                return $this->literalFromToken(TokenType::T_DOUBLE, $this->advance());
            case TokenType::T_STRING:
                return $this->literalFromToken(TokenType::T_STRING, $this->advance());
            case TokenType::T_ISTRING:
                return $this->literalFromToken(TokenType::T_ISTRING, $this->advance());
            case TokenType::T_CHAR:
                return $this->literalFromToken(TokenType::T_CHAR, $this->advance());
            case TokenType::T_NULL:
                $this->advance();

                return $this->literalNoValue('null');
            case TokenType::T_TRUE:
                $this->advance();

                return $this->boolLiteral('true');
            case TokenType::T_FALSE:
                $this->advance();

                return $this->boolLiteral('false');
            case TokenType::T_IDENTIFIER:
            case TokenType::T_CONSTANT:
                return $this->parseNameLed();
            case TokenType::T_STATIC:
                return $this->parseStaticCall();
            case TokenType::T_PARENTHESES_OPEN:
                return $this->parseParenOrCast();
            case TokenType::T_LESS:
                return $this->parseTypeHint();
            case TokenType::T_SBRACKET_OPEN:
                return $this->parseArray();
            case TokenType::T_BRACKET_OPEN:
                return $this->parseDynamicBraceLed();
            case TokenType::T_FUNCTION:
                return $this->parseClosure();
            default:
                break;
        }

        $this->unsupported();
    }

    /**
     * Closure expression: function([params]) [use (captures)] { [statements] }.
     * (`fn` is lexed as the same FUNCTION token.) Builds {type:closure,
     * left:params?, right:statements?, use:captures?, ...}.
     */
    private function parseClosure(): array
    {
        $this->expect(TokenType::T_FUNCTION);
        $params = $this->parseParameterParens();

        $use = null;
        if ($this->accept(TokenType::T_USE)) {
            $this->expect(TokenType::T_PARENTHESES_OPEN);
            $use = [$this->parseUseParameter()];
            while ($this->accept(TokenType::T_COMMA)) {
                $use[] = $this->parseUseParameter();
            }
            $this->expect(TokenType::T_PARENTHESES_CLOSE);
        }

        $statements = $this->parseBlock();

        $node = ['type' => 'closure'];
        if ($params !== null) {
            $node['left'] = $params;
        }
        if ($statements !== null) {
            $node['right'] = $statements;
        }
        if ($use !== null) {
            $node['use'] = $use;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /** A closure `use (...)` capture: [const] [&] IDENTIFIER => parameter node. */
    private function parseUseParameter(): array
    {
        $const = $this->accept(TokenType::T_CONST) ? 1 : 0;
        $reference = $this->accept(TokenType::T_BITWISE_AND) ? 1 : 0;
        $name      = $this->expect(TokenType::T_IDENTIFIER);

        return [
            'type'      => 'parameter',
            'name'      => $this->remap((string) $name->value),
            'const'     => $const,
            'data-type' => 'variable',
            'mandatory' => 0,
            'reference' => $reference,
            'file'      => $this->file,
            'line'      => $this->line(),
            'char'      => $this->char(),
        ];
    }

    /** `{ident}(...)` dynamic function call, or `{ident}::...` dynamic-class static call. */
    private function parseDynamicBraceLed(): array
    {
        $this->expect(TokenType::T_BRACKET_OPEN);
        $ident = $this->expect(TokenType::T_IDENTIFIER);
        $this->expect(TokenType::T_BRACKET_CLOSE);

        if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
            return $this->finishFcall($ident, 2);
        }
        if ($this->check(TokenType::T_DOUBLECOLON)) {
            return $this->parseDoubleColon((string) $ident->value, 1);
        }

        $this->unsupported();
    }

    /** IDENTIFIER/CONSTANT atom — may begin an fcall, scall, or static access. */
    private function parseNameLed(): array
    {
        $isConstant = $this->check(TokenType::T_CONSTANT);
        $tok        = $this->advance();

        if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
            return $this->finishFcall($tok);
        }

        if ($this->check(TokenType::T_DOUBLECOLON)) {
            return $this->parseDoubleColon((string) $tok->value, 0);
        }

        return $this->literalFromToken($isConstant ? TokenType::T_CONSTANT : TokenType::T_IDENTIFIER, $tok);
    }

    private function finishFcall(Token $name, int $callType = 1): array
    {
        $parameters = $this->parseCallArguments();

        $node = [
            'type'      => 'fcall',
            'name'      => $this->remap((string) $name->value),
            'call-type' => $callType,
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseStaticCall(): array
    {
        $this->expect(TokenType::T_STATIC);

        return $this->parseDoubleColon('static', 0);
    }

    /**
     * Parse `class :: ...` — either a static call (scall) when followed by
     * `(`, or a static property/constant access otherwise.
     *
     * @param string $className   raw class-name string for scall
     * @param int    $dynamicClass 1 when the class was a `{var}` form
     */
    private function parseDoubleColon(string $className, int $dynamicClass): array
    {
        $this->expect(TokenType::T_DOUBLECOLON);

        // class::{m}() / class::{"m"}()
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            $this->advance();
            if ($this->check(TokenType::T_IDENTIFIER)) {
                $m = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);

                return $this->finishScall($dynamicClass, $className, 1, $m);
            }
            if ($this->check(TokenType::T_STRING)) {
                $s = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);

                return $this->finishScall($dynamicClass, $className, 2, $s);
            }
            $this->syntaxError();
        }

        // class::class / self::class / static::class / parent::class magic constant.
        // `class` is a reserved keyword (T_CLASS), so it never reaches expectNameToken();
        // emit it as a static-constant-access node with the member value "class", which
        // StaticConstantAccess resolves to the fully-qualified class name. See #2527.
        if ($this->check(TokenType::T_CLASS)) {
            $this->advance();
            $classLiteral  = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->syntheticToken($className));
            $memberLiteral = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->syntheticToken('class'));

            return $this->expr('static-constant-access', $classLiteral, $memberLiteral, null);
        }

        $member     = $this->expectNameToken();
        $memberType = $member->opcode;

        if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
            return $this->finishScall($dynamicClass, $className, 0, $member);
        }

        // static-property-access (IDENTIFIER member) / static-constant-access (CONSTANT member)
        $classLiteral  = $this->literalFromToken(TokenType::T_IDENTIFIER, $this->syntheticToken($className));
        $memberLiteral = $this->literalFromToken(TokenType::T_IDENTIFIER, $member);
        $exprType      = $memberType === TokenType::T_CONSTANT ? 'static-constant-access' : 'static-property-access';

        return $this->expr($exprType, $classLiteral, $memberLiteral, null);
    }

    private function finishScall(int $dynamicClass, string $className, int $dynamicMethod, Token $method): array
    {
        $parameters = $this->parseCallArguments();

        $node = [
            'type'          => 'scall',
            'dynamic-class' => $dynamicClass,
            'class'         => $this->remap($className),
            'dynamic'       => $dynamicMethod,
            'name'          => $this->remap((string) $method->value),
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function parseParenOrCast(): array
    {
        // ( type ) expr  => cast
        if (isset(self::TYPE_NAMES[$this->laType(1)]) && $this->laType(2) === TokenType::T_PARENTHESES_CLOSE) {
            $this->advance(); // (
            $typeName = self::TYPE_NAMES[$this->advance()->opcode];
            $this->expect(TokenType::T_PARENTHESES_CLOSE);
            // The cast rule has PARENTHESES_CLOSE precedence (very high), so its
            // operand binds tighter than any binary operator: `(long) v + w`
            // parses as `add(cast(long, v), w)`.
            $operand = $this->parseExpr(235);

            return $this->expr('cast', $typeName, $operand, null);
        }

        $this->expect(TokenType::T_PARENTHESES_OPEN);
        $inner = $this->parseExpr(0);
        $this->expect(TokenType::T_PARENTHESES_CLOSE);

        return $this->expr('list', $inner, null, null);
    }

    private function parseTypeHint(): array
    {
        // < IDENTIFIER > expr
        if ($this->laType(1) === TokenType::T_IDENTIFIER && $this->laType(2) === TokenType::T_GREATER) {
            $this->advance(); // <
            $ident = $this->advance();
            $this->expect(TokenType::T_GREATER);
            $operand = $this->parseExpr(0);

            return $this->expr('type-hint', $this->literalFromToken(TokenType::T_IDENTIFIER, $ident), $operand, null);
        }

        $this->unsupported();
    }

    private function parseArray(): array
    {
        $this->expect(TokenType::T_SBRACKET_OPEN);
        if ($this->accept(TokenType::T_SBRACKET_CLOSE)) {
            return $this->expr('empty-array', null, null, null);
        }

        $items = [$this->parseArrayItem()];
        while ($this->accept(TokenType::T_COMMA)) {
            $items[] = $this->parseArrayItem();
        }
        $this->expect(TokenType::T_SBRACKET_CLOSE);

        return $this->expr('array', $items, null, null);
    }

    private function parseArrayItem(): array
    {
        $first = $this->parseExpr(0);
        if ($this->accept(TokenType::T_COLON)) {
            $value = $this->parseExpr(0);

            return [
                'key'   => $first,
                'value' => $value,
                'file'  => $this->file,
                'line'  => $this->line(),
                'char'  => $this->char(),
            ];
        }

        return [
            'value' => $first,
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];
    }

    private function parseNew(): array
    {
        $this->expect(TokenType::T_NEW);

        if ($this->check(TokenType::T_STATIC)) {
            $this->advance();
            $parameters = null;
            if ($this->accept(TokenType::T_PARENTHESES_OPEN) && !$this->accept(TokenType::T_PARENTHESES_CLOSE)) {
                $parameters = $this->parseCallArgumentsAfterOpen();
            }

            $node = [
                'type'    => 'new',
                'class'   => 'static',
                'dynamic' => 0,
            ];
            if ($parameters !== null) {
                $node['parameters'] = $parameters;
            }
            $node['file'] = $this->file;
            $node['line'] = $this->line();
            $node['char'] = $this->char();

            return $node;
        }

        // new {Class} ...
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            $this->advance();
            $name = $this->expect(TokenType::T_IDENTIFIER);
            $this->expect(TokenType::T_BRACKET_CLOSE);

            return $this->finishNewInstance(1, $name);
        }

        // new array(...) etc. (new-type)
        if (isset(self::TYPE_NAMES[$this->peekType()])) {
            $typeName = self::TYPE_NAMES[$this->advance()->opcode];
            $this->expect(TokenType::T_PARENTHESES_OPEN);
            $parameters = $this->accept(TokenType::T_PARENTHESES_CLOSE)
                ? null
                : $this->parseCallArgumentsAfterOpen();

            $node = [
                'type'          => 'new-type',
                'internal-type' => $typeName,
            ];
            if ($parameters !== null) {
                $node['parameters'] = $parameters;
            }
            $node['file'] = $this->file;
            $node['line'] = $this->line();
            $node['char'] = $this->char();

            return $node;
        }

        $name = $this->expectNameToken();

        return $this->finishNewInstance(0, $name);
    }

    private function finishNewInstance(int $dynamic, Token $name): array
    {
        $parameters = null;
        if ($this->accept(TokenType::T_PARENTHESES_OPEN)) {
            if (!$this->accept(TokenType::T_PARENTHESES_CLOSE)) {
                $parameters = $this->parseCallArgumentsAfterOpen();
            }
        }

        $node = [
            'type'    => 'new',
            'class'   => $this->remap((string) $name->value),
            'dynamic' => $dynamic,
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /** Postfix chain: ->property, ->method(), [index]. */
    private function parsePostfix(array $left): array
    {
        while (true) {
            if ($this->check(TokenType::T_ARROW)) {
                $left = $this->parseArrowPostfix($left);
                continue;
            }
            if ($this->check(TokenType::T_SBRACKET_OPEN)) {
                $this->advance();
                $index = $this->parseExpr(0);
                $this->expect(TokenType::T_SBRACKET_CLOSE);
                $left = $this->expr('array-access', $left, $index, null);
                continue;
            }
            break;
        }

        return $left;
    }

    private function parseArrowPostfix(array $left): array
    {
        $this->expect(TokenType::T_ARROW);

        // ->{IDENT} / ->{"str"}
        if ($this->check(TokenType::T_BRACKET_OPEN)) {
            $this->advance();
            if ($this->check(TokenType::T_IDENTIFIER)) {
                $m = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);
                if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
                    return $this->finishMcall(2, $left, $m);
                }

                return $this->expr('property-dynamic-access', $left, $this->literalFromToken(TokenType::T_IDENTIFIER, $m), null);
            }
            if ($this->check(TokenType::T_STRING)) {
                $s = $this->advance();
                $this->expect(TokenType::T_BRACKET_CLOSE);
                if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
                    return $this->finishMcall(3, $left, $s);
                }

                return $this->expr('property-string-access', $left, $this->literalFromToken(TokenType::T_STRING, $s), null);
            }
            $this->syntaxError();
        }

        $member = $this->expectNameToken();
        if ($this->check(TokenType::T_PARENTHESES_OPEN)) {
            return $this->finishMcall(1, $left, $member);
        }

        return $this->expr('property-access', $left, $this->literalFromToken(TokenType::T_IDENTIFIER, $member), null);
    }

    private function finishMcall(int $callType, array $object, Token $method): array
    {
        $parameters = $this->parseCallArguments();

        $node = [
            'type'      => 'mcall',
            'variable'  => $object,
            'name'      => $this->remap((string) $method->value),
            'call-type' => $callType,
        ];
        if ($parameters !== null) {
            $node['parameters'] = $parameters;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    /** @return list<array>|null call parameters, null when `()` is empty */
    private function parseCallArguments(): ?array
    {
        $this->expect(TokenType::T_PARENTHESES_OPEN);
        if ($this->accept(TokenType::T_PARENTHESES_CLOSE)) {
            return null;
        }

        return $this->parseCallArgumentsAfterOpen();
    }

    /** @return list<array> after the opening `(` was consumed and is non-empty */
    private function parseCallArgumentsAfterOpen(): array
    {
        $params = [$this->parseCallArgument()];
        while ($this->accept(TokenType::T_COMMA)) {
            $params[] = $this->parseCallArgument();
        }
        $this->expect(TokenType::T_PARENTHESES_CLOSE);

        return $params;
    }

    private function parseCallArgument(): array
    {
        $name = null;
        if ($this->check(TokenType::T_IDENTIFIER) && $this->laType(1) === TokenType::T_COLON) {
            $name = $this->advance();
            $this->expect(TokenType::T_COLON);
        }

        $expr = $this->parseExpr(0);

        $node = [];
        if ($name !== null) {
            $node['name'] = $this->remap((string) $name->value);
        }
        $node['parameter'] = $expr;
        $node['file']      = $this->file;
        $node['line']      = $this->line();
        $node['char']      = $this->char();

        return $node;
    }

    /* ----- literal-expr (property/const/parameter defaults) ------------- */

    private function parseLiteralExpr(): array
    {
        $type = $this->peekType();

        switch ($type) {
            case TokenType::T_INTEGER:
                return $this->literalFromToken(TokenType::T_INTEGER, $this->advance());
            case TokenType::T_CHAR:
                return $this->literalFromToken(TokenType::T_CHAR, $this->advance());
            case TokenType::T_STRING:
                return $this->literalFromToken(TokenType::T_STRING, $this->advance());
            case TokenType::T_DOUBLE:
                return $this->literalFromToken(TokenType::T_DOUBLE, $this->advance());
            case TokenType::T_NULL:
                $this->advance();

                return $this->literalNoValue('null');
            case TokenType::T_FALSE:
                $this->advance();

                return $this->boolLiteral('false');
            case TokenType::T_TRUE:
                $this->advance();

                return $this->boolLiteral('true');
            case TokenType::T_SBRACKET_OPEN:
                return $this->parseLiteralArray();
            case TokenType::T_IDENTIFIER:
            case TokenType::T_CONSTANT:
                $tok = $this->advance();
                if ($this->check(TokenType::T_DOUBLECOLON)) {
                    $this->advance();
                    $member = $this->expect(TokenType::T_CONSTANT);

                    return $this->expr(
                        'static-constant-access',
                        $this->literalFromToken(TokenType::T_IDENTIFIER, $tok),
                        $this->literalFromToken(TokenType::T_IDENTIFIER, $member),
                        null
                    );
                }
                if ($type === TokenType::T_CONSTANT) {
                    return $this->literalFromToken(TokenType::T_CONSTANT, $tok);
                }
                $this->syntaxError();

                // no break (syntaxError throws)
            default:
                break;
        }

        $this->syntaxError();
    }

    private function parseLiteralArray(): array
    {
        $this->expect(TokenType::T_SBRACKET_OPEN);
        if ($this->accept(TokenType::T_SBRACKET_CLOSE)) {
            return $this->expr('empty-array', null, null, null);
        }

        $items = [$this->parseLiteralArrayItem()];
        while ($this->accept(TokenType::T_COMMA)) {
            $items[] = $this->parseLiteralArrayItem();
        }
        $this->expect(TokenType::T_SBRACKET_CLOSE);

        return $this->expr('array', $items, null, null);
    }

    private function parseLiteralArrayItem(): array
    {
        // literal_array_key : IDENTIFIER | STRING | INTEGER ; followed by ':'
        $type = $this->peekType();
        if (
            ($type === TokenType::T_IDENTIFIER || $type === TokenType::T_STRING || $type === TokenType::T_INTEGER)
            && $this->laType(1) === TokenType::T_COLON
        ) {
            $keyTok  = $this->advance();
            $keyType = $type === TokenType::T_IDENTIFIER ? TokenType::T_IDENTIFIER : $type;
            $key     = $this->literalFromToken($keyType, $keyTok);
            $this->expect(TokenType::T_COLON);
            $value = $this->parseLiteralExpr();

            return [
                'key'   => $key,
                'value' => $value,
                'file'  => $this->file,
                'line'  => $this->line(),
                'char'  => $this->char(),
            ];
        }

        $value = $this->parseLiteralExpr();

        return [
            'value' => $value,
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];
    }

    /* ===================================================================== *
     *  Node builders
     * ===================================================================== */

    /**
     * Build an expr node (xx_ret_expr): type, left?, right?, extra?, file, line, char.
     * left/right/extra are included only when non-null.
     *
     * @param mixed $left
     * @param mixed $right
     * @param mixed $extra
     */
    private function expr(string $type, $left, $right, $extra): array
    {
        $node = ['type' => $type];
        if ($left !== null) {
            $node['left'] = $left;
        }
        if ($right !== null) {
            $node['right'] = $right;
        }
        if ($extra !== null) {
            $node['extra'] = $extra;
        }
        $node['file'] = $this->file;
        $node['line'] = $this->line();
        $node['char'] = $this->char();

        return $node;
    }

    private function literalFromToken(int $litType, Token $tok): array
    {
        $node         = ['type' => $this->literalTypeName($litType)];
        $node['value'] = $this->remap((string) $tok->value);
        $node['file']  = $this->file;
        $node['line']  = $this->line();
        $node['char']  = $this->char();

        return $node;
    }

    private function literalNoValue(string $typeName): array
    {
        return [
            'type' => $typeName,
            'file' => $this->file,
            'line' => $this->line(),
            'char' => $this->char(),
        ];
    }

    private function boolLiteral(string $value): array
    {
        return [
            'type'  => 'bool',
            'value' => $value,
            'file'  => $this->file,
            'line'  => $this->line(),
            'char'  => $this->char(),
        ];
    }

    private function literalTypeName(int $litType): string
    {
        switch ($litType) {
            case TokenType::T_CONSTANT:
                return 'constant';
            case TokenType::T_IDENTIFIER:
                return 'variable';
            case TokenType::T_INTEGER:
                return 'int';
            case TokenType::T_DOUBLE:
                return 'double';
            case TokenType::T_STRING:
                return 'string';
            case TokenType::T_ISTRING:
                return 'istring';
            case TokenType::T_CHAR:
                return 'char';
            default:
                return 'null';
        }
    }

    /** A token carrying $value, for synthesizing literal nodes (e.g. scall class names). */
    private function syntheticToken(string $value): Token
    {
        return new Token(TokenType::T_IDENTIFIER, $value, $this->line(), $this->char());
    }

    private function expectNameToken(): Token
    {
        if ($this->check(TokenType::T_IDENTIFIER) || $this->check(TokenType::T_CONSTANT)) {
            return $this->advance();
        }
        $this->syntaxError();
    }
}
