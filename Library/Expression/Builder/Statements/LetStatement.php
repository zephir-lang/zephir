<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/
namespace Zephir\Expression\Builder\Statements;

/**
 * LetStatement
 *
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
        if ($assignments !== null) {
            $this->setAssignments($assignments);
        }
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
     * @return $this
     */
    public function setAssignments($assignments)
    {
        $this->assignments = $assignments;
        return $this;
    }

    /**
     * @param mixed $assignment
     * @return $this
     */
    public function addAssignment($assignment)
    {
        $this->assignments[] = $assignment;
        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return array(
            'type' => 'let',
            'assignments' => $this->getAssignments()
        );
    }
}
