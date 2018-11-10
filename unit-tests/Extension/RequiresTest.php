<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Test\Requires;
use Zephir\Support\TestCase;

class RequiresTest extends TestCase
{
    public function testRequireExternal1()
    {
        $r = new Requires();

        $this->assertSame(
            [1, 2, 3],
            $r->requireExternal1(__DIR__ . '/../fixtures/require-me-1.php')
        );

        $this->assertFalse(defined('REQUIRE_ME'));
        $r->requireExternal1(__DIR__ . '/../fixtures/require-me-2.php');
        $this->assertTrue(defined('REQUIRE_ME'));
    }

    public function testRequireExternal3()
    {
        $r = new Requires();

        $this->assertSame(
            'test',
            $r->requireExternal3(__DIR__ . '/../fixtures/require-me-3.php')
        );
    }
}
