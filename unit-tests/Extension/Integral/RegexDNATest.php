<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension\Integral;

class RegexDNATest extends \PHPUnit_Framework_TestCase
{
    public function testProcess()
    {
        $t = new \Test\RegexDNA();

        ob_start();
        $t->process(__DIR__ . '/regexdna-input.txt');
        $content = ob_get_clean();

        $this->assertSame($content, file_get_contents(__DIR__ . '/regexdna-output.txt'));
    }
}
