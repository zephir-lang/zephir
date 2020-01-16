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

class VarsTest extends TestCase
{
    /**
     * Test var_dump function (array and string).
     */
    public function testVarDumpArrayAndString()
    {
        $this->setOutputCallback(
            function ($output) {
                /* To prevent differences between PHP version */
                return str_replace(["\n", ' '], '', $output);
            }
        );

        $this->expectOutputString(
            'array(3){'.
            '[0]=>int(1)'.
            '[1]=>string(5)"world"'.
            '[2]=>bool(false)}'.
            'string(5)"hello"'
        );

        $t = new \Test\Vars();

        $t->testVarDump();
    }

    /**
     * Test var_dump function with two params.
     */
    public function testVarDumpWithTwoParams()
    {
        $this->expectOutputString(
            'float(3.1)'."\n".
            'bool(true)'."\n"
        );

        $t = new \Test\Vars();

        $t->testVarDump2param(3.1, true);
    }

    /**
     * Test var_dump function with three params.
     */
    public function testVarDumpWithThreeParams()
    {
        $this->setOutputCallback(
            function ($output) {
                return str_replace(["\n", ' '], '', $output);
            }
        );

        $this->expectOutputString(
            'float(3.1)'.
            'bool(true)'.
            'array(3){[0]=>int(1)[1]=>int(2)[2]=>int(3)}'
        );

        $t = new \Test\Vars();

        $t->testVarDump3param(3.1, true, [1, 2, 3]);
    }

    public function testVarExport()
    {
        $this->setOutputCallback(
            function ($output) {
                return str_replace(["\n", ' '], '', $output);
            }
        );

        $this->expectOutputString(
            "array(0=>1,1=>'world',2=>false,)'hello'"
        );

        $t = new \Test\Vars();
        $this->assertSame($t->testVarExport(), "'hello'");
    }

    public function test88Issue()
    {
        $t = new \Test\Vars();

        $this->expectOutputString(
            'string(3) "foo"'."\n".
            'string(3) "bar"'."\n".
            "'foo'".
            "'bar'".
            "'bar'"
        );

        $t->test88Issue('foo', 'bar');

        $t->test88IssueParam2InitString('foo', 'bar');
    }

    public function testVarDumpAndCountOptimizer()
    {
        $this->expectOutputString(
            'int(5)'."\n".
            '5'
        );

        $t = new \Test\Vars();
        $t->testCountOptimizerVarDumpAndExport([1, 2, 3, 4, 5]);
    }

    public function testArrayTypeVarDumpAndExport()
    {
        $this->setOutputCallback(
            function ($output) {
                return str_replace(["\n", ' '], '', $output);
            }
        );

        $this->expectOutputString(
            'array(3){[0]=>int(1)[1]=>int(2)[2]=>int(3)}'.
            'array(0=>1,1=>2,2=>3,)'
        );

        $t = new \Test\Vars();
        $t->testArrayTypeVarDumpAndExport([1, 2, 3]);
    }

    public function testIntVarDump()
    {
        $this->expectOutputString(
            'int(1)'."\n".
            '1'
        );

        $t = new \Test\Vars();
        $t->testIntVarDump();
    }

    public function testDoubleVarDump()
    {
        if (\PHP_VERSION_ID < 70000) {
            $this->expectOutputString(
                'float(1)'."\n".
                '1'
            );
        } else {
            $this->expectOutputString(
                'float(1)'."\n".
                '1.0'
            );
        }

        $t = new \Test\Vars();
        $t->testDoubleVarDump();
    }

    public function testBoolVarDump()
    {
        $this->expectOutputString(
            'bool(true)'."\n".
            'true'
        );

        $t = new \Test\Vars();
        $t->testBoolVarDump();
    }

    public function testGetDefinedVars()
    {
        $t = new \Test\Vars();
        // get_defined_vars() does NOT work in C (and hence Zephir) functions
        // It will return variables of the most recent userland function
        // down the call stack.
        // In this case, all local variables of this method will be returned
        $this->assertEquals(['t' => $t], $t->testGetDefinedVars());
    }
}
