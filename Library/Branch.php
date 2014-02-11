<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
    protected $_parentBranch;

    protected $_level = -1;

    protected $_relatedStatement;

    const TYPE_ROOT              = 0;

    const TYPE_CONDITIONAL_TRUE  = 1;

    const TYPE_CONDITIONAL_FALSE = 2;

    const TYPE_LOOP_INFINITE     = 3;

    const TYPE_LOOP_CONDITIONAL  = 4;

    const TYPE_SWITCH            = 5;

    const TYPE_EXTERNAL          = 6;

    const TYPE_UNKNOWN           = 7;

    /**
     * Set the branch's parent
     *
     * @param Branch $parentBranch
     */
    public function setParentBranch(Branch $parentBranch)
    {
        $this->_parentBranch = $parentBranch;
    }

    /**
     * Returns the branch's parent
     *
     * @return Branch
     */
    public function getParentBranch()
    {
        return $this->_parentBranch;
    }

    /**
     * Set the type of branch. One of the TYPE_* constants
     *
     * @param int $type
     */
    public function setType($type)
    {
        $this->_type = $type;
    }

    /**
     * Returns the branch type
     *
     * @return int
     */
    public function getType()
    {
        return $this->_type;
    }

    /**
     * Sets if the branch is unrechable
     *
     * @param boolean $unrecheable
     */
    public function setUnrecheable($unrecheable)
    {
        $this->_unrecheable = $unrecheable;
    }

    public function isUnrecheable()
    {
        return $this->_unrecheable;
    }

    public function setLevel($level)
    {
        $this->_level = $level;
    }

    public function getLevel()
    {
        return $this->_level;
    }

    public function setUniqueId($uniqueId)
    {
        $this->_uniqueId = $uniqueId;
    }

    public function getUniqueId()
    {
        return $this->_uniqueId;
    }

    public function setRelatedStatement($relatedStatement)
    {
        $this->_relatedStatement = $relatedStatement;
    }

    public function getRelatedStatement()
    {
        return $this->_relatedStatement;
    }
}
