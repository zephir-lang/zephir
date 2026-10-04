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

namespace Zephir;

use Psr\Log\LoggerInterface;
use Zephir\Backend\Backend;
use Zephir\Cache\FunctionCache;
use Zephir\Cache\Manager;
use Zephir\Class\Definition\AbstractDefinition;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Code\Printer;
use Zephir\Exception\CompilerException;
use Zephir\Passes\StaticTypeInference;

use function array_key_last;
use function array_pop;
use function in_array;
use function sprintf;

/**
 * This class encapsulates important entities required during compilation
 */
class CompilationContext
{
    /**
     * Manages class renaming using keyword 'use'.
     */
    public ?AliasManager $aliasManager = null;
    /**
     * The current backend.
     */
    public ?Backend $backend = null;
    /**
     * Helps to create graphs of conditional/jump branches in a specific method.
     */
    public ?BranchManager $branchManager = null;
    /**
     * Manages both function and method call caches.
     */
    public ?Manager $cacheManager = null;
    /**
     * Represents the class currently being compiled.
     */
    public ?Definition $classDefinition = null;
    /**
     * Current code printer.
     */
    public ?Printer  $codePrinter = null;
    public ?Compiler $compiler    = null;
    /**
     * Global config.
     */
    public ?Config $config = null;
    /**
     * The current branch, variables declared in conditional branches
     * must be market if they're used out of those branches.
     */
    public int $currentBranch = 0;
    /**
     * Current method or function that being compiled.
     */
    public ?Method $currentMethod = null;
    /**
     * Label id of the innermost `try` enclosing the current position, whose
     * `try_end_N` a pending exception jumps to. Restored when a `try` ends.
     */
    public int $currentTryCatch = 0;
    /**
     * Counter handing out unique `try` label ids within one function.
     *
     * Reset on entry to each method by Method::compile().
     */
    public int $tryCatchLabelId = 0;
    /**
     * Current cycle/loop block.
     */
    public array $cycleBlocks = [];
    /**
     * Function Cache.
     */
    public ?FunctionCache $functionCache = null;
    /**
     * Code printer for the header.
     */
    public ?Printer $headerPrinter = null;
    /**
     * Represents the c-headers added to the file.
     */
    public ?HeadersManager $headersManager = null;
    /**
     * Tells if the compilation is being made inside a cycle/loop.
     */
    public int $insideCycle = 0;
    /**
     * Stack of `switch` statements being compiled, innermost last.
     *
     * A `switch` is lowered to labels and jumps rather than to a C construct,
     * so a `break`/`continue` that targets it must `goto` its end label instead
     * of emitting a C `break`/`continue`. Each entry records that label, the
     * `insideCycle` depth at the point the `switch` was entered (so a loop
     * opened inside a clause is recognised as the innermost target), and
     * whether the end label was actually jumped to.
     *
     * @var list<array{label: string, cycleDepth: int, used: bool}>
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    public array $switchTargets = [];
    /**
     * Counter handing out unique `switch` label ids within one function.
     *
     * Reset on entry to each method by Method::compile().
     */
    public int $switchLabelId = 0;
    /**
     * Stack of do-while loops being compiled, innermost last.
     *
     * A do-while condition is evaluated by code printed at the end of the
     * body, so a `continue` must jump to a label in front of that code rather
     * than emit a C `continue`, which goes straight to the test. Entries have
     * the same shape as `$switchTargets`.
     *
     * @var list<array{label: string, cycleDepth: int, used: bool}>
     */
    public array $doWhileTargets = [];
    /**
     * Counter handing out unique do-while label ids within one function.
     *
     * Reset on entry to each method by Method::compile().
     */
    public int $doWhileLabelId = 0;
    /**
     * Tells if the compilation is being made inside a try/catch block.
     */
    public int $insideTryCatch = 0;
    /**
     * Per-method cache for `isset(obj->staticProp)` interned zend_string *
     * slots. Maps property-name → emitted C identifier so repeated isset()
     * of the same property within a method reuses one slot.
     * Reset on entry to each method by Method::compile().
     * See https://github.com/zephir-lang/zephir/issues/2385.
     *
     * @var array<string, string>
     */
    public array $issetPropertyCache = [];
    /**
     * Counter feeding unique identifiers for issetPropertyCache slots.
     */
    public int $issetPropertyCacheCounter = 0;
    /**
     * Per-method map of property name => method-scope interned zend_string
     * slot variable, for cached object-property read/write. The slot decls
     * and lazy init are emitted at function scope by Method::compile() (not
     * inline) so a reference is valid from any nested block. Reset on entry
     * to each method. See property-access optimization (issue #1884 follow-up).
     *
     * @var array<string, string>
     */
    public array $propertyNameCache = [];
    /**
     * Counter feeding unique identifiers for propertyNameCache slots.
     */
    public int $propertyNameCacheCounter = 0;
    /**
     * Global logger.
     */
    public ?LoggerInterface $logger = null;
    /**
     * Whether the current method is static or not.
     */
    public bool $staticContext = false;
    /**
     * Represents interned strings and concatenations made in the project.
     */
    public ?StringsManager $stringsManager = null;
    /**
     * Current symbol table.
     */
    public ?SymbolTable $symbolTable = null;
    /**
     * Type inference data.
     */
    public ?StaticTypeInference $typeInference = null;

