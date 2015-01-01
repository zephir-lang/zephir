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
 * BranchManager
 *
 * Records every branch created within a method allowing analyze conditional variable assignment
 */
class BranchManager
{
    protected $currentBranch;

    protected $level = 0;

    protected $uniqueId = 1;

    /**
     * Sets the current active branch in the manager
     *
     * @param Branch $branch
     */
    public function addBranch(Branch $branch)
    {
        if ($this->currentBranch) {
            $branch->setParentBranch($this->currentBranch);
            $this->currentBranch = $branch;
        } else {
            $this->currentBranch = $branch;
        }

        $branch->setUniqueId($this->uniqueId);
        $branch->setLevel($this->level);

        $this->level++;
        $this->uniqueId++;
    }

    /**
     * Removes a branch from the branch manager
     *
     * @param Branch $branch
     */
    public function removeBranch(Branch $branch)
    {
        $parentBranch = $branch->getParentBranch();
        $this->currentBranch = $parentBranch;
        $this->level--;
    }

    /**
     * Returns the active branch in the manager
     *
     * @return Branch
     */
    public function getCurrentBranch()
    {
        return $this->currentBranch;
    }
}
