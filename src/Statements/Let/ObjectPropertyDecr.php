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

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function current;

/**
 * Decrements an object property
 */
class ObjectPropertyDecr
{
    use VariablesTrait;

    protected string $zephirMethod = 'zephir_property_decr';

    /**
     * Compiles obj->x++/obj->x--.
     */
    public function assign(
        string $variable,
        string $property,
        ZephirVariable $symbolVariable,
        CompilationContext $compilationContext,
        array $statement,
    ): void {
        $this->checkVariableInitialized($variable, $symbolVariable, $statement);

        /**
         * Arrays must be stored in the HEAP
         */
        $this->checkVariableLocalOnly($variable, $symbolVariable, $statement);

        /**
         * Only dynamic variables can be used as arrays
         */
        if (!$symbolVariable->isVariable()) {
            throw CompilerException::cannotUseAsArray(
                $symbolVariable->getType(),
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw CompilerException::cannotUseNonInitializedVariableAsObject($statement);
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to increment non-object dynamic variable',
                ['non-object-update', $statement]
            );
        }

        /**
         * Check if the variable to update is defined
         */
        if ('this' === $symbolVariable->getRealName()) {
            $this->checkClassHasProperty($compilationContext->classDefinition, $property, $statement);
        } else {
            /**
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($symbolVariable->hasAnyDynamicType('object')) {
                $classType = current($symbolVariable->getClassTypes());
                $compiler  = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException(
                            'Cannot locate class definition for class: ' . $classType,
                            $statement
                        );
                    }

                    $this->checkClassHasProperty(
                        $classDefinition,
                        $property,
                        $statement,
                        $classType
                    );
                }
            }
        }

        $symbolVariable->setUsed(true);
        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->codePrinter->output(
            'RETURN_ON_FAILURE(' . $this->zephirMethod . '(' . $symbolVariable->getName(
            ) . ', SL("' . $property . '")));'
        );
    }
}
