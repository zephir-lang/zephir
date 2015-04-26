<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir\Detectors;

/**
 * WriteDetector
 *
 * Detects whether a variable is mutated in a given context
 * If a variable is not modified in a local context (method block) we can avoid allocate
 * memory for its body (zvalue)
 *
 * Separate parameters to avoid them to be touched by modifying its reference count
 */
class WriteDetector
{
    protected $_detectionFlags = 0;

    protected $_mutations = array();

    const DETECT_NONE = 0;

    const DETECT_PARAM_PASS = 1;

    const DETECT_ARRAY_USE  = 2;

    const DETECT_VALUE_IN_ASSIGNMENT = 4;

    const DETECT_ALL  = 255;

    /**
     * Do the detection pass on a single variable
     *
     * @param string $variable
     * @param array $statements
     * @return boolean
     */
    public function detect($variable, array $statements)
    {
        $this->passStatementBlock($statements);
        return $this->getNumberOfMutations($variable) > 0;
    }

    /**
     * Sets detection flags
     *
     * @param int $flags
     */
    public function setDetectionFlags($flags)
    {
        $this->_detectionFlags = $flags;
    }

    /**
     * Increase the number of mutations a variable has inside a statement block
     *
     * @param string $variable
     * @return ForValueUseDetector
     */
    public function increaseMutations($variable)
    {
        if (isset($this->_mutations[$variable])) {
            $this->_mutations[$variable]++;
        } else {
            $this->_mutations[$variable] = 1;
        }
        return $this;
    }

    /**
     * Returns the number of assignment instructions that mutated a variable
     *
     * @param string $variable
     * @return int
     */
    public function getNumberOfMutations($variable)
    {
        if (isset($this->_mutations[$variable])) {
            return $this->_mutations[$variable];
        }
        return 0;
    }

