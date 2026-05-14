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

namespace Zephir\Expression\Builder\Statements;

class RawStatement extends AbstractStatement
{
    private array $expression;

    public function __construct(?array $expression = null)
    {
        if (null !== $expression) {
            $this->setExpression($expression);
        }
    }

    public function getExpression(): array
    {
        return $this->expression;
    }

    public function setExpression(array $expression): self
    {
        $this->expression = $expression;

        return $this;
    }

    protected function preBuild(): array
    {
        return $this->getExpression();
    }
}
