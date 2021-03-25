<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Statements\StatementAbstract;

/**
 * Branch.
 *
 * Represents every branch within a method
 */
class Branch
{
    const TYPE_ROOT = 0;

    const TYPE_CONDITIONAL_TRUE = 1;

    const TYPE_CONDITIONAL_FALSE = 2;

    const TYPE_LOOP_INFINITE = 3;

    const TYPE_LOOP_CONDITIONAL = 4;

    const TYPE_SWITCH = 5;

    const TYPE_EXTERNAL = 6;

    const TYPE_UNKNOWN = 7;
    protected $parentBranch;

    protected $level = -1;

    /** @var StatementAbstract|null */
    protected $relatedStatement;

    protected $type;

    protected $unreachable;

    private $uniqueId;

    /**
     * Set the branch's parent.
     *
     * @param Branch $parentBranch
     */
    public function setParentBranch(self $parentBranch)
    {
        $this->parentBranch = $parentBranch;
    }

    /**
     * Returns the branch's parent.
     *
     * @return Branch
     */
    public function getParentBranch()
    {
        return $this->parentBranch;
    }

    /**
     * Set the type of branch. One of the TYPE_* constants.
     *
     * @param int $type
     */
    public function setType($type)
    {
        $this->type = $type;
    }

    /**
     * Returns the branch type.
     *
     * @return int
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * Sets if the branch is unreachable.
     *
     * @param bool $unreachable
     */
    public function setUnreachable($unreachable)
    {
        $this->unreachable = $unreachable;
    }

    /**
     * @return mixed
     */
    public function isUnreachable()
    {
        return $this->unreachable;
    }

    /**
     * @param $level
     */
    public function setLevel($level)
    {
        $this->level = $level;
    }

    /**
     * @return int
     */
    public function getLevel()
    {
        return $this->level;
    }

    /**
     * @param $uniqueId
     */
    public function setUniqueId($uniqueId)
    {
        $this->uniqueId = $uniqueId;
    }

    /**
     * @return mixed
     */
    public function getUniqueId()
    {
        return $this->uniqueId;
    }

    /**
     * @param StatementAbstract $relatedStatement
     */
    public function setRelatedStatement(StatementAbstract $relatedStatement)
    {
        $this->relatedStatement = $relatedStatement;
    }

    /**
     * @return StatementAbstract|null
     */
    public function getRelatedStatement()
    {
        return $this->relatedStatement;
    }
}
