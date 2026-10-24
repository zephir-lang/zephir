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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Globals;

/**
 * Extension\GlobalsTest.
 */
final class GlobalsTest extends TestCase
{
    private Globals $test;

    protected function setUp(): void
    {
        $this->test = new Globals();
    }

    public function testShouldGetGlobalExtensionSettings(): void
    {
        $this->assertFalse($this->test->getDefaultGlobals1());

        $this->assertTrue($this->test->getDefaultGlobals4());

        $this->assertSame(100, $this->test->getDefaultGlobals2());
        $this->assertSame(7.5, $this->test->getDefaultGlobals3());
        $this->assertSame(10, $this->test->getDefaultGlobals5());
        $this->assertSame(15.2, $this->test->getDefaultGlobals6());
        $this->assertSame(65, $this->test->getDefaultGlobals7());
        $this->assertSame('custom_value', $this->test->getDefaultGlobals8());
    }

    public function testShouldSetGlobalExtensionSetting(): void
    {
        $this->test->setBoolValueUsingDotNotation(false);

        $this->assertFalse($this->test->getDefaultGlobals1());
    }

    public function testSetIntValueUsingDotNotation(): void
    {
        $this->test->setIntValueUsingDotNotation(900);

        $this->assertSame(900, $this->test->getDefaultGlobals2());
    }

    public function testSetCharValueUsingString(): void
    {
        $this->test->setCharValue('B');

        $this->assertSame(66, $this->test->getDefaultGlobals7());
    }

    public function testSetCharValueUsingInt(): void
    {
        $this->test->setCharValue(90);

        $this->assertSame(90, $this->test->getDefaultGlobals7());
    }

    public function testSetStringValue(): void
    {
        $this->test->setStringValue('1');
        $this->assertSame('1', $this->test->getDefaultGlobals8());

        $this->test->setStringValue('c');
        $this->assertSame('c', $this->test->getDefaultGlobals8());

        $this->test->setStringValue('char');
        $this->assertSame('char', $this->test->getDefaultGlobals8());

        $this->test->setStringValue('Long Text without any sense...');
        $this->assertSame('Long Text without any sense...', $this->test->getDefaultGlobals8());

        /**
         * Get and set string value from globals struct.
         */
        $this->assertSame('prefix-string-', $this->test->getDefaultGlobalsOrmCachePrefix());

        $this->test->setDefaultGlobalsOrmCachePrefix('1');
        $this->assertSame('1', $this->test->getDefaultGlobalsOrmCachePrefix());

        $this->test->setDefaultGlobalsOrmCachePrefix('c');
        $this->assertSame('c', $this->test->getDefaultGlobalsOrmCachePrefix());

        $this->test->setDefaultGlobalsOrmCachePrefix('char');
        $this->assertSame('char', $this->test->getDefaultGlobalsOrmCachePrefix());

        $this->test->setDefaultGlobalsOrmCachePrefix('Long Text without any sense...');
        $this->assertSame('Long Text without any sense...', $this->test->getDefaultGlobalsOrmCachePrefix());
    }

    /**
     * globals_set() kept only the char * of the zend_string it converted the
     * value to, so every call leaked that string. Each value here is built at
     * runtime, so none of them is interned and a leak cannot hide.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2725
     */
    public function testSetStringValueShouldNotLeak(): void
    {
        $this->test->setStringValue(str_repeat('x', 64) . 0);
        $this->test->setDefaultGlobalsOrmCachePrefix(str_repeat('y', 64) . 0);

        $before = memory_get_usage();
        for ($i = 1; $i <= 1000; $i++) {
            $this->test->setStringValue(str_repeat('x', 64) . $i);
            $this->test->setDefaultGlobalsOrmCachePrefix(str_repeat('y', 64) . $i);
        }
        $after = memory_get_usage();

        $this->assertSame(0, $after - $before);
        $this->assertSame(str_repeat('x', 64) . 1000, $this->test->getDefaultGlobals8());
        $this->assertSame(str_repeat('y', 64) . 1000, $this->test->getDefaultGlobalsOrmCachePrefix());

        $this->test->setStringValue('custom_value');
        $this->test->setDefaultGlobalsOrmCachePrefix('prefix-string-');
    }

    public function testSetBoolValueUsingInt(): void
    {
        $this->test->setBoolValue(0);

        $this->assertFalse($this->test->getDefaultGlobals4());
    }

    public function testSetBoolValueUsingBool(): void
    {
        $this->test->setBoolValue(true);
        $this->assertTrue($this->test->getDefaultGlobals4());

        $this->test->setBoolValue(false);
        $this->assertFalse($this->test->getDefaultGlobals4());
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2726
     */
    public function testHashValueShouldDefaultToNull(): void
    {
        $this->assertNull($this->test->getHashValue());
        $this->assertNull($this->test->getDefaultGlobalsOrmAstCache());
    }

    public function testSetHashValue(): void
    {
        $this->test->setHashValue(['a' => 1, 'b' => [2]]);
        $this->assertSame(['a' => 1, 'b' => [2]], $this->test->getHashValue());

        $this->test->setDefaultGlobalsOrmAstCache(['phql' => 'ast']);
        $this->assertSame(['phql' => 'ast'], $this->test->getDefaultGlobalsOrmAstCache());

        $this->test->setHashValue(null);
        $this->test->setDefaultGlobalsOrmAstCache(null);
        $this->assertNull($this->test->getHashValue());
        $this->assertNull($this->test->getDefaultGlobalsOrmAstCache());
    }

    public function testSetHashValueCastsLikePhp(): void
    {
        $this->test->setHashValue('scalar');
        $this->assertSame((array) 'scalar', $this->test->getHashValue());

        $this->test->setHashValue((object) ['x' => 1]);
        $this->assertSame(['x' => 1], $this->test->getHashValue());

        $this->test->setHashValue(null);
    }

    public function testSetHashValueShouldNotLeak(): void
    {
        $this->test->setHashValue([str_repeat('x', 64) . 0]);

        $before = memory_get_usage();
        for ($i = 1; $i <= 1000; $i++) {
            $this->test->setHashValue([str_repeat('x', 64) . $i]);
            $this->test->getHashValue();
        }
        $after = memory_get_usage();

        $this->assertSame(0, $after - $before);
        $this->assertSame([str_repeat('x', 64) . 1000], $this->test->getHashValue());

        $this->test->setHashValue(null);
    }

    public function testShouldSetGlobalExtensionOrmCacheLevel(): void
    {
        $this->test->setDefaultGlobalsOrmCacheLevel(3);

        $this->assertSame(3, $this->test->getDefaultGlobalsOrmCacheLevel());
    }
}
