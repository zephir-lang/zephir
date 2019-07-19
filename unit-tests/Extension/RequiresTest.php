<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Requires;

class RequiresTest extends TestCase
{
    public function testRequireExternal1()
    {
        $r = new Requires();

        $this->assertSame(
            [1, 2, 3],
            $r->requireExternal1(__DIR__.'/../fixtures/require-me-1.php')
        );

        $this->assertFalse(\defined('REQUIRE_ME'));
        $r->requireExternal1(__DIR__.'/../fixtures/require-me-2.php');
        $this->assertTrue(\defined('REQUIRE_ME'));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/pull/1428
     */
    public function shouldRequireUsingNewSymbolTable()
    {
        if (\PHP_VERSION_ID >= 70400) {
            $this->markTestSkipped(
                "Does not ready for PHP >= 70400"
            );
        }

        $r = new Requires();

        $this->assertSame(
            'test',
            $r->requireExternal3(__DIR__.'/../fixtures/require-me-3.php')
        );
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1621
     */
    public function shouldRenderTemplate()
    {
        if (\PHP_VERSION_ID >= 70400) {
            $this->markTestSkipped(
                "Does not ready for PHP >= 70400"
            );
        }

        $r = new Requires();
        $a = 1;

        $this->assertEquals(
            2,
            $r->renderTemplate(__DIR__.'/../fixtures/template.php', ['a' => 2])
        );

        $this->assertEquals(1, $a);
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1713
     */
    public function shouldRequirePhar()
    {
        $r = new Requires();

        $this->assertEquals(
            "I'm in",
            $r->requireExternal1('phar://'.__DIR__.'/../fixtures/requires/myapp.phar/index.php')
        );
    }
}
