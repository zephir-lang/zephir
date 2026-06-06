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

use function preg_match;
use function strlen;
use function strspn;
use function strtolower;
use function substr;

/**
 * Hand-written port of the C extension's re2c lexer (`parser/scanner.re`) and
 * the token-feed loop of `parser/base.c`.
 *
 * Faithfulness goals (verified against golden IR dumps):
 *  - `activeLine`/`activeChar` arithmetic is reproduced *exactly*, including the
 *    intentionally non-uniform per-rule increments and the off-by-one between
 *    the first line (`activeChar` starts at 1) and later lines (reset to 0 on
 *    newline). This is what makes node positions byte-identical.
 *  - Each emitted {@see Token} records `line`/`char` as the scanner state
 *    *immediately after* that token is consumed — i.e. the value the parser
 *    stamps when the token is the current lookahead.
 *  - IGNORE tokens (whitespace, newlines, line comments, C comments) advance
 *    position but are not surfaced. `/** ... *​/` docblocks surface as COMMENT,
 *    dropped when nested deeper than one `{` level (`number_brackets > 1`).
 */
final class Lexer
{
    /** Keyword/type lexemes (lower-cased) => token opcode. Matched case-insensitively, as re2c single-quoted literals are. */
    private const KEYWORDS = [
        'null'         => TokenType::T_NULL,
        'false'        => TokenType::T_FALSE,
        'true'         => TokenType::T_TRUE,
        'namespace'    => TokenType::T_NAMESPACE,
        'use'          => TokenType::T_USE,
        'as'           => TokenType::T_AS,
        'interface'    => TokenType::T_INTERFACE,
        'class'        => TokenType::T_CLASS,
        'extends'      => TokenType::T_EXTENDS,
        'implements'   => TokenType::T_IMPLEMENTS,
        'internal'     => TokenType::T_INTERNAL,
        'public'       => TokenType::T_PUBLIC,
        'protected'    => TokenType::T_PROTECTED,
        'private'      => TokenType::T_PRIVATE,
        'static'       => TokenType::T_STATIC,
        'inline'       => TokenType::T_INLINE,
        'deprecated'   => TokenType::T_DEPRECATED,
        'final'        => TokenType::T_FINAL,
        'abstract'     => TokenType::T_ABSTRACT,
        'function'     => TokenType::T_FUNCTION,
        'fn'           => TokenType::T_FUNCTION,
        'let'          => TokenType::T_LET,
        'echo'         => TokenType::T_ECHO,
        'const'        => TokenType::T_CONST,
        'int'          => TokenType::T_TYPE_INTEGER,
        'uint'         => TokenType::T_TYPE_UINTEGER,
        'long'         => TokenType::T_TYPE_LONG,
        'ulong'        => TokenType::T_TYPE_ULONG,
        'char'         => TokenType::T_TYPE_CHAR,
        'uchar'        => TokenType::T_TYPE_UCHAR,
        'double'       => TokenType::T_TYPE_DOUBLE,
        'float'        => TokenType::T_TYPE_DOUBLE,
        'bool'         => TokenType::T_TYPE_BOOL,
        'boolean'      => TokenType::T_TYPE_BOOL,
        'string'       => TokenType::T_TYPE_STRING,
        'array'        => TokenType::T_TYPE_ARRAY,
        'var'          => TokenType::T_TYPE_VAR,
        'object'       => TokenType::T_TYPE_OBJECT,
        'callable'     => TokenType::T_TYPE_CALLABLE,
        'resource'     => TokenType::T_TYPE_RESOURCE,
        'mixed'        => TokenType::T_TYPE_MIXED,
        'if'           => TokenType::T_IF,
        'else'         => TokenType::T_ELSE,
        'elseif'       => TokenType::T_ELSEIF,
        'do'           => TokenType::T_DO,
        'while'        => TokenType::T_WHILE,
        'for'          => TokenType::T_FOR,
        'in'           => TokenType::T_IN,
        'new'          => TokenType::T_NEW,
        'return'       => TokenType::T_RETURN,
        'require_once' => TokenType::T_REQUIRE_ONCE,
        'yield'        => TokenType::T_YIELD,
        'require'      => TokenType::T_REQUIRE,
        'clone'        => TokenType::T_CLONE,
        'empty'        => TokenType::T_EMPTY,
        'void'         => TokenType::T_VOID,
        'loop'         => TokenType::T_LOOP,
        'break'        => TokenType::T_BREAK,
        'continue'     => TokenType::T_CONTINUE,
        'typeof'       => TokenType::T_TYPEOF,
        'instanceof'   => TokenType::T_INSTANCEOF,
        'likely'       => TokenType::T_LIKELY,
        'unlikely'     => TokenType::T_UNLIKELY,
        'isset'        => TokenType::T_ISSET,
        'unset'        => TokenType::T_UNSET,
        'throw'        => TokenType::T_THROW,
        'fetch'        => TokenType::T_FETCH,
        'switch'       => TokenType::T_SWITCH,
        'case'         => TokenType::T_CASE,
        'default'      => TokenType::T_DEFAULT,
        'reverse'      => TokenType::T_REVERSE,
        'try'          => TokenType::T_TRY,
        'catch'        => TokenType::T_CATCH,
    ];

