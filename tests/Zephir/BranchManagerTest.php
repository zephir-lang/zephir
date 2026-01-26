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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Branch;
use Zephir\BranchManager;

final class BranchManagerTest extends TestCase
{
    private BranchManager $branchManager;

    protected function setUp(): void
    {
        $this->branchManager = new BranchManager();
    }

    public function testShouldInitializeWithDefaults(): void
    {
        $this->assertNull($this->branchManager->getCurrentBranch());
    }

    public function testShouldAddRootBranch(): void
    {
        $rootBranch = new Branch();
        $rootBranch->setType(Branch::TYPE_ROOT);

        $this->branchManager->addBranch($rootBranch);

        $this->assertSame($rootBranch, $this->branchManager->getCurrentBranch());
        $this->assertSame(0, $rootBranch->getLevel());
    }

    public function testShouldAddNestedBranches(): void
    {
        $rootBranch = new Branch();
        $rootBranch->setType(Branch::TYPE_ROOT);
        $this->branchManager->addBranch($rootBranch);

        $childBranch = new Branch();
        $childBranch->setType(Branch::TYPE_CONDITIONAL_TRUE);
        $this->branchManager->addBranch($childBranch);

        $this->assertSame($childBranch, $this->branchManager->getCurrentBranch());
        $this->assertSame($rootBranch, $childBranch->getParentBranch());
        $this->assertSame(1, $childBranch->getLevel());
    }

    public function testShouldRemoveBranchAndRestoreParent(): void
    {
        $rootBranch = new Branch();
        $rootBranch->setType(Branch::TYPE_ROOT);
        $this->branchManager->addBranch($rootBranch);

        $childBranch = new Branch();
        $childBranch->setType(Branch::TYPE_CONDITIONAL_TRUE);
        $this->branchManager->addBranch($childBranch);

        $this->branchManager->removeBranch($childBranch);

        $this->assertSame($rootBranch, $this->branchManager->getCurrentBranch());
    }

    public function testShouldAssignUniqueIds(): void
    {
        $branch1 = new Branch();
        $this->branchManager->addBranch($branch1);

        $branch2 = new Branch();
        $this->branchManager->addBranch($branch2);

        $this->assertSame(1, $branch1->getUniqueId());
        $this->assertSame(2, $branch2->getUniqueId());
    }

    public function testShouldIncrementLevels(): void
    {
        $branch1 = new Branch();
        $this->branchManager->addBranch($branch1);
        $this->assertSame(0, $branch1->getLevel());

        $branch2 = new Branch();
        $this->branchManager->addBranch($branch2);
        $this->assertSame(1, $branch2->getLevel());

        $branch3 = new Branch();
        $this->branchManager->addBranch($branch3);
        $this->assertSame(2, $branch3->getLevel());
    }
}
