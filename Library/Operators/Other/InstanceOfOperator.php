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

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Utils;

/**
 * InstanceOf
 *
 * Checks if a variable is an instance of a class
 */
class InstanceOfOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $context
     * @return CompiledExpression
     * @throws CompilerException
     * @throws \Zephir\Exception
     */
    public function compile($expression, CompilationContext $context)
    {
        $left = new Expression($expression['left']);
        $resolved = $left->compile($context);

        if ($resolved->getType() != 'variable') {
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
                $className = Utils::getFullName($resolvedVariable, $context->classDefinition->getNamespace(), $context->aliasManager);

                if ($context->compiler->isClass($className)) {
                    $classDefinition = $context->compiler->getClassDefinition($className);
                    $classEntry = $classDefinition->getClassEntry($context);
                } else {
                    if (!class_exists($className, false)) {
                        $code = 'SL("' . $resolvedVariable . '")';
                    } else {
                        $classEntry = $context->classDefinition->getClassEntryByClassName($className, $context, true);
                        if (!$classEntry) {
                            $code = 'SL("' . $resolvedVariable . '")';
                        }
                    }
                }
                break;

            default:
                switch ($resolved->getType()) {

                    case 'variable':
                        if ($resolvedVariable == 'this') {
                            /**
                             * @todo It's an optimization variant, but maybe we need to get entry in runtime?
                             */
                            $classEntry = $context->classDefinition->getClassEntry($context);
                        } elseif (!$context->symbolTable->hasVariable($resolvedVariable)) {
                            $className = $context->getFullName($resolvedVariable);

                            if ($className == 'Traversable') {
                                return new CompiledExpression('bool', 'zephir_zval_is_traversable(' . $symbolVariable->getName() . ' TSRMLS_CC)', $expression);
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
                                        $code = 'SL("' . trim(Utils::escapeClassName($className), "\\") . '")';
                                    } else {
                                        $classEntry = $context->classDefinition->getClassEntryByClassName($className, $context, true);
                                        if (!$classEntry) {
                                            $code = 'SL("' . trim(Utils::escapeClassName($className), "\\") . '")';
                                        }
                                    }
                                }
                            }
                        } else {
                            $code = 'Z_STRVAL_P(' . $resolvedVariable . '), Z_STRLEN_P(' . $resolvedVariable . ')';
                        }
                        break;

                    case 'property-access':
                    case 'array-access':
                        $context->headersManager->add('kernel/operators');
                        $tempVariable = $context->symbolTable->getTempVariableForWrite('string', $context);
                        $tempVariable->setMustInitNull(true);
                        $tempVariableName = $tempVariable->getName();
                        $context->codePrinter->output('zephir_get_strval(' . $tempVariableName . ', ' . $resolvedVariable . ');');
                        $code = 'Z_STRVAL_P(' . $tempVariableName . '), Z_STRLEN_P(' . $tempVariableName . ')';
                        break;

                    default:
                        throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
                }
        }

        /* @TODO, Possible optimization is use zephir_is_instance when the const class name is an internal class or interface */
        $context->headersManager->add('kernel/object');
        if (isset($code)) {
            return new CompiledExpression('bool', 'zephir_is_instance_of(' . $symbolVariable->getName() . ', ' . $code . ' TSRMLS_CC)', $expression);
        }

        return new CompiledExpression('bool', 'zephir_instance_of_ev(' . $symbolVariable->getName() . ', ' . $classEntry . ' TSRMLS_CC)', $expression);
    }
}
