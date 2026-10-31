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

use function array_filter;
use function array_map;
use function count;
use function explode;
use function file_get_contents;
use function file_put_contents;
use function is_dir;
use function preg_match;
use function preg_match_all;
use function range;
use function sort;
use function sprintf;
use function str_contains;
use function str_replace;
use function strpos;
use function substr;

/**
 * Every method and static call site whose method name is known at compile
 * time gets its own inline cache record, declared in the C function that
 * holds the site and numbered from 0 there.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2790
 */
final class Issue2790Test extends TestCase
{
    use RunsZephirCommands;

    private const BASE = <<<'ZEP'
        namespace Callcache;

        class Base
        {
            public function __construct()
            {
            }

            public function who() -> string
            {
                return "base";
            }

            public static function sname() -> string
            {
                return "Base";
            }
        }
        ZEP;

    private const CHILD = <<<'ZEP'
        namespace Callcache;

        class Child extends Base
        {
            public function __construct()
            {
                parent::__construct();
            }

            public function sites(var o, var it, string name, string cls) -> string
            {
                var k, v, made;
                string out = "";

                let out .= this->who();
                let out .= o->who();
                let made = new Base();
                let out .= self::sname();
                let out .= static::sname();
                let out .= parent::who();
                let out .= Base::sname();
                let out .= {cls}::sname();
                let out .= o->{name}();
                let out .= {cls}::{name}();

                for k, v in it {
                    let out .= k . v;
                }

                return out;
            }

            public function second(var o) -> string
            {
                return o->who();
            }

            public static function third() -> string
            {
                return static::sname();
            }

            public function fourth() -> string
            {
                return parent::who();
            }

