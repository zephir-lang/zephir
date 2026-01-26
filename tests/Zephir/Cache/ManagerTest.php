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

namespace Zephir\Test\Cache;

use PHPUnit\Framework\TestCase;
use Zephir\Cache\ClassEntryCache;
use Zephir\Cache\FunctionCache;
use Zephir\Cache\Manager;
use Zephir\Cache\MethodCache;
use Zephir\Cache\StaticMethodCache;
use Zephir\Passes\CallGathererPass;

final class ManagerTest extends TestCase
{
    private Manager $manager;

    protected function setUp(): void
    {
        $this->manager = new Manager();
    }

    public function testShouldGetClassEntryCache(): void
    {
        $cache1 = $this->manager->getClassEntryCache();
        $cache2 = $this->manager->getClassEntryCache();

        $this->assertInstanceOf(ClassEntryCache::class, $cache1);
        $this->assertSame($cache1, $cache2, 'Should return same instance');
    }

    public function testShouldGetFunctionCache(): void
    {
        $cache1 = $this->manager->getFunctionCache();
        $cache2 = $this->manager->getFunctionCache();

        $this->assertInstanceOf(FunctionCache::class, $cache1);
        $this->assertSame($cache1, $cache2, 'Should return same instance');
    }

    public function testShouldGetMethodCache(): void
    {
        $cache1 = $this->manager->getMethodCache();
        $cache2 = $this->manager->getMethodCache();

        $this->assertInstanceOf(MethodCache::class, $cache1);
        $this->assertSame($cache1, $cache2, 'Should return same instance');
    }

    public function testShouldGetStaticMethodCache(): void
    {
        $cache1 = $this->manager->getStaticMethodCache();
        $cache2 = $this->manager->getStaticMethodCache();

        $this->assertInstanceOf(StaticMethodCache::class, $cache1);
        $this->assertSame($cache1, $cache2, 'Should return same instance');
    }

    public function testShouldSetGatherer(): void
    {
        $gatherer = $this->createMock(CallGathererPass::class);
        $this->manager->setGatherer($gatherer);

        // After setting gatherer, caches should work
        $cache = $this->manager->getFunctionCache();
        $this->assertInstanceOf(FunctionCache::class, $cache);
    }
}