    /** PHP superglobals that are forced to IDENTIFIER even though they are all-caps. */
    private const SUPERGLOBALS = [
        '_GET'     => true,
        '_POST'    => true,
        '_REQUEST' => true,
        '_COOKIE'  => true,
        '_SERVER'  => true,
        '_SESSION' => true,
        '_FILES'   => true,
        '_ENV'     => true,
    ];

    private string $code;
    private int $len;
    private int $cursor = 0;

    private int $activeLine = 1;
    private int $activeChar = 1;
    private int $classLine  = 0;
    private int $classChar  = 0;
    private int $methodLine = 0;
    private int $methodChar = 0;

    /** Opcode of the last significant (non-IGNORE) token; drives negative-literal disambiguation. */
    private int $activeToken = 0;

    /** Set by getToken(): opcode and value of the token just scanned. */
    private int $tOpcode = 0;
    private ?string $tValue = null;

    private bool $scannerError = false;
    private string $scannerErrorMessage = '';

    public function __construct(string $code)
    {
        $this->code = $code;
        $this->len  = strlen($code);
    }

    /**
     * Tokenize the whole input into the significant-token stream the parser
     * consumes, replicating base.c's feed loop:
     *  - IGNORE tokens advance position but are not surfaced;
     *  - `activeToken` tracks the last non-IGNORE opcode (set before any drop);
     *  - `{`/`}` adjust the bracket depth;
     *  - COMMENT tokens nested deeper than one `{` level are dropped.
     *
     * The stream always ends with an EOF sentinel (opcode 0) carrying the final
     * scanner position, so the parser has a lookahead at end of input.
     *
     * @return list<Token>
     */
    public function tokenize(): array
    {
        $tokens         = [];
        $numberBrackets = 0;

        while (true) {
            $status = $this->getToken();
            if ($status < 0) {
                if ($status === TokenType::RETCODE_ERR || $status === TokenType::RETCODE_IMPOSSIBLE) {
                    $this->scannerError = true;
                    $rest               = substr($this->code, $this->cursor);
                    $this->scannerErrorMessage = $rest !== ''
                        ? 'Scanner error: ' . $status . ' ' . $rest
                        : 'Scanner error: ' . $status;
                }
                break; // EOF or error
            }

            // base.c keeps the last *significant* token (incl. COMMENT) even if it is dropped below.
            if ($this->tOpcode !== TokenType::T_IGNORE) {
                $this->activeToken = $this->tOpcode;
            }

            switch ($this->tOpcode) {
                case TokenType::T_IGNORE:
                    break;
                case TokenType::T_BRACKET_OPEN:
                    $numberBrackets++;
                    $tokens[] = $this->makeToken();
                    break;
                case TokenType::T_BRACKET_CLOSE:
                    $numberBrackets--;
                    $tokens[] = $this->makeToken();
                    break;
                case TokenType::T_COMMENT:
                    if ($numberBrackets <= 1) {
                        $tokens[] = $this->makeToken();
                    }
                    break;
                default:
                    $tokens[] = $this->makeToken();
            }
        }

        // EOF sentinel carrying the final scanner position and class/method snapshots.
        $tokens[] = new Token(
            0,
            null,
            $this->activeLine,
            $this->activeChar,
            $this->classLine,
            $this->classChar,
            $this->methodLine,
            $this->methodChar
        );

        return $tokens;
    }

    private function makeToken(): Token
    {
        return new Token(
            $this->tOpcode,
            $this->tValue,
            $this->activeLine,
            $this->activeChar,
            $this->classLine,
            $this->classChar,
            $this->methodLine,
            $this->methodChar
        );
    }

    public function hasScannerError(): bool
    {
        return $this->scannerError;
    }

