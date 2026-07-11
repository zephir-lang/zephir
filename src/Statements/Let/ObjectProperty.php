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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext as Context;
use Zephir\CompiledExpression as Expression;
use Zephir\Exception as ZephirException;
use Zephir\Exception\CompilerException;
use Zephir\Exception\CompilerException as Exception;
use Zephir\Expression as ZephirExpression;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function sprintf;

/**
 * Adds/Updates an array index
 */
class ObjectProperty
{
    use VariablesTrait;

    /**
     * Compiles foo->x = {expr}.
     */
    public function assign(
        string $variable,
        ZephirVariable $symbolVariable,
        Expression $expression,
        Context $context,
        array $statement
    ): void {
        if (!$symbolVariable->isInitialized()) {
            throw new Exception(
                "Cannot mutate variable '{$variable}' because it is not initialized",
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new Exception(
                "Variable type '{$symbolVariable->getType()}' cannot be used as object",
                $statement
            );
        }

        $propertyName = $statement['property'];
        $className    = $context->classDefinition->getCompleteName();

        if (!$symbolVariable->isInitialized()) {
            throw new Exception(
                sprintf(
                    "Cannot mutate static property '%s::%s' because it is not initialized",
                    $className,
                    $propertyName
                ),
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new Exception(
                "Cannot use variable type: {$symbolVariable->getType()} as an object",
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw CompilerException::cannotUseNonInitializedVariableAsObject($statement);
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'object'])) {
            $context->logger->warning(
                'Possible attempt to update property on non-object dynamic property',
                ['non-valid-objectupdate', $statement]
            );
        }

        /**
         * Try to check if property is implemented on related object
         */
        if ('this' == $variable) {
            $classDefForCheck = $context->classDefinition;

            /**
             * If this is a closure class with an enclosing class, resolve
             * property checks against the enclosing class definition.
             * @see https://github.com/zephir-lang/zephir/issues/2497
             */
            $enclosingClassDefinition = $classDefForCheck->getEnclosingClassDefinition();
            if ($enclosingClassDefinition !== null) {
                $classDefForCheck = $enclosingClassDefinition;
            }

            if (!$classDefForCheck->hasProperty($propertyName)) {
                throw new Exception(
                    "Property '{$propertyName}' is not defined on class '{$classDefForCheck->getCompleteName()}'",
                    $statement
                );
            }
        }

        $codePrinter = $context->codePrinter;

        $context->headersManager->add('kernel/object');

        switch ($expression->getType()) {
            case 'null':
                $context->backend->updateProperty($symbolVariable, $propertyName, 'null', $context);
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':
                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;
                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;
                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                        $context->backend->assignLong($resolvedVariable, $expression->getBooleanCode(), $context);
                        $context->backend->fetchProperty(
                            $tempVariable,
                            $symbolVariable,
                            $propertyName,
                            false,
                            $context
                        );
                        $codePrinter->output(
                            $functionName . '(' . $context->backend->getVariableCode(
                                $tempVariable
                            ) . ', ' . $context->backend->getVariableCode($resolvedVariable) . ')'
                        );
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignLong($tempVariable, $expression->getBooleanCode(), $context);
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'char':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                switch ($statement['operator']) {
                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignLong(
                            $tempVariable,
                            '\'' . $expression->getBooleanCode() . '\'',
                            $context
                        );
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'double':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':
                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;
                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;
                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                        $context->backend->assignDouble($resolvedVariable, $expression->getBooleanCode(), $context);
                        $context->backend->fetchProperty(
                            $tempVariable,
                            $symbolVariable,
                            $propertyName,
                            false,
                            $context
                        );
                        $codePrinter->output(
                            $functionName . '(' . $context->backend->getVariableCode(
                                $tempVariable
                            ) . ', ' . $context->backend->getVariableCode($resolvedVariable) . ')'
                        );
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignDouble($tempVariable, $expression->getBooleanCode(), $context);
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'string':
                switch ($statement['operator']) {
                    case 'concat-assign':
                        /**
                         * Issue #2063: read the current property value first,
                         * then append the literal; otherwise the property is
                         * overwritten rather than concatenated.
                         */
                        $context->headersManager->add('kernel/operators');
                        $tempVariable = $this->fetchPropertyValue($variable, $propertyName, $context);
                        $codePrinter->output(sprintf(
                            'zephir_concat_self_str(%s, SL("%s"));',
                            $context->backend->getVariableCode($tempVariable),
                            $expression->getCode()
                        ));
                        break;
                    case 'assign':
                        /* We only can use nonReferenced variables for not refcounted stuff in ZE3 */
                        $tempVariable = $context->symbolTable->getTempVariableForWrite('variable', $context, false);
                        $tempVariable->initVariant($context);
                        $context->backend->assignString($tempVariable, $expression->getCode(), $context);
                        break;
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'array':
                $context->backend->updateProperty($symbolVariable, $propertyName, $expression, $context);
                break;

            case 'bool':
                $codePrinter->output('if (' . $expression->getBooleanCode() . ') {');
                $codePrinter->increaseLevel();
                $context->backend->updateProperty($symbolVariable, $propertyName, 'true', $context);
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $context->backend->updateProperty($symbolVariable, $propertyName, 'false', $context);
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
                break;

            /* unreachable code */
            case 'empty-array':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);

                $context->backend->initArray($tempVariable, $context);
                $context->backend->updateProperty($symbolVariable, $propertyName, $expression->getCode(), $context);
                $tempVariable->setIdle(true);
                break;

            case 'variable':
                $variableVariable = $context->symbolTable->getVariableForRead(
                    $expression->getCode(),
                    $context,
                    $statement
                );
                switch ($variableVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context, true);
                        if (null !== ($assignMacro = $this->arithmeticAssignMacro($statement['operator']))) {
                            $context->headersManager->add('kernel/operators');
                            $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                            $context->backend->assignLong($resolvedVariable, $variableVariable, $context);
                            $context->backend->fetchProperty($tempVariable, $symbolVariable, $propertyName, false, $context);
                            $codePrinter->output(sprintf(
                                '%s(%s, %s)',
                                $assignMacro,
                                $context->backend->getVariableCode($tempVariable),
                                $context->backend->getVariableCode($resolvedVariable)
                            ));
                        } else {
                            $context->backend->assignLong($tempVariable, $variableVariable, $context);
                        }
                        $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                        $tempVariable->setIdle(true);
                        break;

                    case 'double':
                        $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context, true);
                        if (null !== ($assignMacro = $this->arithmeticAssignMacro($statement['operator']))) {
                            $context->headersManager->add('kernel/operators');
                            $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                            $context->backend->assignDouble($resolvedVariable, $variableVariable, $context);
                            $context->backend->fetchProperty($tempVariable, $symbolVariable, $propertyName, false, $context);
                            $codePrinter->output(sprintf(
                                '%s(%s, %s)',
                                $assignMacro,
                                $context->backend->getVariableCode($tempVariable),
                                $context->backend->getVariableCode($resolvedVariable)
                            ));
                        } else {
                            $context->backend->assignDouble($tempVariable, $variableVariable, $context);
                        }
                        $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                        $tempVariable->setIdle(true);
                        break;

                    case 'bool':
                        $codePrinter->output('if (' . $variableVariable->getName() . ') {');
                        $codePrinter->increaseLevel();
                        $context->backend->updateProperty($symbolVariable, $propertyName, 'true', $context);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $context->backend->updateProperty($symbolVariable, $propertyName, 'false', $context);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                        break;

                    case 'array':
                    case 'string':
                    case 'variable':
                    case 'mixed':
                        /**
                         * Issue #2063: concat-assign (.=) must read the current
                         * property value and append to it, otherwise the
                         * property is simply overwritten.
                         */
                        if ('concat-assign' === $statement['operator']) {
                            $tempVariable = $this->fetchPropertyValue($variable, $propertyName, $context);
                            $rhsCode      = $context->backend->getVariableCode($variableVariable);
                            $tempCode     = $context->backend->getVariableCode($tempVariable);
                            $codePrinter->output('SEPARATE_ZVAL(' . $rhsCode . ');');
                            $codePrinter->output(
                                'zephir_concat_function(' . $rhsCode . ', ' . $tempCode . ', ' . $rhsCode . ');'
                            );
                            $context->backend->updateProperty($symbolVariable, $propertyName, $variableVariable, $context);
                        } elseif (null !== ($assignMacro = $this->arithmeticAssignMacro($statement['operator']))) {
                            /**
                             * Arithmetic compound-assign (+=, -=, *=) with a
                             * variable operand: read the current property, apply
                             * the operator, then write it back. Without this the
                             * read was dropped and the property was overwritten
                             * with the right-hand side (arithmetic sibling of the
                             * #2063 concat-assign bug).
                             */
                            $context->headersManager->add('kernel/operators');
                            $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                            $context->backend->fetchProperty($tempVariable, $symbolVariable, $propertyName, false, $context);
                            $codePrinter->output(sprintf(
                                '%s(%s, %s)',
                                $assignMacro,
                                $context->backend->getVariableCode($tempVariable),
                                $context->backend->getVariableCode($variableVariable)
                            ));
                            $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                            $tempVariable->setIdle(true);
                        } else {
                            $context->backend->updateProperty($symbolVariable, $propertyName, $variableVariable, $context);
                        }
                        $this->checkVariableTemporal($symbolVariable);
                        break;

                    default:
                        throw new Exception('Unknown type ' . $variableVariable->getType(), $statement);
                }
                break;

            default:
                throw new Exception("Unknown type {$expression->getType()}", $statement);
        }
    }

