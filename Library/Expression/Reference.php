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

namespace Zephir\Expression;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable;

/**
 * Expression Reference
 *
 * Resolves expressions that create arrays.
 */
class Reference
{
    /**
     * Expecting the value
     */
    protected bool $expecting = true;

    /**
     * Result of the evaluated expression is read only or not
     */
    protected bool $readOnly = false;

    /**
     * Expecting variable
     */
    protected ?Variable $expectingVariable = null;

    private array $validTypes = [
        'variable',
        'string',
        'object',
        'array',
        'callable',
    ];

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting = $expecting;
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

    /**
     * Compiles a reference to a value.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        /**
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('variable' !== $symbolVariable->getType()) {
                    throw new CompilerException('Cannot use variable type: '.$symbolVariable->getType().' to store a reference', $expression);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        if (!in_array($left->getType(), $this->validTypes)) {
            throw new CompilerException('Cannot obtain a reference from type: '.$left->getType(), $expression);
        }

        $leftVariable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression);
        if (!in_array($leftVariable->getType(), $this->validTypes)) {
            throw new CompilerException('Cannot obtain reference from variable type: '.$leftVariable->getType(), $expression);
        }

        $symbolVariable->setMustInitNull(true);
        $compilationContext->symbolTable->mustGrownStack(true);

        $symbolVariable->increaseVariantIfNull();
        $compilationContext->codePrinter->output('ZEPHIR_MAKE_REFERENCE('.$symbolVariable->getName().', '.$leftVariable->getName().');');

        return new CompiledExpression('reference', $symbolVariable->getRealName(), $expression);
    }
}
