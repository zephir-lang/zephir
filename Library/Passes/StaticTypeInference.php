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
 * StaticTypeInference
 *
 * This pass try to infer typing on dynamic variables so the compiler
 * can replace them by low level types automatically
 */
class StaticTypeInference
{
    protected $variables = array();

    protected $infered = array();

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
        foreach ($statement['variables'] as $variable) {
            if (!isset($this->variables[$variable['variable']])) {
                if (isset($variable['expr']['type'])) {
                    if ($variable['expr']['type'] == 'empty-array' || $variable['expr']['type'] == 'array') {
                        $this->markVariable($variable['variable'], 'array');
                    } else {
                        $this->markVariable($variable['variable'], $variable['expr']['type']);
                    }
                }
            }
        }
    }

    /**
     * Marks a variable to mandatory be stored in the heap if a type has not been defined for it
     *
     * @param string $variable
     * @param string $type
     */
    public function markVariableIfUnknown($variable, $type)
    {
        $this->variables[$variable] = $type;
        $this->infered[$variable] = $type;
    }

    /**
     * Marks a variable to mandatory be stored in the heap
     *
     * @param string $variable
     * @param string $type
     */
    public function markVariable($variable, $type)
    {
        if (isset($this->variables[$variable])) {
            $currentType = $this->variables[$variable];
            if ($currentType == 'undefined') {
                return;
            }
        } else {
            $this->variables[$variable] = $type;
            return;
        }

        switch ($currentType) {

            case 'numeric':
                switch ($type) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        break;
                    default:
                        $this->variables[$variable] = 'undefined';
                        break;
                }
                break;

            case 'bool':
                switch ($type) {
                    case 'bool':
                    case 'numeric':
                        break;
                    default:
                        $this->variables[$variable] = 'undefined';
                        break;
                }
                break;

            case 'double':
                switch ($type) {
                    case 'double':
                    case 'numeric':
                        break;
                    default:
                        $this->variables[$variable] = 'undefined';
                        break;
                }
                break;

            case 'string':
                switch ($type) {
                    case 'string':
                        break;
                    default:
                        $this->variables[$variable] = 'undefined';
                        break;
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'char':
            case 'uchar':
                switch ($type) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                    case 'numeric':
                        break;
                    default:
                        $this->variables[$variable] = 'undefined';
                        break;
                }
                break;

            case 'null':
                if ($type != 'null') {
                    $this->variables[$variable] = 'undefined';
                }
                break;

            case 'variable':
                $this->variables[$variable] = 'undefined';
                break;

            case 'array':
                $this->variables[$variable] = 'undefined';
                break;

            default:
                echo 'StaticTypeInference=', $currentType, ' ', $type, PHP_EOL;
                break;
        }
    }

    /**
     * Process the found infered types and schedule a new pass
     *
     * @return boolean
     */
    public function reduce()
    {
        $pass = false;
        foreach ($this->variables as $variable => $type) {
            if ($type == 'variable' || $type == 'string' || $type == 'istring' || $type == 'array' || $type == 'null' || $type == 'numeric') {
                unset($this->variables[$variable]);
            } else {
                $pass = true;
                $this->infered[$variable] = $type;
            }
        }
        return $pass;
    }

    /**
     * Asks the local context information whether a variable can be stored in the stack instead of the heap
     *
     * @param string $variable
     * @return boolean
     */
    public function getInferedType($variable)
    {
        if (isset($this->variables[$variable])) {
            $type = $this->variables[$variable];
            if ($type != 'variable' && $type != 'undefined' && $type != 'string' && $type != 'istring' && $type != 'array' && $type != 'null' && $type != 'numeric') {
                //echo $variable, ' ', $type, PHP_EOL;
                return $type;
            }
        }
        return false;
    }

    public function passLetStatement(array $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            switch ($assignment['assign-type']) {

                case 'variable':
                    $type = $this->passExpression($assignment['expr']);
                    if (is_string($type)) {
                        $this->markVariable($assignment['variable'], $type);
                    }
                    break;

                case 'object-property':
                case 'array-index':
                case 'object-property-array-index':
                case 'object-property-append':
                case 'static-property-access':
                    $this->markVariable($assignment['variable'], 'variable');
                    break;

                case 'variable-append':
                    $this->markVariable($assignment['variable'], 'variable');
                    break;

                default:
                    // echo $assignment['assign-type'];
            }
        }
    }

    public function passCall(array $expression)
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if ($parameter['parameter']['type'] == 'variable') {
                    //$this->markVariable($parameter['value']);
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
                //$this->markVariable($item['value']['value'], 'dynamical');
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
                    //$this->markVariable($parameter['value'], 'dynamical');
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
            case 'null':
            case 'char':
            case 'uchar':
            case 'istring':
                return $expression['type'];

            case 'string':
                if (ctype_digit($expression['value'])) {
                    return 'long';
                }
                return 'string';

            case 'closure':
            case 'closure-arrow':
            case 'static-constant-access':
                return 'variable';

            case 'reference':
                if ($expression['left']['type'] == 'variable') {
                    $this->markVariable($expression['left']['value'], 'variable');
                }
                return 'variable';

            case 'div':
                return 'double';

            case 'sub':
            case 'add':
            case 'mul':
            case 'bitwise_and':
            case 'bitwise_or':
            case 'bitwise_xor':
            case 'bitwise_shiftleft':
            case 'bitwise_shiftright':
                $left = $this->passExpression($expression['left']);
                $right = $this->passExpression($expression['right']);
                if ($left == 'int' && $right == 'int') {
                    return 'int';
                }
                if ($left == 'uint' && $right == 'uint') {
                    return 'uint';
                }
                if ($left == 'long' && $right == 'long') {
                    return 'long';
                }
                if ($left == 'double' && $right == 'double') {
                    return 'double';
                }
                if ($left == 'double' && $right == 'int') {
                    return 'double';
                }
                if ($left == 'double' && $right == 'long') {
                    return 'double';
                }
                if ($left == 'int' && $right == 'double') {
                    return 'double';
                }
                if ($left == 'int' && $right == 'bool') {
                    return 'int';
                }
                if ($left == 'bool' && $right == 'int') {
                    return 'bool';
                }
                if ($left == 'bool' && $right == 'double') {
                    return 'bool';
                }
                if ($left == $right) {
                    return $left;
                }
                return 'numeric';

            case 'mod':
                $left = $this->passExpression($expression['left']);
                $right = $this->passExpression($expression['right']);
                if ($left == 'long' && $right == 'long') {
                    return 'long';
                }
                if ($left == 'ulong' && $right == 'ulong') {
                    return 'ulong';
                }
                return 'int';

            case 'and':
            case 'or':
                return 'bool';

            case 'concat':
                return 'string';

            case 'equals':
            case 'identical':
            case 'not-identical':
            case 'not-equals':
            case 'less':
            case 'greater':
            case 'greater-equal':
            case 'less-equal':
                return 'bool';

            case 'irange':
            case 'erange':
                return 'variable';

            case 'typeof':
                $this->passExpression($expression['left']);
                return 'string';

            case 'minus':
                $this->passExpression($expression['left']);
                return 'variable';

            case 'not':
                $this->passExpression($expression['left']);
                return 'bool';

            case 'mcall':
            case 'fcall':
            case 'scall':
                $this->passCall($expression);
                return 'undefined';

            case 'array':
                $this->passArray($expression);
                return 'variable';

            case 'empty-array':
                return 'variable';

            case 'new':
            case 'new-type':
                $this->passNew($expression);
                return 'undefined';

            case 'property-access':
            case 'property-dynamic-access':
            case 'property-string-access':
            case 'array-access':
            case 'static-property-access':
                $this->passExpression($expression['left']);
                return 'undefined';

            case 'fetch':
                $this->markVariable($expression['left']['value'], 'variable');
                $this->passExpression($expression['right']);
                return 'bool';

            case 'isset':
            case 'empty':
            case 'instanceof':
            case 'likely':
            case 'unlikely':
                $this->passExpression($expression['left']);
                return 'bool';

            case 'list':
                return $this->passExpression($expression['left']);

            case 'cast':
                return $expression['left'];

            case 'type-hint':
                return $this->passExpression($expression['right']);

            case 'variable':
                if (isset($this->infered[$expression['value']])) {
                    return $this->infered[$expression['value']];
                }
                return null;

            case 'constant':
                return null;

            case 'clone':
            case 'require':
                return 'variable';

            case 'ternary':
                //$right = $this->passExpression($expression['right']);
                //$extra = $this->passExpression($expression['extra']);
                /*if ($right == $extra) {
                    if ($right != 'string' && $right != 'array') {
                        return $right;
                    }
                }*/
                return 'undefined';

            default:
                echo 'STI=', $expression['type'], PHP_EOL;
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
                        $this->markVariableIfUnknown($statement['value'], 'undefined');
                    }
                    if (isset($statement['key'])) {
                        $this->markVariableIfUnknown($statement['key'], 'undefined');
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
                case 'fcall':
                case 'require':
                    $this->passCall($statement['expr']);
                    break;

                case 'break':
                case 'continue':
                case 'unset':
                case 'cblock':
                case 'comment':
                case 'empty': // empty statement != empty operator
                    break;

                default:
                    echo 'SSTI=', $statement['type'];
            }
        }
    }
}