            public static function fifth() -> string
            {
                return self::sname() . Base::sname();
            }
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
        $this->removePath($this->outputDir() . '/callcache');
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldGiveEveryCallSiteItsOwnCache(): void
    {
        $sites = $this->functionBody($this->generate()['child'], 'sites');

        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_METHOD_CACHED\(&_\d+, this_ptr, "who", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_METHOD_CACHED\(&_\d+, o, "who", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_METHOD_CACHED\(NULL, &made, "__construct", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_SELF_CACHED\(&_\d+, "sname", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_STATIC_CACHED\(&_\d+, "sname", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_PARENT_CACHED\(&_\d+, callcache_child_ce, getThis\(\), "who", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_CE_STATIC_CACHED\(&_\d+, callcache_base_ce, "sname", &_zephir_call_cache\[\d+\]\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_CE_STATIC_CACHED\(&_\d+, _\d+, "sname", &_zephir_call_cache\[\d+\]\);/', $sites);

        foreach (['rewind', 'next', 'valid', 'key', 'current'] as $iteratorMethod) {
            $this->assertMatchesRegularExpression(
                sprintf('/ZEPHIR_CALL_METHOD_CACHED\((NULL|&\w+), _\d+, "%s", &_zephir_call_cache\[\d+\]\);/', $iteratorMethod),
                $sites,
            );
        }
    }

    public function testShouldGiveTheReturnFormsTheirOwnCache(): void
    {
        $child = $this->generate()['child'];

        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_METHOD_CACHED(o, "who", &_zephir_call_cache[0]);',
            $this->functionBody($child, 'second'),
        );
        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_STATIC_CACHED("sname", &_zephir_call_cache[0]);',
            $this->functionBody($child, 'third'),
        );
        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_PARENT_CACHED(callcache_child_ce, getThis(), "who", &_zephir_call_cache[0]);',
            $this->functionBody($child, 'fourth'),
        );
        $this->assertStringContainsString(
            'ZEPHIR_CALL_PARENT_CACHED(NULL, callcache_child_ce, getThis(), "__construct", &_zephir_call_cache[0]);',
            $this->functionBody($child, '__construct'),
        );
    }

    /**
     * A name only known at run time can resolve to a different method on
     * every call, so those sites keep the uncached call.
     */
    public function testShouldNotCacheACallWhoseMethodNameIsAVariable(): void
    {
        $sites = $this->functionBody($this->generate()['child'], 'sites');

        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_METHOD_ZVAL\(&_\d+, o, &name_zv, NULL, 0\);/', $sites);
        $this->assertMatchesRegularExpression('/ZEPHIR_CALL_CE_STATIC_ZVAL\(&_\d+, _\d+, &name_zv, NULL, 0\);/', $sites);
    }

    /**
     * The old per-call `zephir_fcall_cache_entry` locals and the uncached
     * macros are gone for every site with a known name.
     */
    public function testShouldNotEmitTheOldMethodCaches(): void
    {
        $child = $this->generate()['child'];

        $this->assertStringNotContainsString('zephir_fcall_cache_entry', $child);
        $this->assertDoesNotMatchRegularExpression(
            '/ZEPHIR_(RETURN_)?CALL_(METHOD|SELF|STATIC|PARENT|CE_STATIC)\(/',
            $child,
        );
    }

    /**
     * Each C function declares exactly as many records as it has sites and
     * uses each of them once, starting from 0.
     */
    public function testShouldNumberTheCachesWithinEachFunction(): void
    {
        $child     = $this->generate()['child'];
        $functions = array_filter(
            explode("\n}\n", $child),
            static fn (string $function): bool => str_contains($function, '_zephir_call_cache'),
        );

        $this->assertGreaterThanOrEqual(6, count($functions));

        foreach ($functions as $function) {
            $this->assertSame(
                1,
                preg_match('/ZEND_TLS zephir_method_cache _zephir_call_cache\[(\d+)\];/', $function, $declaration),
                $function,
            );
            $this->assertSame(1, preg_match_all('/zephir_method_cache _zephir_call_cache\[/', $function));

            preg_match_all('/&_zephir_call_cache\[(\d+)\]/', $function, $uses);
            $indexes = array_map('intval', $uses[1]);
            sort($indexes);

            $this->assertSame(range(0, (int) $declaration[1] - 1), $indexes, $function);
        }
    }

    /**
     * Adding a call to one file leaves the generated C of every other file as
     * it was, so an edit does not recompile the whole extension.
     */
    public function testShouldNotRenumberOtherFilesWhenACallIsAdded(): void
    {
        $before = $this->generate()['child'];

        $base = str_replace(
            'public static function sname()',
            "public function extra(var o) -> string\n    {\n        return o->who() . this->who();\n    }\n\n    public static function sname()",
            self::BASE,
        );

        $after = $this->generate($base);

        $this->assertStringContainsString('"who", &_zephir_call_cache[1]', $after['base']);
        $this->assertSame($before, $after['child']);
    }

    /**
     * @return array{base: string, child: string}
     */
    private function generate(string $base = self::BASE): array
    {
        $projectDir = $this->outputDir() . '/callcache';

        if (!is_dir($projectDir)) {
            $this->assertSame(0, $this->runZephir('init callcache', $this->outputDir())['exitCode']);
            $this->cleanupPath($projectDir);
        }

        file_put_contents($projectDir . '/callcache/base.zep', $base . "\n");
        file_put_contents($projectDir . '/callcache/child.zep', self::CHILD . "\n");

        /* The parsed-file cache goes by modification time, which a rewrite within the same second leaves alone. */
        $this->removePath($projectDir . '/.zephir');

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        /* Windows writes the generated C with CRLF line endings. */
        return [
            'base'  => str_replace("\r\n", "\n", (string) file_get_contents($projectDir . '/ext/callcache/base.zep.c')),
            'child' => str_replace("\r\n", "\n", (string) file_get_contents($projectDir . '/ext/callcache/child.zep.c')),
        ];
    }

    private function functionBody(string $source, string $method): string
    {
        $start = strpos($source, sprintf('PHP_METHOD(Callcache_Child, %s)', $method));
        $this->assertNotFalse($start, 'No method ' . $method);

        $end = strpos($source, "\n}\n", $start);

        return substr($source, $start, false === $end ? null : $end - $start);
    }
}
