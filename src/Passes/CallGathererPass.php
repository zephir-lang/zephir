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

namespace Zephir\Passes;

use Zephir\CompilationContext;
use Zephir\FunctionCall;
use Zephir\StatementsBlock;

/**
 * This pass counts how many times the same function is called inside a
 * statements block. It also counts how many times a method is calling
 * trying to track the possible caller.
 *
 * This pass is used by the function/method caches to explore possible
 * optimizations by implementing inline caches.
 */
class CallGathererPass
{
    protected array $functionCalls = [];

    protected array $methodCalls = [];

    public function __construct(protected CompilationContext $compilationContext)
    {
    }

    /**
     * Gets current CompilationContext.
     *
     * @return CompilationContext
     */
    public function getCompilationContext(): CompilationContext
    {
        return $this->compilationContext;
    }

    /**
     * Returns the number of calls a function had.
     *
     * @param string $funcName
     *
     * @return int
     */
    public function getNumberOfFunctionCalls(string $funcName): int
    {
        return $this->functionCalls[$funcName] ?? 0;
    }

    /**
     * Returns the number of calls a function had.
     *
     * @param string $className
     * @param string $methodName
     *
     * @return int
     */
    public function getNumberOfMethodCalls(string $className, string $methodName): int
    {
        return $this->methodCalls[$className][$methodName] ?? 0;
    }

    /**
     * Do the compilation pass.
     *
     * @param StatementsBlock $block
     */
    public function pass(StatementsBlock $block): void
    {
        $this->passStatementBlock($block->getStatements());
    }

    public function passArray(array $expression): void
    {
        foreach ($expression['left'] as $item) {
            $this->passExpression($item['value']);
        }
    }

    public function passCall(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passExpression(array $expression): void
    {
        switch ($expression['type']) {
            case 'bool':
            case 'double':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'null':
            case 'char':
            case 'uchar':
            case 'string':
            case 'istring':
            case 'static-constant-access':
            case 'variable':
            case 'constant':
            case 'empty-array':
            case 'closure':
            case 'closure-arrow':
            case 'reference':
            case 'short-ternary':
                break;

            case 'div':
            case 'mod':
            case 'sub':
            case 'add':
            case 'mul':
            case 'bitwise_and':
            case 'bitwise_or':
            case 'bitwise_xor':
            case 'bitwise_shiftleft':
            case 'bitwise_shiftright':
            case 'concat':
            case 'and':
            case 'or':
            case 'equals':
            case 'identical':
            case 'not-identical':
            case 'not-equals':
            case 'less':
            case 'greater':
            case 'greater-equal':
            case 'less-equal':
            case 'irange':
            case 'erange':
                $this->passExpression($expression['left']);
                $this->passExpression($expression['right']);
                break;

            case 'minus':
            case 'bitwise_not':
            case 'not':
            case 'static-property-access':
            case 'array-access':
            case 'property-string-access':
            case 'property-dynamic-access':
            case 'property-access':
            case 'unlikely':
            case 'likely':
            case 'instanceof':
            case 'empty':
            case 'isset':
            case 'list':
            case 'require_once':
            case 'require':
            case 'clone':
            case 'typeof':
                $this->passExpression($expression['left']);
                break;

            case 'mcall':
                if (isset($expression['variable']['value'])) {
                    if ('this' == $expression['variable']['value']) {
                        $methodName = $expression['name'];
                        $className  = $this->compilationContext->classDefinition->getCompleteName();
                        if (!isset($this->methodCalls[$className][$methodName])) {
                            $this->methodCalls[$className][$methodName] = 1;
                        } else {
                            ++$this->methodCalls[$className][$methodName];
                        }
                    }
                }
                $this->passCall($expression);
                break;

            case 'scall':
                $this->passCall($expression);
                break;

            case 'fcall':
                if (FunctionCall::CALL_NORMAL == $expression['call-type']) {
                    $name = $expression['name'];
                    if (!isset($this->functionCalls[$name])) {
                        $this->functionCalls[$name] = 1;
                    } else {
                        ++$this->functionCalls[$name];
                    }
                }
                $this->passCall($expression);
                break;

            case 'array':
                $this->passArray($expression);
                break;

            case 'new':
                $this->passNew($expression);
                break;

            case 'new-type':
                $this->passNewType($expression);
                break;

            case 'cast':
            case 'type-hint':
            case 'short-ternary':
            case 'ternary':
            case 'fetch':
                $this->passExpression($expression['right']);
                break;

            default:
                echo 'CGP=', $expression['type'], PHP_EOL;
                break;
        }
    }

    public function passLetStatement(array $statement): void
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
        }
    }

    public function passNew(array $expression): void
    {
        if (!$expression['dynamic']) {
            $className = $this->compilationContext->getFullName($expression['class']);
            if (!isset($this->methodCalls[$className]['__construct'])) {
                $this->methodCalls[$className]['__construct'] = 1;
            } else {
                ++$this->methodCalls[$className]['__construct'];
            }
        }

        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passNewType(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passStatementBlock(array $statements): void
    {
        foreach ($statements as $statement) {
            switch ($statement['type']) {
                // empty statement != empty operator
                case 'empty':
                case 'comment':
                case 'cblock':
                case 'unset':
                case 'continue':
                case 'break':
                case 'declare':
                    break;

                case 'let':
                    $this->passLetStatement($statement);
                    break;

                case 'echo':
                    if (isset($statement['expressions'])) {
                        foreach ($statement['expressions'] as $expr) {
                            $this->passExpression($expr);
                        }
                    }
                    break;

                case 'if':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    if (isset($statement['else_statements'])) {
                        $this->passStatementBlock($statement['else_statements']);
                    }
                    break;

                case 'switch':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    if (isset($statement['clauses'])) {
                        foreach ($statement['clauses'] as $clause) {
                            if (isset($clause['expr'])) {
                                $this->passExpression($clause['expr']);
                            }
                            if (isset($clause['statements'])) {
                                $this->passStatementBlock($clause['statements']);
                            }
                        }
                    }
                    break;

                case 'while':
                case 'for':
                case 'do-while':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'throw':
                case 'require_once':
                case 'require':
                case 'return':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    break;

                case 'try-catch':
                case 'loop':
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'scall':
                case 'mcall':
                case 'fetch':
                    $this->passExpression($statement['expr']);
                    break;

                case 'fcall':
                    $expr = $statement['expr'];
                    if (FunctionCall::CALL_NORMAL == $expr['call-type']) {
                        $name = $expr['name'];
                        if (!isset($this->functionCalls[$name])) {
                            $this->functionCalls[$name] = 1;
                        } else {
                            ++$this->functionCalls[$name];
                        }
                    }
                    $this->passCall($expr);
                    break;

                default:
                    echo 'SCGP=', $statement['type'];
            }
        }
    }
}
