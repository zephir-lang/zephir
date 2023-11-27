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

use Zephir\StatementsBlock;

use function is_string;

use const PHP_EOL;

/**
 * This pass try to infer typing on dynamic variables so the compiler
 * can replace them by low level types automatically
 */
class StaticTypeInference
{
    protected array $infered   = [];
    protected array $variables = [];

    public function declareVariables(array $statement): void
    {
        foreach ($statement['variables'] as $variable) {
            if (!isset($this->variables[$variable['variable']])) {
                if (isset($variable['expr']['type'])) {
                    if ('empty-array' == $variable['expr']['type'] || 'array' == $variable['expr']['type']) {
                        $this->markVariable($variable['variable'], 'array');
                    } else {
                        $this->markVariable($variable['variable'], $variable['expr']['type']);
                    }
                }
            }
        }
    }

    /**
     * Asks the local context information whether a variable can be stored in the stack instead of the heap.
     *
     * @param string $variable
     *
     * @return bool
     */
    public function getInferedType($variable)
    {
        if (isset($this->variables[$variable])) {
            $type = $this->variables[$variable];
            if ('variable' != $type && 'undefined' != $type && 'string' != $type && 'istring' != $type && 'array' != $type && 'null' != $type && 'numeric' != $type) {
                // echo $variable, ' ', $type, PHP_EOL;
                return $type;
            }
        }

        return false;
    }

    /**
     * Marks a variable to mandatory be stored in the heap.
     *
     * @param string $variable
     * @param string $type
     */
    public function markVariable($variable, $type): void
    {
        if (isset($this->variables[$variable])) {
            $currentType = $this->variables[$variable];
            if ('undefined' == $currentType) {
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
                if ('null' != $type) {
                    $this->variables[$variable] = 'undefined';
                }
                break;

            case 'variable':
            case 'array':
            case 'mixed':
                $this->variables[$variable] = 'undefined';
                break;

            default:
                // TODO: Find the reason
                echo 'StaticTypeInference=', $currentType, ' ', $type, PHP_EOL;
                break;
        }
    }

    /**
     * Marks a variable to mandatory be stored in the heap if a type has not been defined for it.
     *
     * @param string $variable
     * @param string $type
     */
    public function markVariableIfUnknown($variable, $type): void
    {
        $this->variables[$variable] = $type;
        $this->infered[$variable]   = $type;
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
                return $expression['type'];

            case 'closure':
            case 'closure-arrow':
            case 'static-constant-access':
                return 'variable';

            case 'reference':
                if ('variable' == $expression['left']['type']) {
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
                $left  = $this->passExpression($expression['left']);
                $right = $this->passExpression($expression['right']);
                if ('int' == $left && 'int' == $right) {
                    return 'int';
                }
                if ('uint' == $left && 'uint' == $right) {
                    return 'uint';
                }
                if ('long' == $left && 'long' == $right) {
                    return 'long';
                }
                if ('double' == $left && 'double' == $right) {
                    return 'double';
                }
                if ('double' == $left && 'int' == $right) {
                    return 'double';
                }
                if ('double' == $left && 'long' == $right) {
                    return 'double';
                }
                if ('int' == $left && 'double' == $right) {
                    return 'double';
                }
                if ('int' == $left && 'bool' == $right) {
                    return 'int';
                }
                if ('bool' == $left && 'int' == $right) {
                    return 'bool';
                }
                if ('bool' == $left && 'double' == $right) {
                    return 'bool';
                }
                if ($left == $right) {
                    return $left;
                }

                return 'numeric';

            case 'mod':
                $left  = $this->passExpression($expression['left']);
                $right = $this->passExpression($expression['right']);
                if ('long' == $left && 'long' == $right) {
                    return 'long';
                }
                if ('ulong' == $left && 'ulong' == $right) {
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
            case 'bitwise_not':
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
            case 'require_once':
                return 'variable';

            case 'ternary':
            case 'short-ternary':
                // $right = $this->passExpression($expression['right']);
                // $extra = $this->passExpression($expression['extra']);
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

    public function passLetStatement(array $statement): void
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

    public function passNew(array $expression): void
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

                case 'throw':
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

                case 'fetch':
                    $this->passExpression($statement['expr']);
                    break;

                case 'mcall':
                case 'scall':
                case 'fcall':
                case 'require':
                case 'require_once':
                    $this->passCall($statement['expr']);
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
                    echo 'SSTI=', $statement['type'];
            }
        }
    }

    /**
     * Process the found infered types and schedule a new pass.
     *
     * @return bool
     */
    public function reduce(): bool
    {
        $pass = false;
        foreach ($this->variables as $variable => $type) {
            if ('variable' == $type || 'string' == $type || 'istring' == $type || 'array' == $type || 'null' == $type || 'numeric' == $type) {
                unset($this->variables[$variable]);
            } else {
                $pass                     = true;
                $this->infered[$variable] = $type;
            }
        }

        return $pass;
    }
}
