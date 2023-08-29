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

/**
 * BranchManager.
 *
 * Records every branch created within a method allowing analyze conditional variable assignment
 */
class BranchManager
{
    /** @var Branch|null */
    protected $currentBranch;

    protected $level = 0;

    protected $uniqueId = 1;

    /** @var Branch|null */
    protected $rootBranch;

    /**
     * Sets the current active branch in the manager.
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

        ++$this->level;
        ++$this->uniqueId;

        if (Branch::TYPE_ROOT == $branch->getType()) {
            $this->setRootBranch($branch);
        }
    }

    /**
     * Removes a branch from the branch manager.
     *
     * @param Branch $branch
     */
    public function removeBranch(Branch $branch)
    {
        $parentBranch = $branch->getParentBranch();
        $this->currentBranch = $parentBranch;
        --$this->level;
    }

    /**
     * Returns the active branch in the manager.
     *
     * @return Branch|null
     */
    public function getCurrentBranch()
    {
        return $this->currentBranch;
    }

    public function getCurrentBranchId()
    {
        return $this->currentBranch->getUniqueId();
    }

    public function setRootBranch(Branch $branch)
    {
        $this->rootBranch = $branch;
    }
}
