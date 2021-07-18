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

namespace Zephir\Operators\Other;

use Zephir\Classes\Entry;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

use function Zephir\escape_class;

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
     * @throws Exception
     *
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $context): CompiledExpression
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
                $classEntry = (new Entry($resolvedVariable, $context));
                break;
            default:
                switch ($resolved->getType()) {
                    case 'variable':
                        if ('this' == $resolvedVariable) {
                            /**
                             * TODO: It's an optimization variant, but maybe we need to get entry in runtime?
                             */
                            $classEntry = $context->classDefinition->getClassEntry($context);
                        } elseif (!$context->symbolTable->hasVariable($resolvedVariable)) {
                            $className = $context->getFullName($resolvedVariable);

                            if ('Traversable' === $className) {
                                $symbol = $context->backend->getVariableCode($symbolVariable);

                                return new CompiledExpression('bool', 'zephir_zval_is_traversable('.$symbol.')', $expression);
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
                                        $entry = (new Entry($resolvedVariable, $context));
                                        $classEntry = $entry->get();

                                        if (!$entry->isInternal()) {
                                            $code = 'SL("'.trim(escape_class($className), '\\').'")';
                                        }
                                    }
                                }
                            }
                        } else {
                            $code = $this->prepareBackendSpecificCode($resolvedVariable, $context);
                        }
                        break;

                    case 'property-access':
                    case 'array-access':
                        $context->headersManager->add('kernel/operators');
                        $tempVariable = $context->symbolTable->getTempVariableForWrite('string', $context);
                        $tempVariable->setMustInitNull(true);
                        $tempVariableName = $tempVariable->getName();
                        $context->codePrinter->output('zephir_get_strval('.$tempVariableName.', '.$resolvedVariable.');');
                        $code = $this->prepareBackendSpecificCode($tempVariableName, $context);
                        break;

                    default:
                        throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
                }
        }

        /* TODO:, Possible optimization is use zephir_is_instance when the const class name is an internal class or interface */
        $context->headersManager->add('kernel/object');
        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($code)) {
            return new CompiledExpression('bool', 'zephir_is_instance_of('.$symbol.', '.$code.')', $expression);
        }

        return new CompiledExpression('bool', 'zephir_instance_of_ev('.$symbol.', '.$classEntry.')', $expression);
    }

    private function prepareBackendSpecificCode($variable, CompilationContext $context): string
    {
        return strtr('Z_STRVAL_P(:p:name), Z_STRLEN_P(:p:name)', [
            ':name' => $variable,
            ':p' => $context->backend->isZE3() ? '&' : '',
        ]);
    }
}
