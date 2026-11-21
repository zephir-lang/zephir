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
use Zephir\Class\Definition\Definition;
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

    private ?array $attributes = null;

    /** The class the constant belongs to, whose scope its value is read in. */
    private ?Definition $definition = null;

    public function __construct(
        protected string $name,
        protected array $value,
        protected ?string $docblock = null,
        protected ?array $original = null,
    ) {
    }

    /**
     * The raw AST node, kept for the `#[...]` attributes written on this
     * constant. Mirrors Property::getOriginal().
     */
    public function getOriginal(): ?array
    {
        return $this->original;
    }

    /**
     * The trait this member was merged from, or null when the class declares
     * it itself.
     */
    public function getDeclaringTrait(): ?string
    {
        return $this->original['declaring-trait'] ?? null;
    }

    public function setDefinition(Definition $definition): void
    {
        $this->definition = $definition;
    }

    /**
     * @return Attribute[]
     */
    public function getAttributes(): array
    {
        return $this->attributes ??= Attribute::listFromNode($this->original);
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
        $context = $this->declarationContext($compilationContext);

        /**
         * Every value goes through the evaluator, even one that needs no
         * folding, so an `Attribute::*` flag, alone or inside an array, keeps
         * its C macro instead of the value of the PHP running Zephir [#2738].
         * Anything else comes back untouched.
         */
        $this->foldValue($context);

        if (isset($this->value['native'])) {
            return;
        }

        /**
         * An array element or key may name a constant too, which PHP resolves
         * as it compiles the declaration. See #2825.
         */
        if ('array' === $this->value['type']) {
            $this->value = (new ConstantExpressionEvaluator())->reduce($this->value, $context);

            return;
        }

        if ('constant' === $this->value['type']) {
            $compiledExpression = (new Constants())->compile($this->value, $context);

            $this->value = [
                'type'  => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];

            return;
        }

        if ('static-constant-access' === $this->value['type']) {
            $array = (new StaticConstantAccess())->arrayConstantValue($this->value, $context);
            if (null !== $array) {
                $this->value = $array;

                return;
            }

            $access = new StaticConstantAccess();
            $access->setAlwaysFold(true);
            $compiledExpression = $access->compile($this->value, $context);

            $this->value = [
                'type'  => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];
        }
    }

    /**
     * The scope a constant's value is read in: the class it belongs to, with
     * the names of the file it is written in, outside any function. It is
     * read wherever it is first used, which may be another class's method.
     * See #2825.
     */
    private function declarationContext(CompilationContext $compilationContext): CompilationContext
    {
        if (null === $this->definition) {
            return $compilationContext;
        }

        $context                  = clone $compilationContext;
        $context->classDefinition = $this->definition;
        $context->aliasManager    = $this->definition->getAliasManager() ?? $compilationContext->aliasManager;
        $context->declaringTrait  = $this->getDeclaringTrait();
        $context->currentMethod   = null;
        $context->symbolTable     = null;

        return $context;
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