    public function getScannerErrorMessage(): string
    {
        return $this->scannerErrorMessage;
    }

    public function getActiveLine(): int
    {
        return $this->activeLine;
    }

    public function getActiveChar(): int
    {
        return $this->activeChar;
    }

    /**
     * Scan a single raw token (including IGNORE). Mirrors xx_get_token().
     * Sets $this->tOpcode and $this->tValue; returns 0 on success, or a
     * negative RETCODE_* on EOF/error.
     */
    private function getToken(): int
    {
        $this->tValue = null;
        $start        = $this->cursor;

        if ($start >= $this->len) {
            return TokenType::RETCODE_EOF;
        }

        $c = $this->code[$start];

        // Newline.
        if ($c === "\n") {
            $this->cursor++;
            $this->activeLine++;
            $this->activeChar = 0;
            $this->tOpcode    = TokenType::T_IGNORE;

            return 0;
        }

        // Whitespace run [ \t\r]+.
        if ($c === ' ' || $c === "\t" || $c === "\r") {
            $n = strspn($this->code, " \t\r", $start);
            $this->cursor += $n;
            $this->activeChar += $n;
            $this->tOpcode = TokenType::T_IGNORE;

            return 0;
        }

        // Numeric literals (possibly leading '-').
        if (($c >= '0' && $c <= '9') || $c === '-') {
            $status = $this->tryNumber($start);
            if ($status !== null) {
                return $status;
            }
            // '-' that does not begin a number falls through to operator handling.
        }

        // Identifiers / keywords / constants.
        if ($c === '\\' || $c === '_' || $c === '$' || ($c >= 'a' && $c <= 'z') || ($c >= 'A' && $c <= 'Z')) {
            $status = $this->tryIdentifier($start);
            if ($status !== null) {
                return $status;
            }
        }

        // String / char / interned-string literals.
        if ($c === '"') {
            return $this->scanQuoted($start, '"', TokenType::T_STRING);
        }
        if ($c === "'") {
            return $this->scanQuoted($start, "'", TokenType::T_CHAR);
        }
        if ($c === '~' && $start + 1 < $this->len && $this->code[$start + 1] === '"') {
            return $this->scanQuoted($start, '"', TokenType::T_ISTRING);
        }

        // Comments and c-blocks (need 2-char lookahead).
        if ($c === '/') {
            $n = $this->code[$start + 1] ?? '';
            if ($n === '*') {
                return $this->scanBlockComment($start);
            }
            if ($n === '/') {
                $this->scanLineComment($start);

                return 0;
            }
        }
        if ($c === '%' && ($this->code[$start + 1] ?? '') === '{') {
            return $this->scanCblock($start);
        }

        return $this->scanOperator($start, $c);
    }

    /**
     * Attempt to scan a numeric literal at $start. Returns 0 on success
     * (setting opcode/value), or null when no numeric literal matches (so the
     * caller can treat a leading '-' as an operator).
     */
    private function tryNumber(int $start): ?int
    {
        $code = $this->code;

        // Longest-match: hex, then double, then decimal integer (mirrors re2c).
        if (preg_match('/\G-?0x[0-9A-Fa-f]+/A', $code, $m, 0, $start)) {
            $opcode = TokenType::T_INTEGER;
        } elseif (preg_match('/\G-?\d+\.\d+/A', $code, $m, 0, $start)) {
            $opcode = TokenType::T_DOUBLE;
        } elseif (preg_match('/\G-?\d+/A', $code, $m, 0, $start)) {
            $opcode = TokenType::T_INTEGER;
        } else {
            return null;
        }

        $text = $m[0];

        // A '-' glued to a digit right after a value is binary subtraction, not a sign.
        if ($text[0] === '-' && $this->tokenEndsValue($this->activeToken)) {
            $this->cursor = $start + 1;
            $this->activeChar++;
            $this->tOpcode = TokenType::T_SUB;

            return 0;
        }

        $this->cursor = $start + strlen($text);
        $this->activeChar += strlen($text);
        $this->tOpcode = $opcode;
        $this->tValue  = $text;

        return 0;
    }

