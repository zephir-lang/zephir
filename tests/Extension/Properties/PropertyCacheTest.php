<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\PropertyCache;
use Stub\PropertyCacheChild;
use Stub\PropertyCacheMagic;

/**
 * Locks the runtime behaviour of the cached object-property read/write path
 * (zephir_read_property_cached / zephir_update_property_zval_cached) across
 * every reachable shape, so the property-access optimization provably
 * preserves semantics. Each property is read AND written, and hot paths are
 * exercised repeatedly (first access = cache miss/fill, later = cache hit).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */
final class PropertyCacheTest extends TestCase
{
    public function testPublicReadWrite(): void
    {
        $o = new PropertyCache();
        $this->assertSame(1, $o->getPub());
        $o->setPub(9);
        $this->assertSame(9, $o->getPub());
    }

    public function testProtectedReadWrite(): void
    {
        $o = new PropertyCache();
        $this->assertSame(2, $o->getProt());
        $o->setProt(20);
        $this->assertSame(20, $o->getProt());
    }

    public function testPrivateReadWrite(): void
    {
        $o = new PropertyCache();
        $this->assertSame(3, $o->getPriv());
        $o->setPriv(30);
        $this->assertSame(30, $o->getPriv());
    }

    public function testCompoundReadModifyWrite(): void
    {
        $o = new PropertyCache();
        $this->assertSame(2, $o->incPub());
        $this->assertSame(3, $o->incPub());
        $this->assertSame(3, $o->getPub());
    }

    public function testSameSiteReadTwice(): void
    {
        $o = new PropertyCache();
        $this->assertSame(2, $o->sumPubTwice());
        $o->setPub(5);
        $this->assertSame(10, $o->sumPubTwice());
    }

    public function testValueTypeTransitionArrayThenScalar(): void
    {
        $o = new PropertyCache();
        $this->assertSame(42, $o->swapArrayScalar());
        $this->assertSame(42, $o->getPub());
    }

    public function testBranchedReadUsesFunctionScopeSlot(): void
    {
        // The name slot is declared at function scope, so both branches and
        // the post-branch read must resolve it.
        $o = new PropertyCache();       // pub=1, prot=2, priv=3
        $this->assertSame(4, $o->branchedRead(true));   // pub + priv
        $this->assertSame(5, $o->branchedRead(false));  // prot + priv
    }

    public function testHotReadLoopCacheHitPath(): void
    {
        $o = new PropertyCache();
        $this->assertSame(3, $o->readLoop(3));   // 3 * pub(1)
        $o->setPub(10);
        $this->assertSame(50, $o->readLoop(5));  // 5 * pub(10)
    }

    public function testMutateAndReadWithinSameLoopIteration(): void
    {
        // Value changes every iteration: a cached write then a cached read of
        // the same property in the same iteration. The read must see the
        // just-written value, never a stale cached one. 1+2+3+4 = 10.
        $o = new PropertyCache();
        $this->assertSame(10, $o->mutateInLoop(4));
        // The last written value is still readable afterwards through the
        // same (now warm) cache slot.
        $this->assertSame(4, $o->getPub());
    }

    public function testCompoundMutateInLoop(): void
    {
        // Cached read + cached write against the same slot each iteration
        // while the value keeps changing. 0+1+2+3+4 = 10.
        $o = new PropertyCache();
        $this->assertSame(10, $o->accumulateInLoop(4));
        $this->assertSame(10, $o->getPub());
    }

    public function testMultipleInstancesAreIndependent(): void
    {
        $a = new PropertyCache();
        $b = new PropertyCache();
        $a->setPub(11);
        $b->setPub(22);
        $this->assertSame(11, $a->getPub());
        $this->assertSame(22, $b->getPub());
    }

    public function testCloneIsIndependent(): void
    {
        $a = new PropertyCache();
        $a->setPub(11);
        $c = clone $a;
        $this->assertSame(11, $c->getPub());
        $c->setPub(33);
        $this->assertSame(11, $a->getPub());
        $this->assertSame(33, $c->getPub());
    }

    public function testInheritedMethodOnChildInstance(): void
    {
        $child = new PropertyCacheChild();
        // getPub() is compiled in the parent; this_ptr is the child ce.
        $this->assertSame(1, $child->getPub());
        $this->assertSame(1, $child->getInheritedPub());
        $this->assertSame(100, $child->getChildProp());
        $child->setPub(7);
        $this->assertSame(7, $child->getPub());
    }

    /**
     * The parent's cached access site runs against three different class
     * entries (parent, Zephir child, PHP userland child) interleaved — the
     * engine must re-validate ce each time, never reuse a stale entry.
     */
    public function testPolymorphicSiteAcrossClassEntries(): void
    {
        $parent = new PropertyCache();
        $zChild = new PropertyCacheChild();
        $pChild = new PhpPropertyCacheChild();

        for ($i = 0; $i < 50; $i++) {
            $this->assertSame(1, $parent->getPub());
            $this->assertSame(1, $zChild->getPub());
            $this->assertSame(1, $pChild->getPub());
        }

        $parent->setPub(10);
        $zChild->setPub(20);
        $pChild->setPub(30);
        $this->assertSame(10, $parent->getPub());
        $this->assertSame(20, $zChild->getPub());
        $this->assertSame(30, $pChild->getPub());
    }

    /**
     * Magic accessors are reached only via the dynamic `this->{name}` path,
     * which the optimization does not touch; pin it stays correct.
     */
    public function testMagicViaDynamicPathUnaffected(): void
    {
        $m = new PropertyCacheMagic();
        $m->writeVirtual('foo', 42);
        $this->assertSame(42, $m->readVirtual('foo'));
        $this->assertNull($m->readVirtual('missing'));
    }
}

/**
 * PHP userland subclass: gives the parent's cached access site a class entry
 * the compiled extension never saw at build time.
 */
class PhpPropertyCacheChild extends PropertyCache
{
}
