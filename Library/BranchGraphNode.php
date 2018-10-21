<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

/**
 * BranchGraphNode
 *
 * Allows to visualize assignments for a specific variable in every branch used
 */
class BranchGraphNode
{
    protected $_increase = 0;

    protected $_branches = array();

    /**
     * BranchGraphNode
     *
     * @param Branch $branch
     */
    public function __construct($branch)
    {
        $this->_branch = $branch;
    }

    /**
     * Inserts a node in the branch graph
     *
     * @param BranchGraphNode $branch
     */
    public function insert(BranchGraphNode $branch)
    {
        if (!in_array($branch, $this->_branches)) {
            $this->_branches[] = $branch;
        }
    }

    /**
     * Increases the branch graph level
     */
    public function increase()
    {
        $this->_increase++;
    }

    /**
     * Generates an ASCII visualization of the branch
     *
     * @param int $padding
     */
    public function show($padding = 0)
    {
        echo str_repeat("    ", $padding), $this->_branch->getUniqueId(), ':' , $this->_increase;
        if (count($this->_branches)) {
            echo ':', PHP_EOL;
            foreach ($this->_branches as $node) {
                $node->show($padding + 1);
            }
        } else {
            echo PHP_EOL;
        }
    }
}
