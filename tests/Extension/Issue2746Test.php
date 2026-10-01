<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Issue2746;
use TypeError;

use function extension_loaded;
use function fdiv;
use function fopen;
use function gmp_init;
use function preg_replace;

/**
 * PHP's `(int)` and `(float)` run an object's own cast handler and warn when
 * it has none, saturate a float string beyond the int range, and read a
 * resource as its handle. Every Zephir conversion of a zval to `long` or
 * `double` must do the same. Each assertion compares the extension against
 * the PHP cast.
 *
 * A `double` parameter must coerce its argument as any internal `float`
 * parameter does, so `fdiv(a, 1.0)` is the oracle there, called from both a
 * strict and a weak mode file.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2746
 */
final class Issue2746Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2746 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2746();
    }

    public static function valueProvider(): array
    {
        $cases = [
            'int'                => [7],
            'float'              => [7.5],
            'numeric string'     => ['7'],
            'leading whitespace' => [' 12'],
            'leading numeric'    => ['12abc'],
            'non-numeric'        => ['abc'],
            'empty string'       => [''],
            'huge float string'  => ['1e100'],
            'tiny float string'  => ['-1e100'],
            'inf float string'   => ['1e1000'],
            'hex string'         => ['0x1A'],
            'fraction string'    => ['.5'],
            'null'               => [null],
            'true'               => [true],
            'false'              => [false],
            'empty array'        => [[]],
            'array'              => [[1, 2]],
            'object'             => [new stdClass()],
            'closure'            => [static fn () => 1],
            'resource'           => [fopen('php://memory', 'r')],
        ];

        if (extension_loaded('gmp')) {
            $cases['gmp']      = [gmp_init(7)];
            $cases['huge gmp'] = [gmp_init('99999999999999999999')];
        }

        return $cases;
    }

    /**
     * @dataProvider valueProvider
     */
    public function testLongLocalMatchesIntCast(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->toLong($a), fn () => (int) $a);
    }

    /**
     * @dataProvider valueProvider
     */
    public function testIntCastMatchesPhp(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->castInt($a), fn () => (int) $a);
    }

    /**
     * @dataProvider valueProvider
     */
    public function testIntvalMatchesPhp(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->intvalOf($a), fn () => intval($a));
    }

    /**
     * @dataProvider valueProvider
     */
    public function testDoubleLocalMatchesFloatCast(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->toDouble($a), fn () => (float) $a);
    }

    /**
     * @dataProvider valueProvider
     */
    public function testDoubleCastMatchesPhp(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->castDouble($a), fn () => (float) $a);
    }

    /**
     * @dataProvider valueProvider
     */
    public function testDoublevalMatchesPhp(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->doublevalOf($a), fn () => doubleval($a));
    }

    /**
     * @dataProvider valueProvider
     */
    public function testDoubleParamStrictMatchesPhp(mixed $a): void
    {
        $this->assertSame(
            $this->parameterTranscript(fn () => fdiv($a, 1.0)),
            $this->parameterTranscript(fn () => $this->test->doubleParam($a))
        );
    }

    /**
     * @dataProvider valueProvider
     */
    public function testDoubleParamWeakMatchesPhp(mixed $a): void
    {
        $call = require __DIR__ . '/../fixtures/weak-mode-call.php';

        $this->assertSame(
            $this->parameterTranscript(fn () => $call('fdiv', $a, 1.0)),
            $this->parameterTranscript(fn () => $call([$this->test, 'doubleParam'], $a))
        );
    }

    /**
     * @dataProvider valueProvider
     */
    public function testOptionalDoubleParamMatchesPhp(mixed $a): void
    {
        $this->assertSame(
            $this->parameterTranscript(fn () => fdiv($a, 1.0)),
            $this->parameterTranscript(fn () => $this->test->optionalDoubleParam($a))
        );
    }

    public function testOptionalDoubleParamKeepsItsDefault(): void
    {
        $this->assertSame(1.5, $this->test->optionalDoubleParam());
    }

    public function testNullableDoubleParamAcceptsNull(): void
    {
        $this->assertSame(0.0, $this->test->nullableDoubleParam(null));
    }

    public function testNullableDoubleParamRejectsObject(): void
    {
        $this->expectException(TypeError::class);
        $this->expectExceptionMessage('must be of type ?float, stdClass given');

        $this->test->nullableDoubleParam(new stdClass());
    }

    /**
     * The diagnostics name the function and its parameter; drop both so a
     * Zephir method compares against the PHP function.
     */
    private function parameterTranscript(callable $subject): string
    {
        return preg_replace(
            ['/[\w\\\\]+(::\w+)?\(\): /', '/\(\$\w+\)/'],
            ['', '($a)'],
            $this->transcript($subject)
        );
    }
}