    /**
     * Maps an arithmetic compound-assignment operator to its kernel macro, or
     * null when the operator is not an arithmetic compound-assign. Shared by
     * the variable-operand object-property assign paths so `+= $var` / `-= $var`
     * / `*= $var` read-modify-write instead of silently overwriting the
     * property with the right-hand side (the arithmetic equivalent of the
     * concat-assign bug fixed in issue #2063).
     */
    private function arithmeticAssignMacro(string $operator): ?string
    {
        return match ($operator) {
            'add-assign' => 'ZEPHIR_ADD_ASSIGN',
            'sub-assign' => 'ZEPHIR_SUB_ASSIGN',
            'mul-assign' => 'ZEPHIR_MUL_ASSIGN',
            default      => null,
        };
    }

    /**
     * Reads the current value of an object property into a fresh temp variable
     * (used to support read-modify-write operators such as concat-assign).
     */
    private function fetchPropertyValue(
        string $variable,
        string $propertyName,
        Context $context
    ): ZephirVariable {
        $tempVariable = $context->symbolTable->getTempVariableForObserveOrNullify('variable', $context);

        $expression = new ZephirExpression([
            'type'  => 'property-access',
            'left'  => ['type' => 'variable', 'value' => $variable],
            'right' => ['value' => $propertyName],
        ]);
        $expression->setExpectReturn(true, $tempVariable);

        try {
            $expression->compile($context);
        } catch (ZephirException $e) {
            throw new CompilerException($e->getMessage(), [], $e->getCode(), $e);
        }

        return $tempVariable;
    }
}
