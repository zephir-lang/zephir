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

namespace Zephir\Types;

use ReflectionException;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;

abstract class AbstractType
{
    /**
     * The array of methods in zephir mapped to PHP internal methods.
     */
    protected array $methodMap = [];

    /**
     * Intercepts calls to built-in methods.
     *
     * @param string $methodName
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     *
     * @return bool|CompiledExpression
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function invokeMethod(
        $methodName,
        $caller,
        CompilationContext $compilationContext,
        Call $call,
        array $expression
    ) {
        /**
         * Checks first whether the method exist in the array type definition
         */
        if (method_exists($this, $methodName)) {
            return $this->{$methodName}($caller, $compilationContext, $call, $expression);
        }

        /**
         * Check the method map
         */
        if (isset($this->methodMap[$methodName])) {
            $paramNumber = $this->getNumberParam($methodName);
            if (0 === $paramNumber) {
                if (isset($expression['parameters'])) {
                    $parameters = $expression['parameters'];
                    array_unshift($parameters, ['parameter' => $caller]);
                } else {
                    $parameters = [['parameter' => $caller]];
                }
            } else {
                if (isset($expression['parameters'])) {
                    $parameters = [];
                    foreach ($expression['parameters'] as $number => $parameter) {
                        if ($number === $paramNumber) {
                            $parameters[] = null;
                        }
                        $parameters[] = $parameter;
                    }
                    $parameters[$paramNumber] = ['parameter' => $caller];
                } else {
                    $parameters = [['parameter' => $caller]];
                }
            }

            $functionCall = BuilderFactory::getInstance()->statements()
                ->functionCall($this->methodMap[$methodName], $parameters)
                ->setFile($expression['file'])
                ->setLine($expression['line'])
                ->setChar($expression['char']);

            $expression = new Expression($functionCall->build());

            return $expression->compile($compilationContext);
        }

        throw new CompilerException(
            sprintf('Method "%s" is not a built-in method of type "%s"', $methodName, $this->getTypeName()),
            $expression
        );
    }

    /**
     * Get the name of the type.
     *
     * @return string
     */
    abstract public function getTypeName(): string;

    public function getMethodMap(): array
    {
        return $this->methodMap;
    }

    /**
     * Returns the number of the parameter where the object must be bound.
     */
    protected function getNumberParam(string $methodName): int
    {
        return 0;
    }
}