    /**
     * Attempt to scan an identifier/keyword/constant at $start. Returns 0 on
     * success or null when the identifier regex does not match (e.g. a bare '$'
     * not followed by an identifier char), letting the caller fall through.
     */
    private function tryIdentifier(int $start): ?int
    {
        if (!preg_match('/\G[\\\\_$]?[_a-zA-Z\\\\][a-zA-Z0-9_\\\\]*/A', $this->code, $m, 0, $start)) {
            return null;
        }

        $lexeme = $m[0];
        $kw     = self::KEYWORDS[strtolower($lexeme)] ?? null;
        if ($kw !== null) {
            $this->cursor = $start + strlen($lexeme);
            $this->activeChar += strlen($lexeme);
            $this->tOpcode = $kw;

            if ($kw === TokenType::T_CLASS) {
                $this->classLine = $this->activeLine;
                $this->classChar = $this->activeChar;
            } elseif ($kw === TokenType::T_FUNCTION) {
                $this->methodLine = $this->activeLine;
                $this->methodChar = $this->activeChar;
            }

            return 0;
        }

        // Not a keyword: classify as identifier or constant.
        if ($lexeme[0] === '$') {
            $value = substr($lexeme, 1);
            $this->activeChar += strlen($lexeme) - 1;
        } else {
            $value = $lexeme;
            $this->activeChar += strlen($lexeme);
        }
        $this->cursor = $start + strlen($lexeme);
        $this->tValue = $value;

        $vlen = strlen($value);
        if ($vlen > 3 && $value[0] === '_' && isset(self::SUPERGLOBALS[$value])) {
            $this->tOpcode = TokenType::T_IDENTIFIER;

            return 0;
        }

        if (strspn($value, '_') === $vlen) {
            $this->tOpcode = TokenType::T_IDENTIFIER;

            return 0;
        }

        $isConstant = true;
        for ($j = 0; $j < $vlen; $j++) {
            $ch = $value[$j];
            if (!(($ch >= 'A' && $ch <= 'Z') || ($ch >= '0' && $ch <= '9') || $ch === '_')) {
                $isConstant = false;
                break;
            }
        }
        $this->tOpcode = $isConstant ? TokenType::T_CONSTANT : TokenType::T_IDENTIFIER;

        return 0;
    }

    /**
     * Scan a quoted literal (string `"..."`, char `'...'`, or istring `~"..."`).
     * Escapes are NOT decoded — the raw inner text is stored, as in scanner.re.
     */
    private function scanQuoted(int $start, string $quote, int $opcode): int
    {
        $inner = ($opcode === TokenType::T_ISTRING) ? $start + 2 : $start + 1;
        $i     = $inner;

        while ($i < $this->len) {
            $ch = $this->code[$i];
            if ($ch === '\\') {
                $i += 2; // backslash escape: consume two bytes
                continue;
            }
            if ($ch === $quote) {
                break;
            }
            $i++;
        }

        if ($i >= $this->len) {
            // Unterminated literal: re2c falls back to the catch-all error rule.
            return TokenType::RETCODE_ERR;
        }

        $innerLen     = $i - $inner;
        $this->tValue = substr($this->code, $inner, $innerLen);
        $this->cursor = $i + 1; // past the closing quote
        $this->tOpcode = $opcode;

        // active_char increments differ per literal kind (faithful to scanner.re):
        //   STRING  += innerLen          (omits both quotes)
        //   CHAR    += innerLen + 1      (omits opening quote only)
        //   ISTRING += innerLen + 1      (omits the ~ and opening quote)
        if ($opcode === TokenType::T_STRING) {
            $this->activeChar += $innerLen;
        } else {
            $this->activeChar += $innerLen + 1;
        }

        return 0;
    }

