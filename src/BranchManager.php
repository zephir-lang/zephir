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

namespace Zephir;

/**
 * Records every branch created within a method allowing to analyze conditional variable assignment
 */
class BranchManager
{
    protected ?Branch $currentBranch = null;

    protected int $level = 0;

    protected int $uniqueId = 1;

    protected ?Branch $rootBranch;

    /**
     * Sets the current active branch in the manager.
     *
     * @param Branch $branch
     */
    public function addBranch(Branch $branch): void
    {
        if ($this->currentBranch) {
            $branch->setParentBranch($this->currentBranch);
        }

        $this->currentBranch = $branch;

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
    public function removeBranch(Branch $branch): void
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
    public function getCurrentBranch(): ?Branch
    {
        return $this->currentBranch;
    }

    public function getCurrentBranchId()
    {
        return $this->currentBranch->getUniqueId();
    }

    public function setRootBranch(Branch $branch): void
    {
        $this->rootBranch = $branch;
    }
}
