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

namespace Zephir;

use Closure;

use function in_array;

/**
 * This represents a compiled expression, the object can be used to check
 * if the expression type is able to used in certain types of the application.
 */
class CompiledExpression implements TypeAwareInterface
{
    public function __construct(protected string $type, protected mixed $code, protected ?array $originalExpr = null)
    {
    }

    /**
     * Returns a C representation for a boolean constant.
     */
    public function getBooleanCode(): mixed
    {
        if ('true' === $this->code || true === $this->code) {
            return '1';
        }

        if ('false' === $this->code || false === $this->code) {
            return '0';
        }

        return $this->code;
    }

    /**
     * Returns the code produced by the compiled expression.
     */
    public function getCode(): mixed
    {
        return $this->code;
    }

    /**
     * Original AST code that produced the code.
     */
    public function getOriginal(): ?array
    {
        return $this->originalExpr;
    }

    /**
     * Returns the type of the compiled expression.
     */
    public function getType(): string
    {
        return $this->type;
    }

    /**
     * Checks if the compiled expression is an integer or compatible type.
     */
    public function isIntCompatibleType(): bool
    {
        return in_array($this->type, ['int', 'uint', 'long', 'ulong', 'char', 'uchar'], true);
    }

    /**
     * Resolves an expression
     *
     * Some code cannot be directly pushed into the generated source
     * because it's missing some bound parts, this method resolves the missing parts
     * returning the generated code.
     */
    public function resolve(?string $result, CompilationContext $compilationContext): string
    {
        if (!($this->code instanceof Closure)) {
            return $this->code;
        }

        $code = $this->code;
        if (!$result) {
            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext
            );
            $compilationContext->codePrinter->output($code($tempVariable->getName()));
            $tempVariable->setIsInitialized(true, $compilationContext);

            return $tempVariable->getName();
        }

        return $code($result);
    }
}
