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
    private $assignments;

    /**
     * @param array|null $assignments
     */
    public function __construct(array $assignments = null)
    {
        if (null !== $assignments) {
            $this->setAssignments($assignments);
        }
    }

    /**
     * @param mixed $assignment
     *
     * @return $this
     */
    public function addAssignment($assignment)
    {
        $this->assignments[] = $assignment;

        return $this;
    }

    /**
     * @return mixed
     */
    public function getAssignments()
    {
        return $this->assignments;
    }

    /**
     * @param array $assignments
     *
     * @return $this
     */
    public function setAssignments($assignments)
    {
        $this->assignments = $assignments;

        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return [
            'type'        => 'let',
            'assignments' => $this->getAssignments(),
        ];
    }
}