    /**
     * Scan a `/* ... *​/` block comment. Docblocks become COMMENT (value kept),
     * plain C comments become IGNORE. Multi-line comments advance line/char
     * character-by-character exactly as scanner.re does.
     */
    private function scanBlockComment(int $start): int
    {
        // Find the closing "*/".
        $i      = $start + 2;
        $closed = false;
        while ($i < $this->len) {
            if ($this->code[$i] === '*' && ($this->code[$i + 1] ?? '') === '/') {
                $i += 2;
                $closed = true;
                break;
            }
            $i++;
        }
        if (!$closed) {
            return TokenType::RETCODE_ERR;
        }

        $this->cursor = $i;
        $l            = $i - $start; // total length incl. /* and */

        // Decide docblock (COMMENT) vs C comment (IGNORE).
        if ($l === 5) {
            $opcode = TokenType::T_COMMENT;          // /***/
        } elseif ($l === 4) {
            $opcode = TokenType::T_IGNORE;           // /**/
        } elseif ($this->code[$start + 2] === '*' && $this->code[$start + $l - 2] === '*') {
            $opcode = TokenType::T_COMMENT;          // /** ... **/
        } else {
            $opcode = TokenType::T_IGNORE;           // /* ... */
        }

        $hasData    = false;
        $valueStart = $start;
        if ($opcode === TokenType::T_COMMENT && $l > 5) {
            $hasData    = true;
            $valueStart = $start + 1;
        }
        if ($opcode === TokenType::T_IGNORE && $l > 4) {
            $hasData = true;
        }

        if ($hasData) {
            $valLen       = ($i - $valueStart) - 1;
            $value        = substr($this->code, $valueStart, $valLen);
            $this->tValue = $value;

            // Walk value[0 .. valLen-2] updating line/char (scanner.re's loop).
            $ch = $this->activeChar;
            for ($k = 0; $k < $valLen - 1; $k++) {
                if ($value[$k] === "\n") {
                    $ch = 1;
                    $this->activeLine++;
                } else {
                    $ch++;
                }
            }
            $this->activeChar = $ch;
        } elseif ($opcode === TokenType::T_COMMENT) {
            $valueStart   = $start + 1;
            $valLen       = ($i - $valueStart) - 1;
            $this->tValue = substr($this->code, $valueStart, $valLen);
        }

        if ($opcode === TokenType::T_IGNORE) {
            $this->tValue = null; // C comments carry no value
        }

        $this->tOpcode = $opcode;

        return 0;
    }

    /**
     * Scan a `// ...` line comment (to end of line, excluding the newline).
     */
    private function scanLineComment(int $start): void
    {
        $i = $start + 2;
        while ($i < $this->len && $this->code[$i] !== "\n" && $this->code[$i] !== "\r") {
            $i++;
        }
        $this->activeChar += $i - $start;
        $this->cursor      = $i;
        $this->tOpcode     = TokenType::T_IGNORE;
    }

    /**
     * Scan a `%{ ... }%` c-block; the inner text is kept verbatim.
     */
    private function scanCblock(int $start): int
    {
        $i      = $start + 2;
        $closed = false;
        while ($i < $this->len) {
            if ($this->code[$i] === '}' && ($this->code[$i + 1] ?? '') === '%') {
                $i += 2;
                $closed = true;
                break;
            }
            $i++;
        }
        if (!$closed) {
            return TokenType::RETCODE_ERR;
        }

        $this->cursor = $i;
        $valueStart   = $start + 2;
        $valLen       = ($i - $valueStart) - 2; // drop trailing }%
        $value        = substr($this->code, $valueStart, $valLen);
        $this->tValue = $value;

        $ch = $this->activeChar;
        for ($k = 0; $k < $valLen - 1; $k++) {
            if ($value[$k] === "\n") {
                $ch = 1;
                $this->activeLine++;
            } else {
                $ch++;
            }
        }
        $this->activeChar = $ch;
        $this->tOpcode    = TokenType::T_CBLOCK;

        return 0;
    }

