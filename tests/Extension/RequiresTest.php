<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Requires;

class RequiresTest extends TestCase
{
    /** @var Requires */
    private $test;

    protected function setUp(): void
    {
        $this->test = new Requires();
    }

    public function testRequireExternal1()
    {
        $this->assertSame(
            [1, 2, 3],
            $this->test->requireExternal1(__DIR__.'/../fixtures/require-me-1.php')
        );

        $this->assertFalse(\defined('REQUIRE_ME'));
        $this->test->requireExternal1(__DIR__.'/../fixtures/require-me-2.php');
        $this->assertTrue(\defined('REQUIRE_ME'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/pull/1428
     */
    public function testShouldRequireUsingNewSymbolTable()
    {
        ob_start();
        $actual = $this->test->requireExternal3(__DIR__.'/../fixtures/require-me-3.php');
        ob_end_clean();

        $this->assertSame('test', $actual);
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1621
     */
    public function testShouldRenderTemplate()
    {
        $param = 1;

        $this->assertEquals(
            2,
            $this->test->renderTemplate(__DIR__.'/../fixtures/template.php', ['a' => 2])
        );

        $this->assertEquals(1, $param);
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1713
     */
    public function testShouldRequirePhar()
    {
        $this->assertEquals(
            "I'm in",
            $this->test->requireExternal1('phar://'.__DIR__.'/../fixtures/requires/myapp.phar/index.php')
        );
    }
}
