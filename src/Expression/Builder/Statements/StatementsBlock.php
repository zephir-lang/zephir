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

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * Allows to manually build a statements block AST node
 */
class StatementsBlock extends AbstractBuilder
{
    public function __construct(private ?array $statements = null)
    {
        if (null !== $statements) {
            $this->setStatements($statements);
        }
    }

    public function build(): array
    {
        return $this->resolve($this->preBuild());
    }

    public function getStatements(): array
    {
        return $this->statements;
    }

    public function setStatements(array $statements): self
    {
        $this->statements = $statements;

        return $this;
    }

    protected function preBuild(): array
    {
        return $this->getStatements();
    }
}
