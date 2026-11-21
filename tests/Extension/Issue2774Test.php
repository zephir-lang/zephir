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
use Stub\Issue2774;
use Throwable;

use function get_class;
use function ob_end_clean;
use function ob_get_contents;
use function ob_start;
use function var_export;

use const INF;
use const NAN;
use const PHP_INT_MAX;
use const PHP_INT_MIN;

/**
 * `var_export()` exports any expression, a cast or a native variable
 * included, and honors its `return` argument: without it, or with `false`,
 * it prints and gives `null`; with `true` it gives the string and prints
 * nothing, even when the result is discarded.
 *
 * Each side runs inside an output buffer, so the comparison covers what is
 * printed as well as the diagnostics and the value.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2774
 */
final class Issue2774Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2774 $test;

    /**
     * @var callable
     */
    private $call;

    protected function setUp(): void
    {
        $this->test = new Issue2774();
        $this->call = require __DIR__ . '/../fixtures/weak-mode-call.php';
    }

    /**
     * Factories rather than values: a recursive structure cannot live in a
     * data provider.
     */
    public static function valueProvider(): array
    {
        return [
            'true'             => [static fn () => true],
            'false'            => [static fn () => false],
            'int'              => [static fn () => -1],
            'float'            => [static fn () => 1.5],
            'float fraction'   => [static fn () => 0.1],
            'infinity'         => [static fn () => INF],
            'empty string'     => [static fn () => ''],
            'quoted string'    => [static fn () => "it's \\ \0"],
            'null'             => [static fn () => null],
            'empty array'      => [static fn () => []],
            'nested array'     => [static fn () => [1, 'a' => [true, null], 5 => 'x']],
            'object'           => [static function () {
                $object    = new stdClass();
                $object->a = [1];

                return $object;
            }],
            'recursive array'  => [static function () {
                $array   = [1];
                $array[] = &$array;

                return $array;
            }],
            'recursive object' => [static function () {
                $object       = new stdClass();
                $object->self = $object;

                return $object;
            }],
        ];
    }

    /**
     * Each call form as the extension runs it and as PHP does.
     */
    public static function formProvider(): array
    {
        return [
            'returned'           => [
                static fn (Issue2774 $t, $v) => $t->exportVar($v),
                static fn ($v) => var_export($v, true),
            ],
            'concatenated'       => [
                static fn (Issue2774 $t, $v) => $t->exportConcat($v),
                static fn ($v) => '<' . var_export($v, true) . '>',
            ],
            'echoed'             => [
                static fn (Issue2774 $t, $v) => $t->echoExport($v),
                static function ($v): void {
                    echo var_export($v, true);
                },
            ],
            'discarded'          => [
                static fn (Issue2774 $t, $v) => $t->discarded($v),
                static function ($v): void {
                    var_export($v, true);
                },
            ],
            'printed, result'    => [
                static fn (Issue2774 $t, $v) => $t->printReturned($v),
                static fn ($v) => var_export($v),
            ],
            'printed by false'   => [
                static fn (Issue2774 $t, $v) => $t->printFalse($v),
                static fn ($v) => var_export($v, false),
            ],
            'printed, statement' => [
                static fn (Issue2774 $t, $v) => $t->printStatement($v),
                static function ($v): void {
                    var_export($v, false);
                },
            ],
        ];
    }

    public static function formAndValueProvider(): array
    {
        $cases = [];

        foreach (self::formProvider() as $form => [$extension, $php]) {
            foreach (self::valueProvider() as $value => [$make]) {
                $cases[$form . ', ' . $value] = [$extension, $php, $make];
            }
        }

        return $cases;
    }

    /**
     * @dataProvider formAndValueProvider
     */
    public function testCallFormMatchesPhp(callable $extension, callable $php, callable $make): void
    {
        $this->assertMatchesPhp(
            self::captured(fn () => $extension($this->test, $make())),
            self::captured(static fn () => $php($make()))
        );
    }

    public static function castProvider(): array
    {
        return [
            'true'         => [true],
            'false'        => [false],
            'zero'         => [0],
            'int'          => [7],
            'zero float'   => [0.0],
            'empty string' => [''],
            'zero string'  => ['0'],
            'string'       => ['a'],
            'null'         => [null],
            'empty array'  => [[]],
            'array'        => [[0]],
            'object'       => [new stdClass()],
        ];
    }

    /**
     * @dataProvider castProvider
     */
    public function testCastArgumentMatchesPhp(mixed $flag): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportCast($flag),
            static fn () => var_export((bool) $flag, true)
        );
    }

    /**
     * @dataProvider castProvider
     */
    public function testNativeBoolArgumentMatchesPhp(mixed $flag): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportNative($flag),
            static fn () => var_export((bool) $flag, true)
        );
    }

    public static function intProvider(): array
    {
        return [
            'zero'     => [0],
            'negative' => [-1],
            'max'      => [PHP_INT_MAX],
            'min'      => [PHP_INT_MIN],
        ];
    }

    /**
     * @dataProvider intProvider
     */
    public function testNativeIntArgumentMatchesPhp(int $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportInt($value),
            static fn () => var_export($value, true)
        );
    }

    public static function doubleProvider(): array
    {
        return [
            'integral'      => [1.0],
            'fraction'      => [0.1],
            'negative zero' => [-0.0],
            'exponent'      => [1e100],
            'infinity'      => [INF],
            'nan'           => [NAN],
        ];
    }

    /**
     * @dataProvider doubleProvider
     */
    public function testNativeDoubleArgumentMatchesPhp(float $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportDouble($value),
            static fn () => var_export($value, true)
        );
    }

    public function testNativeExpressionArgumentMatchesPhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportSum(2, -5),
            static fn () => var_export(2 + -5, true)
        );
    }

    public function testLiteralArgumentMatchesPhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportLiteral(),
            static fn () => var_export(5, true)
        );
    }

    /**
     * A string literal is exported as its text even when a variable has that
     * name.
     */
    public function testStringLiteralNamedLikeAVariableMatchesPhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportShadowedLiteral(1),
            static fn () => var_export('value', true)
        );
    }

    /**
     * var_dump() shares the lookup: a string literal is dumped as its text
     * even when a variable has that name.
     *
     * The text is spelled out: Xdebug's develop mode prefixes a userland
     * var_dump() with its file and line, so PHP's own call is no oracle.
     */
    public function testDumpedStringLiteralNamedLikeAVariable(): void
    {
        $this->assertSame(
            ['value' => null, 'output' => "string(5) \"value\"\n"],
            self::captured(fn () => $this->test->dumpShadowedLiteral(1))()
        );
    }

    public static function arrayProvider(): array
    {
        return [
            'empty'  => [[]],
            'nested' => [[1, 'a' => [true, 'b']]],
        ];
    }

    /**
     * @dataProvider arrayProvider
     */
    public function testArrayArgumentMatchesPhp(array $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportArray($value),
            static fn () => var_export($value, true)
        );
    }

    public static function stringProvider(): array
    {
        return [
            'empty'   => [''],
            'plain'   => ['abc'],
            'escaped' => ["it's \\ \0"],
        ];
    }

    /**
     * @dataProvider stringProvider
     */
    public function testStringArgumentMatchesPhp(string $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->exportString($value),
            static fn () => var_export($value, true)
        );
    }

    public static function flagProvider(): array
    {
        return [
            'true'         => [true],
            'false'        => [false],
            'one'          => [1],
            'zero'         => [0],
            'float'        => [1.5],
            'string'       => ['yes'],
            'empty string' => [''],
            'null'         => [null],
            'array'        => [[]],
            'object'       => [new stdClass()],
        ];
    }

    /**
     * A flag known only at run time is read as PHP's `var_export()` reads it:
     * coerced to bool, `null` deprecated from 8.1, an array or object refused.
     *
     * @dataProvider flagProvider
     */
    public function testRuntimeReturnArgumentMatchesPhp(mixed $flag): void
    {
        $this->assertMatchesPhp(
            self::captured(fn () => $this->test->exportFlag(['a' => 1], $flag)),
            self::captured(fn () => ($this->call)('var_export', ['a' => 1], $flag))
        );
    }

    /**
     * A get hook that throws stops the export: PHP gives no string, so
     * nothing of the partial export is printed or returned.
     *
     * @requires PHP >= 8.4
     */
    public function testExportStoppedByAHookMatchesPhp(): void
    {
        self::defineHooks();

        foreach (self::formProvider() as $form => [$extension, $php]) {
            $this->assertMatchesPhp(
                self::captured(fn () => $extension($this->test, new \Issue2774Hooks())),
                self::captured(static fn () => $php(new \Issue2774Hooks())),
                $form
            );
        }
    }

    /**
     * Runs the subject inside an output buffer and gives back what it
     * printed together with its value or exception.
     */
    private static function captured(callable $subject): callable
    {
        return static function () use ($subject): array {
            ob_start();

            try {
                return ['value' => $subject(), 'output' => ob_get_contents()];
            } catch (Throwable $e) {
                return ['thrown' => get_class($e) . ': ' . $e->getMessage(), 'output' => ob_get_contents()];
            } finally {
                ob_end_clean();
            }
        };
    }

    /**
     * Hooks are PHP 8.4 syntax, so the class is compiled only where it parses.
     */
    private static function defineHooks(): void
    {
        if (class_exists('Issue2774Hooks', false)) {
            return;
        }

        eval(<<<'PHP'
            class Issue2774Hooks
            {
                public $before = 1;

                public $failing {
                    get {
                        throw new \RuntimeException('hook failed');
                    }
                }
            }
            PHP);
    }
}
