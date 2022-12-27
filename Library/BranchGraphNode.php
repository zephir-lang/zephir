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

use function count;
use function in_array;

/**
 * Allows visualizing assignments for a specific variable in every branch used.
 */
class BranchGraphNode
{
    /**
     * @var int
     */
    protected int $increase = 0;

    /**
     * @var Branch[]
     */
    protected array $branches = [];

    /**
     * @var Branch
     */
    protected Branch $branch;

    /**
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
    public function insert(self $branch): void
    {
        if (!in_array($branch, $this->branches)) {
            $this->branches[] = $branch;
        }
    }

    /**
     * Increases the branch graph level.
     */
    public function increase(): void
    {
        ++$this->increase;
    }

    /**
     * Generates an ASCII visualization of the branch.
     *
     * @param int $padding
     */
    public function show(int $padding = 0): void
    {
        echo str_repeat('    ', $padding), $this->branch->getUniqueId(), ':' , $this->increase;

        if (count($this->branches) > 0) {
            echo ':', PHP_EOL;
            /** @var BranchGraphNode $node */
            foreach ($this->branches as $node) {
                $node->show($padding + 1);
            }
        } else {
            echo PHP_EOL;
        }
    }
}
