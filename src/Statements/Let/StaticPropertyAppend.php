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

use ReflectionException;
use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;

use function in_array;

/**
 * StaticPropertyAppend.
 *
 * Updates object properties dynamically
 */
class StaticPropertyAppend extends ArrayIndex
{
    /**
     * Compiles ClassName::foo[index] = {expr}.
     *
     * @param string             $className
     * @param string             $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws Exception
     * @throws CompilerException
     */
    public function assignStatic(
        $className,
        $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $compiler = $compilationContext->compiler;
        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className)) {
                $classDefinition = $compiler->getClassDefinition($className);
            } else {
                if ($compiler->isBundledClass($className)) {
                    $classDefinition = $compiler->getInternalClassDefinition($className);
                } else {
                    throw new CompilerException("Cannot locate class '" . $className . "'", $statement);
                }
            }
        } else {
            if (in_array($className, ['self', 'static'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' == $className) {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass    = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException(
                            'Cannot assign static property "'
                            . $property
                            . '" on parent because class '
                            . $classDefinition->getCompleteName()
                            . ' does not extend any class',
                            $statement
                        );
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
        }

        $this->checkClassHasProperty(
            $classDefinition,
            $property,
            $statement
        );

        /** @var Property $propertyDefinition */
        $propertyDefinition = $classDefinition->getProperty($property);
        $this->checkAccessNonStaticProperty(
            $propertyDefinition,
            $classDefinition,
            $property,
            $statement
        );

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition != $compilationContext->classDefinition) {
                throw new CompilerException(
                    "Cannot access private static property '"
                    . $classDefinition->getCompleteName()
                    . '::'
                    . $property
                    . "' out of its declaring context",
                    $statement
                );
            }
        }

        $compilationContext->headersManager->add('kernel/object');
        $classEntry = $classDefinition->getClassEntry($compilationContext);
        $this->_assignStaticPropertyArrayMultipleIndex(
            $classEntry,
            $property,
            $resolvedExpr,
            $compilationContext,
            $statement
        );
    }

    /**
     * Compiles x::y[a][b][] = {expr} (multiple offset assignment).
     *
     * @param string             $classEntry
     * @param string             $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     */
    protected function _assignStaticPropertyArrayMultipleIndex(
        $classEntry,
        string $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed).
         */
        $variableExpr      = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);
        $offsetExpressions = $this->getOffsetExpressions($statement, $compilationContext);

        $compilationContext->backend->assignStaticPropertyArrayMulti(
            $classEntry,
            $variableExpr,
            $property,
            $offsetExpressions,
            $compilationContext
        );

        $this->checkVariableTemporal($variableExpr);
    }

    /**
     * @param array              $statement
     * @param CompilationContext $compilationContext
     *
     * @return array
     * @throws ReflectionException
     * @throws Exception
     */
    protected function getOffsetExpressions(
        array $statement,
        CompilationContext $compilationContext
    ): array {
        $offsetExpressions[] = 'a';

        return $offsetExpressions;
    }
}
