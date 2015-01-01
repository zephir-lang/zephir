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

namespace Zephir;

/**
 * Branch
 *
 * Represents every branch within a method
 */
class Branch
{
    protected $parentBranch;

    protected $level = -1;

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
     * @param $relatedStatement
     */
    public function setRelatedStatement($relatedStatement)
    {
        $this->relatedStatement = $relatedStatement;
    }

    /**
     * @return mixed
     */
    public function getRelatedStatement()
    {
        return $this->relatedStatement;
    }
}
