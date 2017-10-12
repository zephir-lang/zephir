<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Passes;

use Zephir\FunctionCall;
use Zephir\StatementsBlock;
use Zephir\CompilationContext;

/**
 * Zephir\Passes\CallGathererPass
 *
 * This pass counts how many times the same function is called inside a
 * statements block. It also count how many times a method is calling
 * trying to track the possible caller.
 *
 * This pass is used by the function/method caches to explore possible
 * optimizations by implementing inline caches.
 *
 * @package Zephir\Passes
 */
class CallGathererPass
{
    protected $functionCalls = [];

    protected $methodCalls = [];

    protected $compilationContext;

    /**
     * CallGathererPass constructor
     *
     * @param CompilationContext $compilationContext
     */
    public function __construct(CompilationContext $compilationContext)
    {
        $this->compilationContext = $compilationContext;
    }

    /**
     * Gets current CompilationContext.
     *
     * @return CompilationContext
     */
    public function getCompilationContext()
    {
        return $this->compilationContext;
    }

    /**
     * Returns the number of calls a function had
     *
     * @param string $funcName
     * @return int
     */
    public function getNumberOfFunctionCalls($funcName)
    {
        if (isset($this->functionCalls[$funcName])) {
            return $this->functionCalls[$funcName];
        }
        return 0;
    }

    /**
     * Returns the number of calls a function had
     *
     * @param string $className
     * @param string $methodName
     * @return int
     */
    public function getNumberOfMethodCalls($className, $methodName)
    {
        if (isset($this->methodCalls[$className][$methodName])) {
            return $this->methodCalls[$className][$methodName];
        }
        return 0;
    }

    /**
     * Returns all the method calls
     *
     * @return array
     */
    public function getAllMethodCalls()
    {
        return $this->methodCalls;
    }

    /**
     * Do the compilation pass
     *
     * @param StatementsBlock $block
     */
    public function pass(StatementsBlock $block)
    {
        $this->passStatementBlock($block->getStatements());
    }

    public function passLetStatement(array $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
        }
    }

    public function passCall(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passArray(array $expression)
    {
        foreach ($expression['left'] as $item) {
            $this->passExpression($item['value']);
        }
    }

    public function passNew(array $expression)
    {
        if (!$expression['dynamic']) {
            $className = $this->compilationContext->getFullName($expression['class']);
            if (!isset($this->methodCalls[$className]['__construct'])) {
                $this->methodCalls[$className]['__construct'] = 1;
            } else {
                $this->methodCalls[$className]['__construct']++;
            }
        }

        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passNewType(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $this->passExpression($parameter['parameter']);
            }
        }
    }

    public function passExpression(array $expression)
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

            case 'typeof':
                $this->passExpression($expression['left']);
                break;

            case 'minus':
                $this->passExpression($expression['left']);
                break;

            case 'not':
            case 'bitwise_not':
                $this->passExpression($expression['left']);
                break;

            case 'mcall':
                if (isset($expression['variable']['value'])) {
                    if ($expression['variable']['value'] == 'this') {
                        $methodName = $expression['name'];
                        $className = $this->compilationContext->classDefinition->getCompleteName();
                        if (!isset($this->methodCalls[$className][$methodName])) {
                            $this->methodCalls[$className][$methodName] = 1;
                        } else {
                            $this->methodCalls[$className][$methodName]++;
                        }
                    }
                }
                $this->passCall($expression);
                break;

            case 'scall':
                $this->passCall($expression);
                break;

            case 'fcall':
                if ($expression['call-type'] == FunctionCall::CALL_NORMAL) {
                    $name = $expression['name'];
                    if (!isset($this->functionCalls[$name])) {
                        $this->functionCalls[$name] = 1;
                    } else {
                        $this->functionCalls[$name]++;
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

            case 'property-access':
            case 'property-dynamic-access':
            case 'property-string-access':
            case 'array-access':
            case 'static-property-access':
                $this->passExpression($expression['left']);
                break;

            case 'fetch':
                $this->passExpression($expression['right']);
                break;

            case 'isset':
            case 'empty':
            case 'instanceof':
            case 'likely':
            case 'unlikely':
                $this->passExpression($expression['left']);
                break;

            case 'list':
                $this->passExpression($expression['left']);
                break;

            case 'cast':
                $this->passExpression($expression['right']);
                break;

            case 'type-hint':
                $this->passExpression($expression['right']);
                break;

            case 'clone':
            case 'require':
                $this->passExpression($expression['left']);
                break;

            case 'ternary':
            case 'short-ternary':
                $this->passExpression($expression['right']);
                break;

            default:
                echo 'CGP=', $expression['type'], PHP_EOL;
                break;
        }
    }

    public function passStatementBlock(array $statements)
    {
        foreach ($statements as $statement) {
            switch ($statement['type']) {
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

                case 'declare':
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
                case 'do-while':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'for':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'return':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    break;

                case 'loop':
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'try-catch':
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    break;

                case 'throw':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    break;

                case 'fetch':
                    $this->passExpression($statement['expr']);
                    break;

                case 'mcall':
                case 'scall':
                    $this->passExpression($statement['expr']);
                    break;

                case 'fcall':
                    $expr = $statement['expr'];
                    if ($expr['call-type'] == FunctionCall::CALL_NORMAL) {
                        $name = $expr['name'];
                        if (!isset($this->functionCalls[$name])) {
                            $this->functionCalls[$name] = 1;
                        } else {
                            $this->functionCalls[$name]++;
                        }
                    }
                    $this->passCall($expr);
                    break;

                case 'require':
                    if (isset($statement['expr'])) {
                        $this->passExpression($statement['expr']);
                    }
                    break;

                case 'break':
                case 'continue':
                case 'unset':
                case 'cblock':
                case 'comment':
                // empty statement != empty operator
                case 'empty':
                    break;

                default:
                    echo 'SCGP=', $statement['type'];
            }
        }
    }
}
