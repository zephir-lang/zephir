<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Passes;

use Zephir\StatementsBlock;
use Zephir\FunctionCall;

/**
 * CallGathererPass
 *
 * This pass counts how many times the same function is called inside a
 * statements block. It also count how many times a method is calling
 * trying to track the possible caller.
 *
 * This pass is used by the function/method caches to explore possible
 * optimizations by implementing inline caches.
 */
class CallGathererPass
{

    protected $functionCalls = array();

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
        foreach ($statement['assignments'] as $assigment) {
            if (isset($assigment['expr'])) {
                $this->passExpression($assigment['expr']);
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
            case 'static-constant-access':
            case 'variable':
            case 'constant':
            case 'empty-array':
                break;

            case 'div':
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
                $left = $this->passExpression($expression['left']);
                $right = $this->passExpression($expression['right']);
                break;

            case 'typeof':
                $this->passExpression($expression['left']);
                break;

            case 'minus':
                $this->passExpression($expression['left']);
                break;

            case 'not':
                $this->passExpression($expression['left']);
                break;

            case 'mcall':
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
                $this->passExpression($expression['right']);
                $this->passExpression($expression['extra']);
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
                    $this->passCall($statement['expr']);
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
                case 'empty': // empty statement != empty operator
                    break;

                default:
                    echo 'SCGP=', $statement['type'];
            }
        }
    }
}
