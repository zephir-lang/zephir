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

namespace Zephir\Passes;

use Zephir\StatementsBlock;

/**
 * LocalContextPass
 *
 * This pass tries to check whether variables only do exist in the local context of the method block
 * or if they're used externally which will unallow variables to be placed in the stack
 *
 * This pass also tracks the number of initializations a variable may have, this allows
 * to mark variables as read-only after their last initialization. The number of
 * mutations is relative, since assignments inside cycles/loops may perform a n-number of
 * mutations
 *
 * @see http://en.wikipedia.org/wiki/Escape_analysis
 */
class LocalContextPass
{
    protected $variables = array();

    protected $mutations = array();

    protected $uses = array();

    protected $lastCallLine = false;

    protected $lastUnsetLine = false;

    /**
     * Do the compilation pass
     *
     * @param StatementsBlock $block
     */
    public function pass(StatementsBlock $block)
    {
        $this->passStatementBlock($block->getStatements());
    }

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
                    $this->variables[$variable['variable']] = false;
                    continue;
                }
            }
            if (!isset($this->variables[$variable['variable']])) {
                $this->variables[$variable['variable']] = true;
            }
        }
    }

    /**
     * Marks a variable to mandatory be stored in the heap
     *
     * @param string $variable
     */
    public function markVariableNoLocal($variable)
    {
        if (isset($this->variables[$variable])) {
            $this->variables[$variable] = false;
        }
    }

    /**
     * Marks the lastest use/read of a variable
     *
     * @param string $variable
     * @param array $node
     */
    public function markLastUse($variable, $node)
    {
        if (isset($node['line'])) {
            $this->uses[$variable] = $node['line'];
        }
    }

    /**
     * Asks the local context information whether a variable can be stored in the stack instead of the heap
     *
     * @param string $variable
     *
     * @return boolean
     */
    public function shouldBeLocal($variable)
    {
        if (isset($this->variables[$variable])) {
            if ($this->variables[$variable]) {
                return true;
            }
        }

        return false;
    }

    /**
     * Increase the number of mutations a variable has inside a statement block
     *
     * @param string $variable
     */
    public function increaseMutations($variable)
    {
        if (isset($this->mutations[$variable])) {
            $this->mutations[$variable]++;
        } else {
            $this->mutations[$variable] = 1;
        }
    }

    /**
     * Returns the number of assignment instructions that mutated a variable
     *
     * @param string $variable
     * @return int
     */
    public function getNumberOfMutations($variable)
    {
        if (isset($this->mutations[$variable])) {
            return $this->mutations[$variable];
        }

        return 0;
    }

    /**
     * Returns the latest line where a variable was read
     *
     * @param string $variable
     * @return int
     */
    public function getLastVariableUseLine($variable)
    {
        if (isset($this->uses[$variable])) {
            return $this->uses[$variable];
        }
        return 0;
    }

    /**
     * Returns the line where the latest call in the method was made
     *
     * @return int
     */
    public function getLastCallLine()
    {
        return $this->lastCallLine;
    }

    /**
     * Returns the line where the latest unset operation was made
     *
     * @return int
     */
    public function getLastUnsetLine()
    {
        return $this->lastUnsetLine;
    }

    public function passLetStatement(array $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
            $this->increaseMutations($assignment['variable']);

            switch ($assignment['assign-type']) {

                case 'variable':

                    switch ($assignment['operator']) {
                        case 'mul-assign':
                        case 'sub-assign':
                        case 'add-assign':
                            $this->markVariableNoLocal($assignment['variable']);
                            break;
                    }

                    switch ($assignment['expr']['type']) {

                        case 'property-access':
                        case 'property-dynamic-access':
                        case 'property-string-access':
                        case 'array-access':
                        case 'static-property-access':
                        case 'static-constant-access':
                        case 'string':
                        case 'array':
                        case 'empty-array':
                        case 'fcall':
                        case 'mcall':
                        case 'scall':
                        case 'concat':
                        case 'clone':
                        case 'require':
                        case 'type-hint':
                        case 'minus':
                        case 'new':
                        case 'new-type':
                        case 'closure':
                        case 'closure-arrow':
                        case 'reference':
                        case 'irange':
                        case 'erange':
                            $this->markVariableNoLocal($assignment['variable']);
                            break;

                        case 'constant':
                            if (defined($assignment['expr']['value'])) {
                                if (gettype(constant($assignment['expr']['value'])) == 'string') {
                                    $this->markVariableNoLocal($assignment['variable']);
                                }
                            }
                            break;

                        case 'variable':
                            $this->markVariableNoLocal($assignment['expr']['value']);
                            $this->markVariableNoLocal($assignment['variable']);
                            break;

                        case 'cast':
                            switch ($assignment['expr']['left']) {
                                case 'array':
                                case 'string':
                                    $this->markVariableNoLocal($assignment['variable']);
                                    break;
                            }
                            break;

                        default:
                            //echo '[', $assignment['expr']['type'], ']', PHP_EOL;
                    }
                    break;

                case 'object-property':
                case 'array-index':
                case 'object-property-array-index':
                case 'object-property-append':

                    switch ($assignment['expr']['type']) {
                        case 'variable':
                            $this->markVariableNoLocal($assignment['expr']['value']);
                            break;
                    }
                    $this->markVariableNoLocal($assignment['variable']);
                    break;

                case 'variable-append':
                    $this->markVariableNoLocal($assignment['variable']);
                    switch ($assignment['expr']['type']) {
                        case 'variable':
                            $this->markVariableNoLocal($assignment['expr']['value']);
                            break;
                        default:
                            //echo '[', $assignment['assign-type'], ']';
                    }
                    break;

                default:
                    //echo $assignment['assign-type'];
            }
        }
    }

    public function passCall(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if ($parameter['parameter']['type'] == 'variable') {
                    $this->markVariableNoLocal($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    public function passArray(array $expression)
    {
        foreach ($expression['left'] as $item) {
            if ($item['value']['type'] == 'variable') {
                $this->markVariableNoLocal($item['value']['value']);
            } else {
                $this->passExpression($item['value']);
            }
        }
    }

    public function passNew(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if ($parameter['parameter']['type'] == 'variable') {
                    $this->markVariableNoLocal($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
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
            case 'string':
            case 'istring':
            case 'null':
            case 'char':
            case 'uchar':
            case 'empty-array':
            case 'constant':
            case 'static-constant-access':
            case 'closure':
            case 'closure-arrow':
                break;

            case 'variable':
                $this->markLastUse($expression['value'], $expression);
                break;

            case 'reference':
                if ($expression['left']['type'] == 'variable') {
                    $this->markVariableNoLocal($expression['left']['value']);
                }
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
            case 'irange':
            case 'erange':
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
                if ($expression['type'] == 'mcall') {
                    if (isset($expression['variable'])) {
                        if ($expression['variable']['type'] == 'variable') {
                            $this->markVariableNoLocal($expression['variable']['value']);
                        }
                    }
                }
                $this->passCall($expression);
                $this->lastCallLine = $expression['line'];
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
                $this->markVariableNoLocal($expression['left']['value']);
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
                echo 'LocalContextPassType=', $expression['type'], PHP_EOL;
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
                        $this->markLastUse($statement['value'], $statement);
                        $this->markVariableNoLocal($statement['value']);
                        $this->increaseMutations($statement['value']);
                    }
                    if (isset($statement['key'])) {
                        $this->markLastUse($statement['key'], $statement);
                        $this->markVariableNoLocal($statement['key']);
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
                                $this->markVariableNoLocal($catch['variable']['value']);
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
                    $this->lastCallLine = $statement['line'];
                    break;

                case 'fetch':
                    $this->passExpression($statement['expr']);
                    break;

                case 'mcall':
                case 'scall':
                case 'fcall':
                case 'require':
                    if ($statement['expr']['type'] == 'mcall') {
                        if (isset($statement['expr']['variable'])) {
                            if ($statement['expr']['variable']['type'] == 'variable') {
                                $this->markVariableNoLocal($statement['expr']['variable']['value']);
                            }
                        }
                    }
                    $this->passCall($statement['expr']);
                    $this->lastCallLine = $statement['line'];
                    break;

                case 'unset':
                    if ($statement['expr']['type'] == "array-access") {
                        if ($statement['expr']['left']['type'] == 'variable') {
                            $this->markVariableNoLocal($statement['expr']['left']['value']);
                            $this->increaseMutations($statement['expr']['left']['value']);
                        }
                    } else {
                        if ($statement['expr']['type'] == "list") {
                            if ($statement['expr']['left']['type'] == 'array-access') {
                                if ($statement['expr']['left']['left']['type'] == 'variable') {
                                    $this->markVariableNoLocal($statement['expr']['left']['left']['value']);
                                    $this->increaseMutations($statement['expr']['left']['left']['value']);
                                }
                            }
                        }
                    }
                    $this->lastUnsetLine = $statement['line'];
                    break;

                case 'break':
                case 'continue':
                case 'empty':
                case 'cblock':
                case 'comment':
                    break;

                default:
                    echo 'Statement=', $statement['type'], PHP_EOL;
            }
        }
    }
}
