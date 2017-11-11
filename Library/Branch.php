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

namespace Zephir;

use Zephir\Statements\StatementAbstract;

/**
 * Branch
 *
 * Represents every branch within a method
 */
class Branch
{
    protected $parentBranch;

    protected $level = -1;

    /** @var  StatementAbstract|null */
    protected $relatedStatement;

    protected $type;

    protected $unreachable;

    const TYPE_ROOT              = 0;

    const TYPE_CONDITIONAL_TRUE  = 1;

    const TYPE_CONDITIONAL_FALSE = 2;

    const TYPE_LOOP_INFINITE     = 3;

    const TYPE_LOOP_CONDITIONAL  = 4;

    const TYPE_SWITCH            = 5;

    const TYPE_EXTERNAL          = 6;

    const TYPE_UNKNOWN           = 7;

    private $uniqueId;

    /**
     * Set the branch's parent
     *
     * @param Branch $parentBranch
     */
    public function setParentBranch(Branch $parentBranch)
    {
        $this->parentBranch = $parentBranch;
    }

    /**
     * Returns the branch's parent
     *
     * @return Branch
     */
    public function getParentBranch()
    {
        return $this->parentBranch;
    }

    /**
     * Set the type of branch. One of the TYPE_* constants
     *
     * @param int $type
     */
    public function setType($type)
    {
        $this->type = $type;
    }

    /**
     * Returns the branch type
     *
     * @return int
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * Sets if the branch is unreachable
     *
     * @param boolean $unreachable
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
     * @return void
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
