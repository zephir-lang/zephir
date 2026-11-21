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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

use function array_intersect;
use function array_map;
use function array_unique;
use function array_values;
use function count;
use function file_get_contents;
use function file_put_contents;
use function is_dir;
use function preg_match;
use function preg_match_all;
use function range;
use function sort;
use function sprintf;
use function str_replace;
use function strpos;
use function substr;

/**
 * Every static property access gets its own cache record, declared in the C
 * function that holds it and numbered from 0 there. The record is handed to
 * the kernel with the class the code is written in, which visibility is
 * checked against.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
final class Issue2805Test extends TestCase
{
    use RunsZephirCommands;

    private const BASE = <<<'ZEP'
        namespace Spropcache;

        class Base
        {
            public static items = [];
            protected static count = 0;
        }
        ZEP;

    private const CHILD = <<<'ZEP'
        namespace Spropcache;

        class Child extends Base
        {
            public static own = null;

            public static function sites(var v, var k) -> var
            {
                var r;

                let r = self::own;
                let self::own = v;
                let parent::count += 1;
                let parent::count -= 1;
                let self::own .= "x";
                let parent::items[] = v;
                let parent::items[k] = v;
                let parent::items[k][] = v;
                let parent::items[k] += 1;
                unset parent::items[k];
                natsort(parent::items);

                if isset self::own {
                    let r = 1;
                }

                return r;
            }

            public static function second() -> var
            {
                return self::own;
            }

            public static function third() -> bool
            {
                return isset self::own;
            }

            public static function fourth() -> bool
            {
                return empty self::own;
            }

            public function instanceIsset() -> bool
            {
                return isset this->{"x"};
            }

            public static function borrowAfterOwned() -> array
            {
                return [self::own, count(parent::items)];
            }

            public static function borrowAfterStringWrite() -> int
            {
                let self::own = "";

                return count(parent::items);
            }
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
        $this->removePath($this->outputDir() . '/spropcache');
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldGiveEveryStaticPropertyAccessItsOwnCache(): void
    {
        $sites = $this->functionBody($this->generate(), 'sites');
        $child = 'spropcache_child_ce, spropcache_child_ce';
        $base  = 'spropcache_base_ce, spropcache_child_ce';
        $site  = '&_zephir_sprop_cache\[\d+\]';

        $this->assertMatchesRegularExpression('/zephir_read_static_property_cached\(&r, ' . $child . ', SL\("own"\), ' . $site . ', PH_NOISY_CC\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_update_static_property_cached\(' . $child . ', SL\("own"\), ' . $site . ', v\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_add_static_property_cached\(' . $base . ', SL\("count"\), ' . $site . ', &_\d+\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_sub_static_property_cached\(' . $base . ', SL\("count"\), ' . $site . ', &_\d+\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_update_static_property_array_multi_cached\(' . $base . ', SL\("items"\), ' . $site . ', v, SL\("a"\), 1\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_update_static_property_array_multi_cached\(' . $base . ', SL\("items"\), ' . $site . ', v, SL\("z"\), 1, k\);/', $sites);
        $this->assertMatchesRegularExpression('/zephir_update_static_property_array_multi_cached\(' . $base . ', SL\("items"\), ' . $site . ', v, SL\("za"\), 2, k\);/', $sites);
        $this->assertMatchesRegularExpression('/= zephir_fetch_static_property_rw_cached\(' . $base . ', SL\("items"\), ' . $site . ', &/', $sites);
        $this->assertMatchesRegularExpression('/= zephir_fetch_static_property_write_cached\(' . $base . ', SL\("items"\), ' . $site . ', &/', $sites);
        $this->assertMatchesRegularExpression('/zephir_isset_static_property_cached\(' . $child . ', SL\("own"\), ' . $site . '\)/', $sites);
    }

    public function testShouldNotEmitTheUncachedStaticPropertyCalls(): void
    {
        $child = $this->generate();

        $uncached = [
            'zephir_read_static_property_ce(',
            'zephir_update_static_property_ce(',
            'zephir_add_static_property_ce(',
            'zephir_sub_static_property_ce(',
            'zephir_update_static_property_array_multi_ce(',
            'zephir_fetch_static_property_write_ce(',
            'zephir_fetch_static_property_rw_ce(',
        ];

        foreach ($uncached as $call) {
            $this->assertStringNotContainsString($call, $child);
        }
    }

    /**
     * A read and a write may throw PHP's Error or TypeError, and the method
     * stops there as PHP's does.
     */
    public function testShouldStopAfterAReadOrAWriteThatThrows(): void
    {
        $sites = $this->functionBody($this->generate(), 'sites');

        $this->assertMatchesRegularExpression('/zephir_read_static_property_cached\(&r, [^;]+;\s*if \(UNEXPECTED\(EG\(exception\)\)\)/', $sites);
        $this->assertMatchesRegularExpression('/zephir_update_static_property_cached\([^;]+, v\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/', $sites);
    }

    /**
     * `empty` asks silently, as PHP's ZEND_ISSET_ISEMPTY_STATIC_PROP does,
     * so an uninitialized typed property is empty instead of an Error.
     */
    public function testShouldReadSilentlyForEmpty(): void
    {
        $this->assertMatchesRegularExpression(
            '/zephir_read_static_property_cached\(&_\d+, spropcache_child_ce, spropcache_child_ce, SL\("own"\), &_zephir_sprop_cache\[0\], PH_READONLY\);/',
            $this->functionBody($this->generate(), 'fourth')
        );
    }

    public function testShouldAskTheObjectForIssetOfAStringNamedProperty(): void
    {
        $body = $this->functionBody($this->generate(), 'instanceIsset');

        $this->assertStringNotContainsString('(0 == 0)', $body);
        $this->assertMatchesRegularExpression('/zephir_isset_property_value_fast\(this_ptr, _zephir_prop_\d+\)/', $body);
    }

    /**
     * A borrowed read (PH_READONLY) must land in a variable the memory frame
     * does not release, or the end of the method frees the property's value.
     * A temp first used for an owned read, or for the string of a static
     * property write, used to be handed out again for a borrowed read.
     */
    public function testShouldNeverBorrowIntoATrackedVariable(): void
    {
        $child = $this->generate();

        foreach (['sites', 'second', 'fourth', 'borrowAfterOwned', 'borrowAfterStringWrite'] as $method) {
            $body = $this->functionBody($child, $method);

            preg_match_all('/zephir_read_static_property_cached\(&(\w+(?:\$\$\d+)?), [^;]*PH_READONLY\);/', $body, $borrowed);
            preg_match_all('/(?:ZEPHIR_INIT_N?VAR|ZEPHIR_OBS_N?VAR|zephir_memory_observe)\(&(\w+(?:\$\$\d+)?)\);/', $body, $tracked);

            $this->assertSame([], array_values(array_intersect($borrowed[1], $tracked[1])), $method);
        }
    }

    /**
     * Each C function declares exactly as many records as it has accesses
     * and numbers them from 0, so editing one method renumbers nothing else.
     */
    public function testShouldNumberTheCachesWithinEachFunction(): void
    {
        $child = $this->generate();

        foreach (['sites', 'second', 'third', 'fourth', 'borrowAfterOwned', 'borrowAfterStringWrite'] as $method) {
            $body = $this->functionBody($child, $method);

            $this->assertSame(1, preg_match('/ZEND_TLS zephir_static_property_cache _zephir_sprop_cache\[(\d+)\];/', $body, $declaration), $method);
            preg_match_all('/&_zephir_sprop_cache\[(\d+)\]/', $body, $uses);

            $indexes = array_unique(array_map('intval', $uses[1]));
            sort($indexes);

            $this->assertSame(range(0, (int) $declaration[1] - 1), $indexes, $method);
            $this->assertSame(count($uses[1]), count($indexes), $method . ' shares a record between two accesses');
        }

        $this->assertStringNotContainsString('_zephir_sprop_cache', $this->functionBody($child, 'instanceIsset'));
    }

    private function generate(): string
    {
        $projectDir = $this->outputDir() . '/spropcache';

        if (!is_dir($projectDir)) {
            $this->assertSame(0, $this->runZephir('init spropcache', $this->outputDir())['exitCode']);
            $this->cleanupPath($projectDir);
        }

        file_put_contents($projectDir . '/spropcache/base.zep', self::BASE . "\n");
        file_put_contents($projectDir . '/spropcache/child.zep', self::CHILD . "\n");

        /* The parsed-file cache goes by modification time, which a rewrite within the same second leaves alone. */
        $this->removePath($projectDir . '/.zephir');

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        /* Windows writes the generated C with CRLF line endings. */
        return str_replace("\r\n", "\n", (string) file_get_contents($projectDir . '/ext/spropcache/child.zep.c'));
    }

    private function functionBody(string $source, string $method): string
    {
        $start = strpos($source, sprintf('PHP_METHOD(Spropcache_Child, %s)', $method));
        $this->assertNotFalse($start, 'No method ' . $method);

        $end = strpos($source, "\n}\n", $start);

        return substr($source, $start, false === $end ? null : $end - $start);
    }
}