    /**
     * Pass let statements
     *
     * @param array $statement
     */
    public function passLetStatement(array $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
            $this->increaseMutations($assignment['variable']);
            if (($this->_detectionFlags & self::DETECT_VALUE_IN_ASSIGNMENT) == self::DETECT_VALUE_IN_ASSIGNMENT) {
                if (isset($assignment['expr'])) {
                    if ($assignment['expr']['type'] == 'variable') {
                        $this->increaseMutations($assignment['expr']['value']);
                        break;
                    }
                }
            }
        }
    }

    /**
     * Pass call expressions
     *
     * @param array $expression
     */
    public function passCall(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $usePass = ($this->_detectionFlags & self::DETECT_PARAM_PASS) == self::DETECT_PARAM_PASS;
                if ($usePass && $parameter['parameter']['type'] == 'variable') {
                    $this->increaseMutations($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass array expressions
     *
     * @param array $expression
     */
    public function passArray(array $expression)
    {
        foreach ($expression['left'] as $item) {
            $usePass = ($this->_detectionFlags & self::DETECT_ARRAY_USE) == self::DETECT_ARRAY_USE;
            if ($usePass && $item['value']['type'] == 'variable') {
                $this->increaseMutations($item['value']['value']);
            } else {
                $this->passExpression($item['value']);
            }
        }
    }

    /**
     * Pass "new" expressions
     *
     * @param array $expression
     */
    public function passNew(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $usePass = ($this->_detectionFlags & self::DETECT_PARAM_PASS) == self::DETECT_PARAM_PASS;
                if ($usePass && $parameter['parameter']['type'] == 'variable') {
                    $this->increaseMutations($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass "declare" statement
     *
     * @param array $statement
     */
    public function declareVariables(array $statement)
    {
        if (isset($statement['data-type'])) {
            if ($statement['data-type'] != 'variable') {
                return;
            }
        }

        foreach ($statement['variables'] as $variable) {
            if (isset($variable['expr'])) {
                if ($variable['expr']['type'] == 'string' || $variable['expr']['type'] == 'empty-array' || $variable['expr']['type'] == 'array') {
                    continue;
                }
            }

            $this->increaseMutations($variable['variable']);
        }
    }


    /**
     * Pass expressions
     *
     * @param array $expression
     */
    public function passExpression(array $expression)
    {
        switch ($expression['type']) {

            case 'bool':
            case 'double':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'string':
            case 'null':
            case 'char':
            case 'uchar':
            case 'empty-array':
            case 'variable':
            case 'constant':
            case 'static-constant-access':
            case 'closure':
            case 'closure-arrow':
            case 'reference':
                break;

            case 'sub':
            case 'add':
            case 'div':
            case 'mul':
            case 'mod':
            case 'and':
            case 'or':
            case 'concat':
            case 'equals':
            case 'identical':
            case 'not-identical':
            case 'not-equals':
            case 'less':
            case 'greater':
            case 'greater-equal':
            case 'less-equal':
            case 'bitwise_and':
            case 'bitwise_or':
            case 'bitwise_xor':
            case 'bitwise_shiftleft':
            case 'bitwise_shiftright':
                $this->passExpression($expression['left']);
                $this->passExpression($expression['right']);
                break;

            case 'typeof':
            case 'not':
                $this->passExpression($expression['left']);
                break;

            case 'mcall':
            case 'fcall':
            case 'scall':
                $this->passCall($expression);
                break;

            case 'array':
                $this->passArray($expression);
                break;

            case 'new':
            case 'new-type':
                $this->passNew($expression);
                break;

            case 'property-access':
            case 'property-dynamic-access':
            case 'property-string-access':
            case 'static-property-access':
            case 'array-access':
                $this->passExpression($expression['left']);
                break;

            case 'isset':
            case 'empty':
            case 'instanceof':
            case 'require':
            case 'clone':
            case 'likely':
            case 'unlikely':
            case 'ternary': /* do special pass later */
                $this->passExpression($expression['left']);
                break;

            case 'fetch':
                $this->increaseMutations($expression['left']['value']);
                $this->passExpression($expression['right']);
                break;

            case 'minus':
                $this->passExpression($expression['left']);
                break;

            case 'list':
                $this->passExpression($expression['left']);
                break;

            case 'cast':
            case 'type-hint':
                $this->passExpression($expression['right']);
                break;

            default:
                echo 'ForValueUseDetectorType=', $expression['type'], PHP_EOL;
                break;
        }
    }

    /**
     * Pass statement block
     *
     * @param array $statements
     */
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
                    $this->declareVariables($statement);
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
                    if (isset($statement['value'])) {
                        $this->increaseMutations($statement['value']);
                    }
                    if (isset($statement['key'])) {
                        $this->increaseMutations($statement['key']);
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
                    if (isset($statement['catches'])) {
                        foreach ($statement['catches'] as $catch) {
                            if (isset($catch['variable'])) {
                                $this->increaseMutations($catch['variable']['value']);
                            }
                            if (isset($catch['statements'])) {
                                $this->passStatementBlock($catch['statements']);
                            }
                        }
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
                case 'fcall':
                case 'require':
                    $this->passCall($statement['expr']);
                    break;

                case 'unset':
                    if ($statement['expr']['type'] == "array-access") {
                        if ($statement['expr']['left']['type'] == 'variable') {
                            $this->increaseMutations($statement['expr']['left']['value']);
                        }
                    } else {
                        if ($statement['expr']['type'] == "list") {
                            if ($statement['expr']['left']['type'] == 'array-access') {
                                if ($statement['expr']['left']['left']['type'] == 'variable') {
                                    $this->increaseMutations($statement['expr']['left']['left']['value']);
                                }
                            }
                        }
                    }
                    break;

                case 'break':
                case 'continue':
                case 'empty':
                case 'cblock':
                case 'comment':
                    break;

                default:
                    echo 'WriteDetectorStatement=', $statement['type'], PHP_EOL;
            }
        }
    }
}
