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
use Zephir\Statements\StatementAbstract;

final class BranchTest extends TestCase
{
    public function testShouldCreateBranchWithDefaults(): void
    {
        $branch = new Branch();

        $this->assertSame(-1, $branch->getLevel());
        $this->assertNull($branch->getParentBranch());
        $this->assertSame(Branch::TYPE_ROOT, $branch->getType());
        $this->assertNull($branch->getRelatedStatement());
    }

    public function testShouldSetAndGetLevel(): void
    {
        $branch = new Branch();
        $branch->setLevel(5);

        $this->assertSame(5, $branch->getLevel());
    }

    public function testShouldSetAndGetParentBranch(): void
    {
        $parent = new Branch();
        $child = new Branch();
        $child->setParentBranch($parent);

        $this->assertSame($parent, $child->getParentBranch());
    }

    public function testShouldSetAndGetType(): void
    {
        $branch = new Branch();
        $branch->setType(Branch::TYPE_CONDITIONAL_TRUE);

        $this->assertSame(Branch::TYPE_CONDITIONAL_TRUE, $branch->getType());
    }

    public function testShouldSetAndGetRelatedStatement(): void
    {
        $branch = new Branch();
        $statement = $this->createMock(StatementAbstract::class);
        $branch->setRelatedStatement($statement);

        $this->assertSame($statement, $branch->getRelatedStatement());
    }

    public function testShouldSetAndGetUniqueId(): void
    {
        $branch = new Branch();
        $branch->setUniqueId(42);

        $this->assertSame(42, $branch->getUniqueId());
    }

    public function testShouldSetAndGetUnreachable(): void
    {
        $branch = new Branch();

        $this->assertNull($branch->isUnreachable());

        $branch->setUnreachable(true);
        $this->assertTrue($branch->isUnreachable());

        $branch->setUnreachable(false);
        $this->assertFalse($branch->isUnreachable());
    }

    public function testBranchTypeConstants(): void
    {
        $this->assertSame(0, Branch::TYPE_ROOT);
        $this->assertSame(1, Branch::TYPE_CONDITIONAL_TRUE);
        $this->assertSame(2, Branch::TYPE_CONDITIONAL_FALSE);
        $this->assertSame(3, Branch::TYPE_LOOP_INFINITE);
        $this->assertSame(4, Branch::TYPE_LOOP_CONDITIONAL);
        $this->assertSame(5, Branch::TYPE_SWITCH);
        $this->assertSame(6, Branch::TYPE_EXTERNAL);
        $this->assertSame(7, Branch::TYPE_UNKNOWN);
    }
}
