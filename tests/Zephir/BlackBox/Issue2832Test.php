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

/**
 * `fetch` takes a property name in every form `isset` takes it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2832
 */
final class Issue2832Test extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testFetchOfAPropertyNamedByAStringLiteralCompiles(): void
    {
        $source = $this->generateOk('fetchname.zep', <<<'ZEP'
            namespace Stub;

            class FetchName
            {
                public function run(var o)
                {
                    var c;

                    return fetch c, o->{"x"};
                }
            }
            ZEP);

        $this->assertStringContainsString('zephir_fetch_property(&c, o, SL("x"), PH_SILENT_CC)', $source);
    }

    public function testFetchOfAPropertyNamedByAnIntCompiles(): void
    {
        $source = $this->generateOk('fetchname.zep', <<<'ZEP'
            namespace Stub;

            class FetchName
            {
                public function run(var o, int i)
                {
                    var c;

                    return fetch c, o->{i};
                }
            }
            ZEP);

        $this->assertMatchesRegularExpression('/zephir_fetch_property_zval\(&c, o, &_\d+, PH_SILENT_CC\)/', $source);
    }

    /**
     * A `fetch` statement drops its result: once the fetch and its exception
     * check are emitted, a bare `_0;` would be a statement with no effect.
     */
    public function testFetchStatementLeavesNoStatementWithoutEffect(): void
    {
        $source = $this->generateOk('fetchname.zep', <<<'ZEP'
            namespace Stub;

            class FetchName
            {
                public function run(var o, var a, var n)
                {
                    var c, d;

                    fetch c, o->{n};
                    fetch d, a[n];

                    return [c, d];
                }
            }
            ZEP);

        $this->assertSame(2, preg_match_all('/_\d+ = zephir_(fetch_property_zval|array_isset_fetch)\(/', $source));
        $this->assertDoesNotMatchRegularExpression('/^\s*_\d+;$/m', $source);
    }
}
