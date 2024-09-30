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

namespace Zephir\Operators;

use Zephir\CompilationContext;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable;

use function is_object;

abstract class AbstractOperator
{
    use VariablesTrait;

    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $literalOnly       = true;
    protected string    $operator;
    protected bool      $readOnly          = false;

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result.
     *
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param bool               $init
     *
     * @return Variable|null
     */
    public function getExpected(CompilationContext $compilationContext, array $expression, bool $init = true): ?Variable
    {
        if (!$this->expecting) {
            return $this->expectingVariable;
        }

        $symbolVariable = $this->expectingVariable;
        if (is_object($symbolVariable)) {
            if ('variable' === $symbolVariable->getType()) {
                if (!$init) {
                    return $symbolVariable;
                }
                $symbolVariable->initVariant($compilationContext);
            } else {
                if (!$this->readOnly) {
                    if (!$this->literalOnly) {
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext,
                            $expression
                        );
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                    }
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                        'variable',
                        $compilationContext
                    );
                }
            }
        } else {
            if (!$this->readOnly) {
                if (!$this->literalOnly) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                        'variable',
                        $compilationContext
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                    'variable',
                    $compilationContext
                );
            }
        }

        return $symbolVariable;
    }

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result, if a temporary variable is created it use whose body is only freed
     * on every iteration.
     *
     * @param CompilationContext $compilationContext
     * @param string             $type
     *
     * @return Variable|null
     */
    public function getExpectedComplexLiteral(
        CompilationContext $compilationContext,
        string $type = 'variable'
    ): ?Variable {
        $symbolVariable = $this->expectingVariable;

        if ($this->expecting) {
            if (is_object($symbolVariable)) {
                if ($symbolVariable->getType() === $type || 'return_value' === $symbolVariable->getName()) {
                    $symbolVariable->initVariant($compilationContext);
                } else {
                    if (!$this->readOnly) {
                        $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                            $type,
                            $compilationContext
                        );
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                            $type,
                            $compilationContext
                        );
                    }
                }
            } else {
                if (!$this->readOnly) {
                    $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                        $type,
                        $compilationContext
                    );
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                        $type,
                        $compilationContext
                    );
                }
            }
        }

        return $symbolVariable;
    }

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result. This method returns a variable that is always stored in the heap.
     *
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param bool               $init
     *
     * @return Variable|null
     */
    public function getExpectedNonLiteral(
        CompilationContext $compilationContext,
        array $expression,
        bool $init = true
    ): ?Variable {
        $symbolVariable = $this->expectingVariable;

        if (!$this->expecting) {
            return $symbolVariable;
        }

        if ($symbolVariable !== null) {
            if ('variable' === $symbolVariable->getType() && !$symbolVariable->isLocalOnly()) {
                if (!$init) {
                    return $symbolVariable;
                }

                $symbolVariable->initVariant($compilationContext);
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        return $symbolVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @return bool
     */
    public function isExpecting(): bool
    {
        return $this->expecting;
    }

    /**
     * Checks if the result of the evaluated expression is read only.
     *
     * @return bool
     */
    public function isReadOnly(): bool
    {
        return $this->readOnly;
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
     */
    public function setExpectReturn(bool $expecting, Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }
}
