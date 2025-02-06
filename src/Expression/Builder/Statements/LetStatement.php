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

/**
 * Allows to manually build a 'let' statement AST node
 */
class LetStatement extends AbstractStatement
{
    public function __construct(private ?array $assignments = null)
    {
        if (null !== $assignments) {
            $this->setAssignments($assignments);
        }
    }

    public function getAssignments(): array
    {
        return $this->assignments;
    }

    public function setAssignments(array $assignments): self
    {
        $this->assignments = $assignments;

        return $this;
    }

    protected function preBuild(): array
    {
        return [
            'type'        => 'let',
            'assignments' => $this->getAssignments(),
        ];
    }
}