    /**
     * Lookup a class from a given class name.
     */
    public function classLookup(string $className, array $statement = []): AbstractDefinition
    {
        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $this->getFullName($className);
            if ($this->compiler->isClass($className)) {
                return $this->compiler->getClassDefinition($className);
            }

            throw new CompilerException("Cannot locate class '$className'", $statement);
        }

        if (in_array($className, ['self', 'static'])) {
            return $this->classDefinition;
        }

        $parent = $this->classDefinition->getExtendsClass();
        if (!$parent instanceof Definition) {
            throw new CompilerException(
                sprintf(
                    'Cannot access parent:: because class %s does not extend any class',
                    $this->classDefinition->getCompleteName()
                ),
                $statement
            );
        }

        return $this->classDefinition->getExtendsClassDefinition();
    }

    /**
     * Registers a `switch` as the innermost `break`/`continue` target.
     */
    public function pushSwitchTarget(string $endLabel): void
    {
        $this->switchTargets[] = [
            'label'      => $endLabel,
            'cycleDepth' => $this->insideCycle,
            'used'       => false,
        ];
    }

    /**
     * Drops the innermost `switch` target and tells whether its end label was
     * ever jumped to - an unreferenced C label would warn.
     */
    public function popSwitchTarget(): bool
    {
        $target = array_pop($this->switchTargets);

        return (bool) ($target['used'] ?? false);
    }

    /**
     * Label that a `break`/`continue` written at the current position must jump
     * to, marking it as referenced. NULL when a C `break`/`continue` is the
     * right emission - that is, when the innermost enclosing construct is a
     * loop, or when there is no enclosing `switch` at all.
     */
    public function useSwitchEndLabel(): ?string
    {
        $last = array_key_last($this->switchTargets);

        if (null === $last || $this->switchTargets[$last]['cycleDepth'] !== $this->insideCycle) {
            return null;
        }

        $this->switchTargets[$last]['used'] = true;

        return $this->switchTargets[$last]['label'];
    }

    /**
     * C lines that leave the current position with the pending exception:
     * a jump to the innermost enclosing `try`, or out of the function. The
     * restore and the return stay on separate lines so that a function
     * without a memory frame drops the restore; see
     * Method::removeMemoryStackReferences().
     *
     * A property initializer returns `zend_object *` and only evaluates
     * folded constants, which cannot throw, so it gets no exit.
     *
     * @return list<string>
     */
    public function exceptionExitLines(): array
    {
        if ($this->insideTryCatch) {
            return ['goto try_end_' . $this->currentTryCatch . ';'];
        }

        if ($this->currentMethod instanceof Method && $this->currentMethod->isInitializer() && !$this->currentMethod->isStatic()) {
            return [];
        }

        return ['ZEPHIR_MM_RESTORE();', 'return;'];
    }

    /**
     * Leaves the current position with the pending exception, as a `throw`
     * does.
     */
    public function emitExceptionExit(?Printer $printer = null): void
    {
        $printer ??= $this->codePrinter;
        foreach ($this->exceptionExitLines() as $line) {
            $printer->output($line);
        }
    }

    /**
     * Stops at an operation that left an exception pending, as PHP does.
     */
    public function emitExceptionCheck(?Printer $printer = null): void
    {
        $lines = $this->exceptionExitLines();
        if ([] === $lines) {
            return;
        }

        $printer ??= $this->codePrinter;
        $printer->output('if (UNEXPECTED(EG(exception))) {');
        $printer->increaseLevel();
        foreach ($lines as $line) {
            $printer->output($line);
        }
        $printer->decreaseLevel();
        $printer->output('}');
    }

    /**
     * Registers a do-while as the innermost `continue` target. Call it after
     * entering the loop's cycle.
     */
    public function pushDoWhileTarget(string $conditionLabel): void
    {
        $this->doWhileTargets[] = [
            'label'      => $conditionLabel,
            'cycleDepth' => $this->insideCycle,
            'used'       => false,
        ];
    }

    /**
     * Drops the innermost do-while target and tells whether its condition
     * label was ever jumped to - an unreferenced C label would warn.
     */
    public function popDoWhileTarget(): bool
    {
        $target = array_pop($this->doWhileTargets);

        return (bool) ($target['used'] ?? false);
    }

    /**
     * Label that a `continue` written at the current position must jump to,
     * marking it as referenced. NULL when the innermost loop is not a
     * do-while, so a C `continue` is right.
     */
    public function useDoWhileConditionLabel(): ?string
    {
        $last = array_key_last($this->doWhileTargets);

        if (null === $last || $this->doWhileTargets[$last]['cycleDepth'] !== $this->insideCycle) {
            return null;
        }

        $this->doWhileTargets[$last]['used'] = true;

        return $this->doWhileTargets[$last]['label'];
    }

    /**
     * Transform class/interface name to FQN format.
     */
    public function getFullName(string $className): string
    {
        $isFunction = $this->currentMethod instanceof FunctionDefinition;
        $namespace  = $isFunction ? $this->currentMethod->getNamespace() : $this->classDefinition->getNamespace();

        return Name::fetchFQN($className, $namespace, $this->aliasManager);
    }
}
