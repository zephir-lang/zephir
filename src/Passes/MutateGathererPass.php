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

use const PHP_EOL;

/**
 * Counts variables mutated inside a specific statement block
 * Non-mutated variables can be promoted to use an inline cache
 */
class MutateGathererPass
{
    protected array $mutations = [];
    protected array $variables = [];

    public function declareVariables(array $statement): void
    {
        foreach ($statement['variables'] as $variable) {
            if (!isset($this->variables[$variable['variable']])) {
                $this->variables[$variable['variable']] = true;
            }
        }
    }

    /**
     * Returns the number of assignment instructions that mutated a variable.
     *
     * @param string $variable
     *
     * @return int
     */
    public function getNumberOfMutations(string $variable): int
    {
        return $this->mutations[$variable] ?? 0;
    }

    /**
     * Increase the number of mutations a variable has inside a statement block.
     *
     * @param string $variable
     *
     * @return MutateGathererPass
     */
    public function increaseMutations(string $variable): static
    {
        if (isset($this->mutations[$variable])) {
            ++$this->mutations[$variable];
        } else {
            $this->mutations[$variable] = 1;
        }

        return $this;
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

    /**
     * Pass array expressions.
     *
     * @param array $expression
     */
    public function passArray(array $expression): void
    {
        foreach ($expression['left'] as $item) {
            if ('variable' != $item['value']['type']) {
                $this->passExpression($item['value']);
            }
        }
    }

    /**
     * Pass call expressions.
     *
     * @param array $expression
     */
    public function passCall(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if ('variable' == $parameter['parameter']['type']) {
                    $this->increaseMutations($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass expressions.
     *
     * @param array $expression
     */
    public function passExpression(array $expression): void
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
            case 'irange':
            case 'erange':
                $this->passExpression($expression['left']);
                $this->passExpression($expression['right']);
                break;

            case 'minus':
            case 'not':
            case 'static-property-access':
            case 'array-access':
            case 'property-string-access':
            case 'property-dynamic-access':
            case 'property-access':
            case 'list':
            case 'typeof':
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
                $this->passNew($expression);
                break;

            case 'isset':
            case 'empty':
            case 'instanceof':
            case 'require':
            case 'require_once':
            case 'clone':
            case 'likely':
            case 'unlikely':
            case 'ternary':
                /* do special pass later */
                $this->passExpression($expression['left']);
                break;

            case 'fetch':
                $this->increaseMutations($expression['left']['value']);
                $this->passExpression($expression['right']);
                break;

            case 'cast':
            case 'type-hint':
                $this->passExpression($expression['right']);
                break;

            default:
                echo 'MutateGathererPassType=', $expression['type'], PHP_EOL;
                break;
        }
    }

    /**
     * Pass let statements.
     *
     * @param array $statement
     */
    public function passLetStatement(array $statement): void
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
            $this->increaseMutations($assignment['variable']);
        }
    }

    /**
     * Pass "new" expressions.
     *
     * @param array $expression
     */
    public function passNew(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if ('variable' != $parameter['parameter']['type']) {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass statement block.
     *
     * @param array $statements
     */
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
                    if (isset($statement['elseif_statements'])) {
                        $this->passStatementBlock($statement['elseif_statements']);
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

                case 'throw':
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

                case 'unset':
                    if ('array-access' == $statement['expr']['type']) {
                        if ('variable' == $statement['expr']['left']['type']) {
                            $this->increaseMutations($statement['expr']['left']['value']);
                        }
                    } else {
                        if ('list' == $statement['expr']['type']) {
                            if ('array-access' == $statement['expr']['left']['type']) {
                                if ('variable' == $statement['expr']['left']['left']['type']) {
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
                    echo 'MutateGathererPassStatement=', $statement['type'], PHP_EOL;
            }
        }
    }
}
