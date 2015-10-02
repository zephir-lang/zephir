<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
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
