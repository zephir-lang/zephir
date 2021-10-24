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
 * Represents a group of branch nodes
 */
class BranchGraph
{
    protected ?BranchGraphNode $root = null;

    protected array $branchMap = [];

    /**
     * Adds a leaf to the branch tree.
     *
     * @param Branch $branch
     */
    public function addLeaf(Branch $branch): void
    {
        $branchNode = $this->branchMap[$branch->getUniqueId()] ?? new BranchGraphNode($branch);
        $branchNode->increase();

        $tempBranch = $branch->getParentBranch();
        while ($tempBranch) {
            if (isset($this->branchMap[$tempBranch->getUniqueId()])) {
                $parentBranchNode = $this->branchMap[$tempBranch->getUniqueId()];
            } else {
                $parentBranchNode = new BranchGraphNode($tempBranch);
                $this->branchMap[$tempBranch->getUniqueId()] = $parentBranchNode;
            }

            $parentBranchNode->insert($branchNode);
            $branchNode = $parentBranchNode;
            $tempBranch = $tempBranch->getParentBranch();
            if (!$tempBranch) {
                $this->root = $parentBranchNode;
            }
        }
    }

    /**
     * Returns the tree's root node.
     *
     * @return BranchGraphNode
     */
    public function getRoot(): BranchGraphNode
    {
        return $this->root;
    }
}
