<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use function Zephir\escape_class;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use function Zephir\fqcn;
use Zephir\Operators\BaseOperator;

/**
 * InstanceOf.
 *
 * Checks if a variable is an instance of a class
 */
class InstanceOfOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $context
     *
     * @throws CompilerException
     * @throws \Zephir\Exception
     *
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $context)
    {
        $left = new Expression($expression['left']);
        $resolved = $left->compile($context);

        if ('variable' != $resolved->getType()) {
            throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
        }

        $symbolVariable = $context->symbolTable->getVariableForRead($resolved->getCode(), $context, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
        }

        $right = new Expression($expression['right']);
        $resolved = $right->compile($context);
        $resolvedVariable = $resolved->getCode();

        switch ($resolved->getType()) {
            case 'string':
                $className = fqcn($resolvedVariable, $context->classDefinition->getNamespace(), $context->aliasManager);

                if ($context->compiler->isClass($className)) {
                    $classDefinition = $context->compiler->getClassDefinition($className);
                    $classEntry = $classDefinition->getClassEntry($context);
                } else {
                    if (!class_exists($className, false)) {
                        $code = 'SL("'.$resolvedVariable.'")';
                    } else {
                        $classEntry = $context->classDefinition->getClassEntryByClassName($className, $context, true);
                        if (!$classEntry) {
                            $code = 'SL("'.$resolvedVariable.'")';
                        }
                    }
                }
                break;
            default:
                switch ($resolved->getType()) {
                    case 'variable':
                        if ('this' == $resolvedVariable) {
                            /**
                             * @todo It's an optimization variant, but maybe we need to get entry in runtime?
                             */
                            $classEntry = $context->classDefinition->getClassEntry($context);
                        } elseif (!$context->symbolTable->hasVariable($resolvedVariable)) {
                            $className = $context->getFullName($resolvedVariable);

                            if ('Traversable' == $className) {
                                $symbol = $context->backend->getVariableCode($symbolVariable);

                                return new CompiledExpression('bool', 'zephir_zval_is_traversable('.$symbol.' TSRMLS_CC)', $expression);
                            }

                            if ($context->compiler->isClass($className)) {
                                $classDefinition = $context->compiler->getClassDefinition($className);
                                $classEntry = $classDefinition->getClassEntry($context);
                            } else {
                                if ($context->compiler->isInterface($className)) {
                                    $classDefinition = $context->compiler->getClassDefinition($className);
                                    $classEntry = $classDefinition->getClassEntry($context);
                                } else {
                                    if (!class_exists($className, false)) {
                                        $code = 'SL("'.trim(escape_class($className), '\\').'")';
                                    } else {
                                        $classEntry = $context->classDefinition->getClassEntryByClassName($className, $context, true);
                                        if (!$classEntry) {
                                            $code = 'SL("'.trim(escape_class($className), '\\').'")';
                                        }
                                    }
                                }
                            }
                        } else {
                            $code = 'Z_STRVAL_P(&'.$resolvedVariable.'), Z_STRLEN_P(&'.$resolvedVariable.')';
                        }
                        break;

                    case 'property-access':
                    case 'array-access':
                        $context->headersManager->add('kernel/operators');
                        $tempVariable = $context->symbolTable->getTempVariableForWrite('string', $context);
                        $tempVariable->setMustInitNull(true);
                        $tempVariableName = $tempVariable->getName();
                        $context->codePrinter->output('zephir_get_strval('.$tempVariableName.', '.$resolvedVariable.');');
                        $code = 'Z_STRVAL_P(&'.$tempVariableName.'), Z_STRLEN_P(&'.$tempVariableName.')';
                        break;

                    default:
                        throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
                }
        }

        /* @TODO, Possible optimization is use zephir_is_instance when the const class name is an internal class or interface */
        $context->headersManager->add('kernel/object');
        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($code)) {
            return new CompiledExpression('bool', 'zephir_is_instance_of('.$symbol.', '.$code.' TSRMLS_CC)', $expression);
        }

        return new CompiledExpression('bool', 'zephir_instance_of_ev('.$symbol.', '.$classEntry.' TSRMLS_CC)', $expression);
    }
}
