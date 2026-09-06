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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;

/**
 * Verifies that isset(obj->staticProp) emits a per-call-site interned
 * zend_string cache rather than a fresh zend_string_init per call.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2385
 */
final class IssetPropertyCacheTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_isset_cache_test_', ['stub/issue2385']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testStaticPropertyIssetEmitsCachedZendString(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2385;

class Cache
{
    public foo;

    public function check() -> bool
    {
        return isset(this->foo);
    }
}
ZEP;
        $c = $this->compileZep('Stub\\Issue2385\\Cache', 'stub/issue2385/cache.zep', $zep);

        $body = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2385_Cache, check)');
        $this->assertNotSame('', $body, 'Could not locate check() body.');

        $this->assertMatchesRegularExpression(
            '/static\s+zend_string\s*\*\s*\w+\s*=\s*NULL/',
            $body,
            "Expected a per-call-site static zend_string * cache slot.\n$body"
        );

        $this->assertStringContainsString(
            'zephir_isset_property_value_fast(',
            $body,
            "Expected the cache-aware fast helper to be called.\n$body"
        );

        // The old per-call zend_string_init path must NOT appear for the
        // static-name isset — the cached fast helper supersedes it.
        $this->assertStringNotContainsString(
            'zephir_isset_property_value(&this_ptr',
            $body,
            "Expected the cache helper to replace the alloc-per-call helper.\n$body"
        );
    }

    public function testRepeatedAccessReusesSameSlot(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2385;

class CacheReuse
{
    public foo;

    public function checkTwice() -> bool
    {
        if isset(this->foo) {
            if isset(this->foo) {
                return true;
            }
        }
        return false;
    }
}
ZEP;
        $c = $this->compileZep('Stub\\Issue2385\\CacheReuse', 'stub/issue2385/cachereuse.zep', $zep);

        $body = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2385_CacheReuse, checkTwice)');
        $this->assertNotSame('', $body, 'Could not locate checkTwice() body.');

        // Exactly one cache slot should be emitted for the same property
        // checked twice within a single method body.
        $count = preg_match_all('/static\s+zend_string\s*\*/', $body);
        $this->assertSame(
            1,
            $count,
            "Expected exactly one static zend_string * slot for repeated isset of same property; got $count.\n$body"
        );
    }

    private function compileZep(string $className, string $relPath, string $zep): string
    {
        $this->compileSource($className, $relPath, $zep);

        return $this->generatedC($relPath);
    }
}
