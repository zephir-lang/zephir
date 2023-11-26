<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Vars;

final class VarsTest extends TestCase
{
    /**
     * Test var_dump function (array and string).
     */
    public function testVarDumpArrayAndString(): void
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

        $test = new Vars();
        $test->testVarDump();
    }

    /**
     * Test var_dump function with two params.
     */
    public function testVarDumpWithTwoParams(): void
    {
        $this->expectOutputString(
            'float(3.1)'."\n".
            'bool(true)'."\n"
        );

        $test = new Vars();
        $test->testVarDump2param(3.1, true);
    }

    /**
     * Test var_dump function with three params.
     */
    public function testVarDumpWithThreeParams(): void
    {
        $this->setOutputCallback(
            fn ($output) => str_replace(["\n", ' '], '', $output)
        );

        $this->expectOutputString(
            'float(3.1)'.
            'bool(true)'.
            'array(3){[0]=>int(1)[1]=>int(2)[2]=>int(3)}'
        );

        $test = new Vars();
        $test->testVarDump3param(3.1, true, [1, 2, 3]);
    }

    public function testVarExport(): void
    {
        $this->setOutputCallback(
            fn ($output) => str_replace(["\n", ' '], '', $output)
        );

        $this->expectOutputString(
            "array(0=>1,1=>'world',2=>false,)'hello'"
        );

        $test = new Vars();
        $this->assertSame($test->testVarExport(), "'hello'");
    }

    public function test88Issue(): void
    {
        $test = new Vars();

        $this->expectOutputString(
            'string(3) "foo"'."\n".
            'string(3) "bar"'."\n".
            "'foo'".
            "'bar'".
            "'bar'"
        );

        $test->test88Issue('foo', 'bar');
        $test->test88IssueParam2InitString('foo', 'bar');
    }

    public function testVarDumpAndCountOptimizer(): void
    {
        $this->expectOutputString(
            'int(5)'."\n".
            '5'
        );

        $test = new Vars();
        $test->testCountOptimizerVarDumpAndExport([1, 2, 3, 4, 5]);
    }

    public function testArrayTypeVarDumpAndExport(): void
    {
        $this->setOutputCallback(
            fn ($output) => str_replace(["\n", ' '], '', $output)
        );

        $this->expectOutputString(
            'array(3){[0]=>int(1)[1]=>int(2)[2]=>int(3)}'.
            'array(0=>1,1=>2,2=>3,)'
        );

        $test = new Vars();
        $test->testArrayTypeVarDumpAndExport([1, 2, 3]);
    }

    public function testIntVarDump(): void
    {
        $this->expectOutputString(
            'int(1)'."\n".
            '1'
        );

        $test = new Vars();
        $test->testIntVarDump();
    }

    public function testDoubleVarDump(): void
    {
        $this->expectOutputString(
            'float(1)'."\n".
            '1.0'
        );

        $test = new Vars();
        $test->testDoubleVarDump();
    }

    public function testBoolVarDump(): void
    {
        $this->expectOutputString(
            'bool(true)'."\n".
            'true'
        );

        $test = new Vars();
        $test->testBoolVarDump();
    }

    public function testGetDefinedVars(): void
    {
        $test = new Vars();

        /**
         * get_defined_vars() does NOT work in C (and hence Zephir) functions
         * It will return variables of the most recent userland function
         * down the call stack.
         * In this case, all local variables of this method will be returned
         */
        $this->assertEquals(['test' => $test], $test->testGetDefinedVars());
    }
}
