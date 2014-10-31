<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
     *
     * @param array $expression
     * @param \CompilationContext $compilationContext
     * @return \CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {

        $left = new Expression($expression['left']);
        $resolved = $left->compile($compilationContext);

        if ($resolved->getType() != 'variable') {
            throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
        }

        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
        }

        $right = new Expression($expression['right']);
        $resolved = $right->compile($compilationContext);
        $resolvedVariable = $resolved->getCode();

        switch ($resolved->getType()) {

            case 'string':
                $className = Utils::getFullName($resolvedVariable, $compilationContext->classDefinition->getNamespace(), $compilationContext->aliasManager);
                if ($compilationContext->compiler->isClass($className)) {
                    $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                    $classEntry = $classDefinition->getClassEntry($compilationContext);
                } else {
                    if (!class_exists($className, false)) {
                        $code = 'SL("' . $resolvedVariable . '")';
                    } else {
                        $classEntry = $compilationContext->classDefinition->getClassEntryByClassName($className, $compilationContext, true);
                        if (!$classEntry) {
                            $code = 'SL("' . $resolvedVariable . '")';
                        }
                    }
                }
                break;

            default:
                switch ($resolved->getType()) {

                    case 'variable':
                        if (!$compilationContext->symbolTable->hasVariable($resolvedVariable)) {
                            $className = $compilationContext->getFullName($resolvedVariable);
                            if ($compilationContext->compiler->isClass($className)) {
                                $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                                $classEntry = $classDefinition->getClassEntry($compilationContext);
                            } else {
                                if ($compilationContext->compiler->isInterface($className)) {
                                    $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                                    $classEntry = $classDefinition->getClassEntry($compilationContext);
                                } else {
                                    if (!class_exists($className, false)) {
                                        $code = 'SL("' . trim(Utils::escapeClassName($className), "\\") . '")';
                                    } else {
                                        $classEntry = $compilationContext->classDefinition->getClassEntryByClassName($className, $compilationContext, true);
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
                        $compilationContext->headersManager->add('kernel/operators');
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('string', $compilationContext);
                        $tempVariable->setMustInitNull(true);
                        $tempVariableName = $tempVariable->getName();
                        $compilationContext->codePrinter->output('zephir_get_strval(' . $tempVariableName . ', ' . $resolvedVariable . ');');
                        $code = 'Z_STRVAL_P(' . $tempVariableName . '), Z_STRLEN_P(' . $tempVariableName . ')';
                        break;

                    default:
                        throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
                }
        }

        /* @TODO, Possible optimization is use zephir_is_instance when the const class name is an internal class or interface */
        $compilationContext->headersManager->add('kernel/object');
        if (isset($code)) {
            return new CompiledExpression('bool', 'zephir_is_instance_of(' . $symbolVariable->getName() . ', ' . $code . ' TSRMLS_CC)', $expression);
        }

        return new CompiledExpression('bool', 'zephir_instance_of_ev(' . $symbolVariable->getName() . ', ' . $classEntry . ' TSRMLS_CC)', $expression);
    }
}
