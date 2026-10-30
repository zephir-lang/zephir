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

use ErrorException;
use PHPUnit\Framework\TestCase;
use RuntimeException;
use stdClass;
use Stub\Issue2684;
use Stringable;

use function fopen;
use function ob_get_clean;
use function ob_start;
use function restore_error_handler;
use function set_error_handler;

use const E_DEPRECATED;
use const NAN;

/**
 * PHP's `explode()` reads its arguments with `Z_PARAM_STR`, `Z_PARAM_STR` and
 * `Z_PARAM_LONG`: scalars and Stringable objects are coerced, `null` is
 * coerced with a deprecation from 8.1, and arrays, plain objects and
 * resources are a `TypeError`. Zephir accepted only strings and answered
 * everything else with a warning and an empty string.
 *
 * Zephir code is never a strict_types file, so PHP is called through the
 * weak-mode fixture: this test file is strict and would reject the very
 * coercions under test.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2684
 */
final class Issue2684Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2684 $test;

    /**
     * @var callable
     */
    private $call;

    protected function setUp(): void
    {
        $this->test = new Issue2684();
        $this->call = require __DIR__ . '/../fixtures/weak-mode-call.php';
    }

    /**
     * Factories rather than values: a resource cannot live in a data provider.
     */
    public static function stringArgumentProvider(): array
    {
        return [
            'string'             => [static fn () => 'a1b,1c'],
            'int'                => [static fn () => 12],
            'int zero'           => [static fn () => 0],
            'separator int'      => [static fn () => 1],
            'float'              => [static fn () => 1.5],
            'float exponent'     => [static fn () => 1e100],
            'negative zero'      => [static fn () => -0.0],
            'true'               => [static fn () => true],
            'false'              => [static fn () => false],
            'null'               => [static fn () => null],
            'stringable'         => [static fn () => new class () implements Stringable {
                public function __toString(): string
                {
                    return 'a,b';
                }
            }],
            'throwing stringable' => [static fn () => new class () {
                public function __toString(): string
                {
                    throw new RuntimeException('no string');
                }
            }],
            'array'              => [static fn () => [1, 2]],
            'plain object'       => [static fn () => new stdClass()],
            'resource'           => [static fn () => fopen('php://memory', 'r')],
        ];
    }

    public static function limitProvider(): array
    {
        return [
            'int'             => [static fn () => 2],
            'numeric string'  => [static fn () => '2'],
            'leading space'   => [static fn () => ' 2'],
            'leading numeric' => [static fn () => '2x'],
            'non-numeric'     => [static fn () => 'x'],
            'float string'    => [static fn () => '1.5'],
            'float'           => [static fn () => 1.5],
            'integral float'  => [static fn () => 2.0],
            'nan'             => [static fn () => NAN],
            'huge float'      => [static fn () => 1e30],
            'null'            => [static fn () => null],
            'true'            => [static fn () => true],
            'false'           => [static fn () => false],
            'array'           => [static fn () => []],
            'plain object'    => [static fn () => new stdClass()],
            'resource'        => [static fn () => fopen('php://memory', 'r')],
        ];
    }

    /**
     * @dataProvider stringArgumentProvider
     */
    public function testSubjectMatchesPhp(callable $make): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeNoLimit(',', $make()),
            fn () => ($this->call)('explode', ',', $make())
        );
    }

    /**
     * @dataProvider stringArgumentProvider
     */
    public function testSubjectWithLiteralSeparatorMatchesPhp(callable $make): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeStr($make()),
            fn () => ($this->call)('explode', ',', $make())
        );
    }

    /**
     * @dataProvider stringArgumentProvider
     */
    public function testSeparatorMatchesPhp(callable $make): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeNoLimit($make(), 'a1b,1c'),
            fn () => ($this->call)('explode', $make(), 'a1b,1c')
        );
    }

    /**
     * @dataProvider limitProvider
     */
    public function testLimitMatchesPhp(callable $make): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit(',', 'a,b,c', $make()),
            fn () => ($this->call)('explode', ',', 'a,b,c', $make())
        );
    }

    /**
     * @dataProvider limitProvider
     */
    public function testLimitWithLiteralSeparatorMatchesPhp(callable $make): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeStrLimit('a,b,c', $make()),
            fn () => ($this->call)('explode', ',', 'a,b,c', $make())
        );
    }

    public function testConstantLimitMatchesPhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeConstLimit('a,b,c'),
            fn () => explode(',', 'a,b,c', 2)
        );
    }

    /**
     * PHP stops at the first argument it cannot parse.
     */
    public function testFirstBadArgumentWins(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit([], new stdClass(), 'x'),
            fn () => ($this->call)('explode', [], new stdClass(), 'x')
        );
    }

    public function testBadSubjectIsReportedBeforeBadLimit(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit(',', [], 'x'),
            fn () => ($this->call)('explode', ',', [], 'x')
        );
    }

    /**
     * The empty separator ValueError comes after every argument is parsed.
     */
    public function testTypeErrorPrecedesEmptySeparator(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeLimit('', 'a', []),
            fn () => ($this->call)('explode', '', 'a', [])
        );
    }

    public function testNullCoercesToEmptySeparatorValueError(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->explodeNoLimit(null, 'a'),
            fn () => ($this->call)('explode', null, 'a')
        );
    }

    /**
     * A deprecation turned into an exception stops the call without a
     * TypeError on top of it.
     */
    public function testThrowingDeprecationHandlerMatchesPhp(): void
    {
        $this->assertSame(
            $this->throwingDeprecations(fn () => ($this->call)('explode', ',', null, null)),
            $this->throwingDeprecations(fn () => $this->test->explodeLimit(',', null, null))
        );
    }

    /**
     * Coercion works on a copy; the caller's variable keeps its type.
     */
    public function testArgumentsAreNotModified(): void
    {
        $separator = 1;
        $subject   = 1.5;
        $limit     = '2';

        $this->test->explodeLimit($separator, $subject, $limit);

        $this->assertSame([1, 1.5, '2'], [$separator, $subject, $limit]);
    }

    public function testExecutionStopsAfterThrow(): void
    {
        ob_start();
        try {
            $this->test->stopsAfterThrow(',', []);
            $result = 'no exception';
        } catch (\TypeError $e) {
            $result = $e->getMessage();
        }
        $output = ob_get_clean();

        $this->assertSame(['', 'explode(): Argument #2 ($string) must be of type string, array given'], [$output, $result]);
    }

    public function testExecutionStopsAfterEmptySeparator(): void
    {
        ob_start();
        try {
            $this->test->stopsAfterThrow('', 'a');
            $result = 'no exception';
        } catch (\ValueError $e) {
            $result = 'ValueError';
        }
        $output = ob_get_clean();

        $this->assertSame(['', 'ValueError'], [$output, $result]);
    }

    public function testThrowInsideTryIsCaught(): void
    {
        ob_start();
        $result = $this->test->catchesInTry(',', new stdClass());
        $output = ob_get_clean();

        $this->assertSame(['', 'TypeError'], [$output, $result]);
    }

    private function throwingDeprecations(callable $subject): string
    {
        set_error_handler(static function (int $code, string $message): bool {
            throw new ErrorException($message, 0, $code);
        }, E_DEPRECATED);

        try {
            return var_export($subject(), true);
        } catch (\Throwable $e) {
            return $e::class . ': ' . $e->getMessage();
        } finally {
            restore_error_handler();
        }
    }
}