    /**
     * Scan an operator/punctuator at $start. The increments here intentionally
     * reproduce scanner.re verbatim, including its non-uniform (sometimes
     * off-by-one) `active_char` bumps on multi-character operators.
     */
    private function scanOperator(int $start, string $c): int
    {
        $n1 = $this->code[$start + 1] ?? '';
        $n2 = $this->code[$start + 2] ?? '';

        switch ($c) {
            case '(':
                return $this->op(1, TokenType::T_PARENTHESES_OPEN);
            case ')':
                return $this->op(1, TokenType::T_PARENTHESES_CLOSE);
            case '{':
                return $this->op(1, TokenType::T_BRACKET_OPEN);
            case '}':
                return $this->op(1, TokenType::T_BRACKET_CLOSE);
            case '[':
                return $this->op(1, TokenType::T_SBRACKET_OPEN);
            case ']':
                return $this->op(1, TokenType::T_SBRACKET_CLOSE);
            case '@':
                return $this->op(1, TokenType::T_AT);
            case '~':
                return $this->op(1, TokenType::T_BITWISE_NOT);
            case ';':
                return $this->op(1, TokenType::T_DOTCOMMA);
            case ',':
                return $this->op(1, TokenType::T_COMMA);
            case '?':
                return $this->op(1, TokenType::T_QUESTION);

            case '!':
                if ($n1 === '=' && $n2 === '=') {
                    return $this->op(3, TokenType::T_NOTIDENTICAL, 3);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_NOTEQUALS, 2);
                }
                return $this->op(1, TokenType::T_NOT);

            case '&':
                if ($n1 === '&') {
                    return $this->op(2, TokenType::T_AND, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_BITWISE_AND, 1);
                }
                return $this->op(1, TokenType::T_BITWISE_AND);

            case '|':
                if ($n1 === '|') {
                    return $this->op(2, TokenType::T_OR, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_BITWISE_OR, 1);
                }
                return $this->op(1, TokenType::T_BITWISE_OR);

            case '^':
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_BITWISE_XOR, 1);
                }
                return $this->op(1, TokenType::T_BITWISE_XOR);

            case '<':
                if ($n1 === '<' && $n2 === '=') {
                    return $this->op(3, TokenType::T_ASSIGN_BITWISE_SHIFTLEFT, 1);
                }
                if ($n1 === '<') {
                    return $this->op(2, TokenType::T_BITWISE_SHIFTLEFT, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_LESSEQUAL, 1);
                }
                if ($n1 === '>') {
                    return $this->op(2, TokenType::T_NOTEQUALS, 2);
                }
                return $this->op(1, TokenType::T_LESS);

            case '>':
                if ($n1 === '>' && $n2 === '=') {
                    return $this->op(3, TokenType::T_ASSIGN_BITWISE_SHIFTRIGHT, 1);
                }
                if ($n1 === '>') {
                    return $this->op(2, TokenType::T_BITWISE_SHIFTRIGHT, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_GREATEREQUAL, 1);
                }
                return $this->op(1, TokenType::T_GREATER);

            case '=':
                if ($n1 === '=' && $n2 === '=') {
                    return $this->op(3, TokenType::T_IDENTICAL, 3);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_EQUALS, 2);
                }
                if ($n1 === '>') {
                    return $this->op(2, TokenType::T_DOUBLEARROW, 2);
                }
                return $this->op(1, TokenType::T_ASSIGN);

            case '+':
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_ADD, 1);
                }
                if ($n1 === '+') {
                    return $this->op(2, TokenType::T_INCR, 2);
                }
                return $this->op(1, TokenType::T_ADD);

            case '-':
                if ($n1 === '>') {
                    return $this->op(2, TokenType::T_ARROW, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_SUB, 1);
                }
                if ($n1 === '-') {
                    return $this->op(2, TokenType::T_DECR, 2);
                }
                return $this->op(1, TokenType::T_SUB);

            case '*':
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_MUL, 1);
                }
                return $this->op(1, TokenType::T_MUL);

            case '/':
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_DIV, 1);
                }
                return $this->op(1, TokenType::T_DIV);

            case '%':
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_MOD, 1);
                }
                return $this->op(1, TokenType::T_MOD);

            case '.':
                if ($n1 === '.' && $n2 === '.') {
                    return $this->op(3, TokenType::T_EXCLUSIVE_RANGE, 3);
                }
                if ($n1 === '.') {
                    return $this->op(2, TokenType::T_INCLUSIVE_RANGE, 2);
                }
                if ($n1 === '=') {
                    return $this->op(2, TokenType::T_ASSIGN_CONCAT, 1);
                }
                return $this->op(1, TokenType::T_DOT);

            case ':':
                if ($n1 === ':') {
                    return $this->op(2, TokenType::T_DOUBLECOLON, 2);
                }
                return $this->op(1, TokenType::T_COLON);
        }

        return TokenType::RETCODE_ERR;
    }

    /**
     * Emit an operator token consuming $consume bytes and bumping active_char
     * by $charInc (defaults to 1, matching the single-character rules).
     */
    private function op(int $consume, int $opcode, int $charInc = 1): int
    {
        $this->cursor += $consume;
        $this->activeChar += $charInc;
        $this->tOpcode = $opcode;

        return 0;
    }

    /**
     * Whether the given (previous, significant) token ends a value, in which
     * case a following '-' glued to a digit is binary subtraction rather than
     * the sign of a negative literal. Mirrors xx_token_ends_value().
     */
    private function tokenEndsValue(int $opcode): bool
    {
        switch ($opcode) {
            case TokenType::T_INTEGER:
            case TokenType::T_DOUBLE:
            case TokenType::T_STRING:
            case TokenType::T_ISTRING:
            case TokenType::T_CHAR:
            case TokenType::T_IDENTIFIER:
            case TokenType::T_CONSTANT:
            case TokenType::T_TRUE:
            case TokenType::T_FALSE:
            case TokenType::T_NULL:
            case TokenType::T_PARENTHESES_CLOSE:
            case TokenType::T_SBRACKET_CLOSE:
                return true;
            default:
                return false;
        }
    }
}
