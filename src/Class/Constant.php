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

namespace Zephir\Class;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression\ConstantExpressionEvaluator;
use Zephir\Expression\Constants;
use Zephir\Expression\StaticConstantAccess;

use function in_array;
use function sprintf;

/**
 * Represents a class constant
 */
class Constant
{
    /** Guards against a constant whose initializer reads itself. */
    private bool $folding = false;

    public function __construct(
        protected string $name,
        protected array $value,
        protected ?string $docblock = null,
    ) {
    }

    /**
     * Produce the code to register a class constant.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $this->processValue($compilationContext);

        if (in_array($this->value['type'], ['array', 'empty-array'], true)) {
            $compilationContext->backend->declareArrayConstant(
                $this->getName(),
                $this->value,
                $compilationContext
            );

            return;
        }

        $constantValue = $this->value['value'] ?? null;
        $compilationContext->backend->declareConstant(
            $this->value['type'],
            $this->getName(),
            $constantValue,
            $compilationContext
        );
    }

    /**
     * Returns the docblock related to the constant.
     */
    public function getDocBlock(): ?string
    {
        return $this->docblock;
    }

    /**
     * Returns the constant's name.
     */
    public function getName(): string
    {
        return $this->name;
    }

    /**
     * Get type of class constant.
     */
    public function getType(): string
    {
        return $this->value['type'];
    }

    /**
     * Returns the constant's value.
     */
    public function getValue(): array
    {
        return $this->value;
    }

    /**
     * Get the type of the value of the constant.
     */
    public function getValueType(): string
    {
        return $this->value['type'];
    }

    /**
     * Get value of the value of the constant.
     */
    public function getValueValue(): mixed
    {
        return $this->value['value'] ?? null;
    }

    /**
     * Process the value of the class constant if needed.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function processValue(CompilationContext $compilationContext): void
    {
        if (ConstantExpressionEvaluator::needsFolding($this->value)) {
            $this->foldValue($compilationContext);

            return;
        }

        if ('constant' === $this->value['type']) {
            $compiledExpression = (new Constants())->compile($this->value, $compilationContext);

            $this->value = [
                'type'  => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];

            return;
        }

        if ('static-constant-access' === $this->value['type']) {
            $compiledExpression = (new StaticConstantAccess())->compile($this->value, $compilationContext);

            $this->value = [
                'type'  => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];
        }
    }

    /**
     * Reduces an expression initializer (`const INT8_MIN = -0x7f - 1;`) to a
     * literal [#2061].
     *
     * Folding can re-enter this method through a `self::OTHER` leaf, so a
     * constant that (transitively) reads itself is reported instead of
     * recursing until the stack is exhausted.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function foldValue(CompilationContext $compilationContext): void
    {
        if ($this->folding) {
            throw new CompilerException(
                sprintf('Cyclic reference detected while resolving constant "%s"', $this->name),
                $this->value
            );
        }

        $this->folding = true;

        try {
            $this->value = (new ConstantExpressionEvaluator())->fold($this->value, $compilationContext);
        } finally {
            $this->folding = false;
        }
    }
}
