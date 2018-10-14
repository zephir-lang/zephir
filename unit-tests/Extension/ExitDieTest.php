<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

class ExitDieTest extends \PHPUnit_Framework_TestCase
{
    public function testExitDie()
    {
        if (PHP_VERSION_ID >= 70300) {
            $this->markTestSkipped("Skipped due to PHP 7.3 stable is not released yet");
        }

        $phpBinary = constant('PHP_BINARY');
        /* If we use phpdbg, you need to add options -qrr */
        if (defined('PHP_SAPI') && constant('PHP_SAPI') == 'phpdbg') {
            $phpBinary .= ' -qrr';
        }

        $phpBinary .= " -d 'enable_dl=true'";

        $testfile1 = __DIR__ .'/fixtures/exit.php';
        $return1 = `$phpBinary $testfile1`;
        $this->assertSame('', trim($return1));

        $arg = 'Hello World';
        $testfile2 = __DIR__ .'/fixtures/exit_string.php';
        $return2 = `$phpBinary $testfile2 "$arg"`;
        $this->assertSame($arg, trim($return2));

        $testfile3 = __DIR__ .'/fixtures/exit_int.php';
        $intArg = 220;
        $cmd3 = "$phpBinary $testfile3 $intArg";
        $out3 = [];

        exec($cmd3, $out3, $return3);
        if (isset($out3[0]) && !empty($out3[0])) {
            $this->fail(sprintf('Failed executing command: %s: %s', $cmd3, $out3[0]));
        }

        $this->assertSame($intArg, $return3);
    }
}
