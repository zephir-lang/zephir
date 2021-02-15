<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\ScallLateConstruct;

class ScallLateConstructTest extends TestCase
{
    /**
     * @see https://github.com/phalcon/zephir/issues/2111
     */    
    public function testConstruct()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            //$this->markTestSkipped('Should be fixed static call before run this test (Invalid callback , no array or string given)');
        }

        $test = new ScallLateConstruct();
        $this->assertSame('hello public', $test->varValue());
    }
}
