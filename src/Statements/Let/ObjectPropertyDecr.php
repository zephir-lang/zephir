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
use Zephir\Variable\Variable as ZephirVariable;

use function current;

/**
 * ObjectPropertyDecr.
 *
 * Decrements an object property
 */
class ObjectPropertyDecr
{
    /**
     * Compiles obj->x--.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompilationContext $compilationContext
     * @param array              $statement
     * @param mixed              $property
     */
    public function assign(
        $variable,
        $property,
        ZephirVariable $symbolVariable,
        CompilationContext $compilationContext,
        $statement
    ): void {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is not initialized",
                $statement
            );
        }

        /*
         * Arrays must be stored in the HEAP
         */
        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is local only",
                $statement
            );
        }

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is not initialized",
                $statement
            );
        }

        /*
         * Only dynamic variables can be used as arrays
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                "Cannot use variable type: '" . $symbolVariable->getType() . "' as array",
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw new CompilerException(
                'Cannot use non-initialized variable as an object',
                $statement
            );
        }

        /*
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to increment non-object dynamic variable',
                ['non-object-update', $statement]
            );
        }

        /*
         * Check if the variable to update is defined
         */
        if ('this' == $symbolVariable->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException(
                    "Class '" . $classDefinition->getCompleteName(
                    ) . "' does not have a property called: '" . $property . "'",
                    $statement
                );
            }
        } else {
            /*
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

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException(
                            "Class '" . $classType . "' does not have a property called: '" . $property . "'",
                            $statement
                        );
                    }
                }
            }
        }

        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->codePrinter->output(
            'RETURN_ON_FAILURE(zephir_property_decr(' . $symbolVariable->getName() . ', SL("' . $property . '")));'
        );
    }
}
