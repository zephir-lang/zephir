<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Zephir\Support\TestCase;

class TryTest extends TestCase
{
    public function testThrow1()
    {
        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception', 'error');
        } else {
            $this->expectException('\Exception', 'error');
        }

        $t = new \Test\TryTest();
        $t->testThrow1();
    }

    public function testThrow2()
    {
        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception', 'error');
        } else {
            $this->expectException('\Exception', 'error');
        }

        $t = new \Test\TryTest();
        $t->testThrow2();
    }
}
