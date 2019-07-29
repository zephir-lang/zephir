<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Branch\BranchGraphNode.
 *
 * Allows to visualize assignments for a specific variable in every branch used
 */
class BranchGraphNode
{
    /** @var int */
    protected $increase = 0;

    /** @var Branch[] */
    protected $branches = [];

    /** @var Branch */
    protected $branch;

    /**
     * BranchGraphNode.
     *
     * @param Branch $branch
     */
    public function __construct(Branch $branch)
    {
        $this->branch = $branch;
    }

    /**
     * Inserts a node in the branch graph.
     *
     * @param BranchGraphNode $branch
     */
    public function insert(self $branch)
    {
        if (!\in_array($branch, $this->branches)) {
            $this->branches[] = $branch;
        }
    }

    /**
     * Increases the branch graph level.
     */
    public function increase()
    {
        ++$this->increase;
    }

    /**
     * Generates an ASCII visualization of the branch.
     *
     * @param int $padding
     */
    public function show($padding = 0)
    {
        echo str_repeat('    ', $padding), $this->branch->getUniqueId(), ':' , $this->increase;
        if (\count($this->branches)) {
            echo ':', PHP_EOL;
            foreach ($this->branches as $node) {
                $node->show($padding + 1);
            }
        } else {
            echo PHP_EOL;
        }
    }
}
