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

namespace Zephir\Traits;

use Exception;
use Zephir\Class\Definition\AbstractDefinition;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Property;
use Zephir\Exception\CompilerException;
use Zephir\GlobalConstant;
use Zephir\Variable\Variable;
use Zephir\Variable\Variable as ZephirVariable;

trait VariablesTrait
{
    /**
     * Check whether a class has a property
     *
     * @param Definition $classDefinition
     * @param string     $property
     * @param array      $statement
     *
     * @return void
     */
    protected function checkAccessNonStaticProperty(
        Property $propertyDefinition,
        AbstractDefinition $classDefinition,
        string $property,
        array $statement
    ): void {
        if (!$propertyDefinition->isStatic()) {
            throw new CompilerException(
                "Cannot access non-static property '"
                . $classDefinition->getCompleteName()
                . '::'
                . $property
                . "'",
                $statement
            );
        }
    }

    /**
     * Check whether a class has a property
     *
     * @param Definition $classDefinition
     * @param string     $property
     * @param array      $statement
     *
     * @return void
     */
    protected function checkClassHasProperty(
        Definition $classDefinition,
        string $property,
        array $statement,
        string $className = ''
    ): void {
        if (!$classDefinition->hasProperty($property)) {
            throw CompilerException::classDoesNotHaveProperty(
                (empty($className) ? $classDefinition->getCompleteName() : $className),
                $property,
                $statement
            );
        }
    }

    /**
     * @param array      $expression
     * @param string     $exception
     * @param array|null $extra
     *
     * @return void
     */
    protected function checkLeft(
        array $expression,
        string $exception = Exception::class,
        ?array $extra = null
    ): void {
        if (!isset($expression['left'])) {
            throw new $exception('Missing left part of the expression', $extra);
        }
    }

    /**
     * @param Variable|null $variable
     * @param array         $expression
     *
     * @return void
     */
    protected function checkNotVariable(
        ?Variable $variable,
        array $expression
    ): void {
        if (null !== $variable && $variable->isNotVariable()) {
            throw CompilerException::returnValuesVariantVars($expression);
        }
    }

    /**
     * @param Variable|null $variable
     * @param array         $expression
     *
     * @return void
     */
    protected function checkNotVariableString(
        ?Variable $variable,
        array $expression
    ): void {
        if (null !== $variable && $variable->isNotVariableAndString()) {
            throw CompilerException::returnValuesVariantVars($expression);
        }
    }

    /**
     * @param array      $expression
     * @param string     $exception
     * @param array|null $extra
     *
     * @return void
     */
    protected function checkRight(
        array $expression,
        string $exception = Exception::class,
        ?array $extra = null
    ): void {
        if (!isset($expression['right'])) {
            throw new $exception('Missing right part of the expression', $extra);
        }
    }

    /**
     * @param string         $variable
     * @param ZephirVariable $symbolVariable
     * @param array          $statement
     *
     * @return void
     */
    protected function checkVariableInitialized(
        string $variable,
        ZephirVariable $symbolVariable,
        array $statement
    ): void {
        if (true !== $symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is not initialized",
                $statement
            );
        }
    }

    /**
     * @param string         $variable
     * @param ZephirVariable $symbolVariable
     * @param array          $statement
     *
     * @return void
     */
    protected function checkVariableLocalOnly(
        string $variable,
        ZephirVariable $symbolVariable,
        array $statement
    ): void {
        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is local only",
                $statement
            );
        }
    }

    /**
     * @param string         $variable
     * @param ZephirVariable $symbolVariable
     * @param array          $statement
     *
     * @return void
     */
    protected function checkVariableReadOnly(
        string $variable,
        ZephirVariable $symbolVariable,
        array $statement
    ): void {
        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is read only",
                $statement
            );
        }
    }

    /**
     * @param GlobalConstant|ZephirVariable $variable
     *
     * @return void
     */
    protected function checkVariableTemporal(GlobalConstant|Variable $variable): void
    {
        if ($variable->isTemporal()) {
            $variable->setIdle(true);
        }
    }
}
