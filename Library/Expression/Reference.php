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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\GlobalConstant;
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
     *
     * @var bool
     */
    protected bool $expecting = true;

    /**
     * Result of the evaluated expression is read only or not
     *
     * @var bool
     */
    protected bool $readOnly = false;

    /**
     * Expecting variable
     *
     * @var Variable|null
     */
    protected ?Variable $expectingVariable = null;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null)
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
     * Resolves an item to be added in an array.
     *
     * @param CompiledExpression $exprCompiled
     * @param CompilationContext $compilationContext
     *
     * @return GlobalConstant|Variable
     *
     * @throws Exception
     */
    public function getArrayValue(CompiledExpression $exprCompiled, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        switch ($exprCompiled->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_LONG('.$tempVar->getName().', '.$exprCompiled->getCode().');');

                return $tempVar;

            case 'char':
            case 'uchar':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_LONG('.$tempVar->getName().', \''.$exprCompiled->getCode().'\');');

                return $tempVar;

            case 'double':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_DOUBLE('.$tempVar->getName().', '.$exprCompiled->getCode().');');

                return $tempVar;

            case 'bool':
                if ('true' === $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                }

                if ('false' === $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                }

                throw new Exception('?');
                break;

            case 'null':
                return new GlobalConstant('ZEPHIR_GLOBAL(global_null)');

            case 'string':
            case 'ulong':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_STRING('.$tempVar->getName().', "'.$exprCompiled->getCode().'", 1);');

                return $tempVar;

            case 'array':
                return $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG('.$tempVar->getName().', '.$itemVariable->getName().');');

                        return $tempVar;

                    case 'double':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_DOUBLE('.$tempVar->getName().', '.$itemVariable->getName().');');

                        return $tempVar;

                    case 'bool':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_BOOL('.$tempVar->getName().', '.$itemVariable->getName().');');

                        return $tempVar;

                    case 'string':
                    case 'variable':
                    case 'array':
                        return $itemVariable;

                    default:
                        throw new CompilerException('Unknown '.$itemVariable->getType(), $itemVariable);
                }
                break;

            default:
                throw new CompilerException('Unknown', $exprCompiled);
        }
    }

    /**
     * Compiles a reference to a value.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
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

        $validTypes = [
            'variable',
            'string',
            'object',
            'array',
            'callable',
        ];

        if (!in_array($left->getType(), $validTypes)) {
            throw new CompilerException('Cannot obtain a reference from type: '.$left->getType(), $expression);
        }

        $leftVariable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression);
        if (!in_array($leftVariable->getType(), $validTypes)) {
            throw new CompilerException('Cannot obtain reference from variable type: '.$leftVariable->getType(), $expression);
        }

        $symbolVariable->setMustInitNull(true);
        $compilationContext->symbolTable->mustGrownStack(true);

        $symbolVariable->increaseVariantIfNull();
        $compilationContext->codePrinter->output('ZEPHIR_MAKE_REFERENCE('.$symbolVariable->getName().', '.$leftVariable->getName().');');

        return new CompiledExpression('reference', $symbolVariable->getRealName(), $expression);
    }
}
