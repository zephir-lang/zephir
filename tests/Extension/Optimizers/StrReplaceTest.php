<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\StrReplace;

class StrReplaceTest extends TestCase
{
    public function testIssue1055()
    {
        $this->assertSame(StrReplace::issue1055(), 'bla bla aaa aaaa askks');
    }

    public function issue1087()
    {
        $this->assertSame(StrReplace::issue1087(), 'test_test');
    }

    public function issue732()
    {
        $this->assertSame(StrReplace::issue732A(), 'F');
        $this->assertSame(StrReplace::issue732B(), 'The slow black bear jumped over the lazy dog.');
    }
}
