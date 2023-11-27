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

namespace Zephir\Detectors;

/**
 * Detects whether a variable is mutated in a given context
 * If a variable is not modified in a local context (method block) we can avoid allocate
 * memory for its body (zvalue)
 *
 * Separate parameters to avoid them to be touched by modifying its reference count
 */
class WriteDetector
{
    public const DETECT_ALL                 = 255;
    public const DETECT_ARRAY_USE           = 2;
    public const DETECT_NONE                = 0;
    public const DETECT_PARAM_PASS          = 1;
    public const DETECT_VALUE_IN_ASSIGNMENT = 4;
    protected int $detectionFlags = 0;

    protected array $mutations = [];

    /**
     * Pass "declare" statement.
     */
    public function declareVariables(array $statement): void
    {
        if (isset($statement['data-type'])) {
            if ('variable' != $statement['data-type']) {
                return;
            }
        }

        foreach ($statement['variables'] as $variable) {
            if (isset($variable['expr'])) {
                if ('string' == $variable['expr']['type'] || 'empty-array' == $variable['expr']['type'] || 'array' == $variable['expr']['type']) {
                    continue;
                }
            }

            $this->increaseMutations($variable['variable']);
        }
    }

    /**
     * Do the detection pass on a single variable.
     */
    public function detect(string $variable, array $statements): bool
    {
        $this->passStatementBlock($statements);

        return $this->getNumberOfMutations($variable) > 0;
    }

    /**
     * Returns the number of assignment instructions that mutated a variable.
     */
    public function getNumberOfMutations(string $variable): int
    {
        return $this->mutations[$variable] ?? 0;
    }

    /**
     * Increase the number of mutations a variable has inside a statement block.
     */
    public function increaseMutations(string $variable): self
    {
        if (isset($this->mutations[$variable])) {
            ++$this->mutations[$variable];
        } else {
            $this->mutations[$variable] = 1;
        }

        return $this;
    }

    /**
     * Pass array expressions.
     */
    public function passArray(array $expression): void
    {
        foreach ($expression['left'] as $item) {
            $usePass = self::DETECT_ARRAY_USE == ($this->detectionFlags & self::DETECT_ARRAY_USE);
            if ($usePass && 'variable' == $item['value']['type']) {
                $this->increaseMutations($item['value']['value']);
            } else {
                $this->passExpression($item['value']);
            }
        }
    }

    /**
     * Pass call expressions.
     */
    public function passCall(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $usePass = self::DETECT_PARAM_PASS == ($this->detectionFlags & self::DETECT_PARAM_PASS);
                if ($usePass && 'variable' == $parameter['parameter']['type']) {
                    $this->increaseMutations($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass expressions.
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
            case 'minus':
            case 'list':
            case 'array-access':
            case 'static-property-access':
            case 'property-string-access':
            case 'property-dynamic-access':
            case 'property-access':
            case 'ternary':
            case 'unlikely':
            case 'likely':
            case 'clone':
            case 'require_once':
            case 'require':
            case 'instanceof':
            case 'empty':
            case 'isset':
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

            case 'fetch':
                $this->increaseMutations($expression['left']['value']);
                $this->passExpression($expression['right']);
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
     * Pass let statements.
     */
    public function passLetStatement(array $statement): void
    {
        foreach ($statement['assignments'] as $assignment) {
            if (isset($assignment['expr'])) {
                $this->passExpression($assignment['expr']);
            }
            $this->increaseMutations($assignment['variable']);
            if (self::DETECT_VALUE_IN_ASSIGNMENT == ($this->detectionFlags & self::DETECT_VALUE_IN_ASSIGNMENT)) {
                if (isset($assignment['expr'])) {
                    if ('variable' == $assignment['expr']['type']) {
                        $this->increaseMutations($assignment['expr']['value']);
                        break;
                    }
                }
            }
        }
    }

    /**
     * Pass "new" expressions.
     */
    public function passNew(array $expression): void
    {
        if (isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                $usePass = self::DETECT_PARAM_PASS == ($this->detectionFlags & self::DETECT_PARAM_PASS);
                if ($usePass && 'variable' == $parameter['parameter']['type']) {
                    $this->increaseMutations($parameter['parameter']['value']);
                } else {
                    $this->passExpression($parameter['parameter']);
                }
            }
        }
    }

    /**
     * Pass statement block.
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
                    echo 'WriteDetectorStatement=', $statement['type'], PHP_EOL;
            }
        }
    }

    /**
     * Sets detection flags.
     */
    public function setDetectionFlags(int $flags): void
    {
        $this->detectionFlags = $flags;
    }
}
