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

use Closure;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Issue2824;
use Stub\Issue2824Child;
use Stub\Issue2824Pair;
use Stub\Issue2824Renamed;
use Throwable;
use WeakReference;

use function call_user_func;
use function gettype;
use function in_array;
use function memory_get_usage;
use function number_format;
use function preg_match;
use function sscanf;
use function str_repeat;
use function strpos;

/**
 * Named arguments are bound at run time by the parameter names of the method
 * or function actually called, as PHP binds them: in any order, after
 * positional ones, with the declared default for a skipped parameter.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824NamedArgumentsTest extends TestCase
{
    private Issue2824 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2824();
    }

    public function testMethodCallBindsByName(): void
    {
        $pair = new Issue2824Pair();

        $this->assertSame($this->php(fn () => $this->mirror()->pair(b: '2', a: '1')), $this->test->typed($pair));
        $this->assertSame('1,2', $this->test->untyped($pair));
        $this->assertSame('1,2', $pair->thisPair());
        $this->test->statement($pair);
    }

    public function testRuntimeCalleeNamesWin(): void
    {
        $this->assertSame('child:1,2', $this->test->typed(new Issue2824Child()));
        $this->assertSame(
            $this->php(fn () => $this->renamedMirror()->pair(b: '2', a: '1')),
            $this->zephir(fn () => $this->test->typed(new Issue2824Renamed()))
        );
    }

    public function testStaticCallBindsByName(): void
    {
        $this->assertSame('1,2', Issue2824Pair::selfPair());
        $this->assertSame('1,2', Issue2824Pair::staticPair());
        $this->assertSame('1,2', Issue2824Child::parentPair());
        $this->assertSame('1,2', $this->test->classStatic());
        $this->assertSame('1,2', $this->test->dynamicClass(Issue2824Pair::class));
    }

    public function testDynamicMethodBindsByName(): void
    {
        $this->assertSame('1,2', $this->test->dynamicMethod(new Issue2824Pair(), 'pair'));
    }

    public function testPositionalThenNamed(): void
    {
        $pair = new Issue2824Pair();

        $this->assertSame('1,2', $this->test->positionalThenNamed($pair));
        $this->assertSame('1,2', $this->test->typedMixed($pair));
    }

    public function testSkippedParameterGetsItsDefault(): void
    {
        $pair = new Issue2824Pair();

        $this->assertSame($this->php(fn () => $this->mirror()->withDefault(b: '2')), $this->test->skipped($pair));
        $this->assertSame('x,2', $this->test->skippedUntyped($pair));
        $this->assertSame('null,2', $this->test->skippedCallable($pair));
        $this->assertSame('null,2', $this->test->skippedObject($pair));
        $this->assertSame('5,7', $this->test->skippedInt($pair));
    }

    public function testVariadicCollectsUnknownNames(): void
    {
        $this->assertSame(
            $this->php(fn () => $this->mirror()->variadic(1, 2, x: 3)),
            $this->test->variadicExtras(new Issue2824Pair())
        );
    }

    public function testUnknownNameThrowsLikePhp(): void
    {
        $this->assertSame(
            $this->php(fn () => $this->mirror()->pair(...['a' => '1', 'zz' => '2'])),
            $this->zephir(fn () => $this->test->unknownName(new Issue2824Pair()))
        );
    }

    public function testNamedOverwritingPositionalThrowsLikePhp(): void
    {
        $this->assertSame(
            $this->php(fn () => $this->mirror()->pair('1', ...['a' => '2'])),
            $this->zephir(fn () => $this->test->overwrite(new Issue2824Pair()))
        );
    }

    public function testFunctionCallBindsByName(): void
    {
        $this->assertSame('1,2', $this->test->userFunction());
        $this->assertSame(
            number_format(thousands_separator: ' ', num: 1234.567, decimals: 2),
            $this->test->numberFormat()
        );
        $this->assertSame(strpos(needle: 'b', haystack: 'abc'), $this->test->strposNamed());
        $this->assertSame(in_array(haystack: ['1', 'b'], strict: true, needle: 1), $this->test->inArrayNamed());

        preg_match(subject: 'abc', matches: $matches, pattern: '/(b)/');
        $this->assertSame($matches, $this->test->pregMatchNamed());
    }

    public function testConstructorBindsByName(): void
    {
        $this->assertSame(['1', '2'], $this->test->construct());
    }

    public function testClosureBindsByName(): void
    {
        $this->assertSame('1,2', $this->test->closure());
    }

    public function testCallUserFuncForwardsNames(): void
    {
        $this->assertSame(
            call_user_func([$this->mirror(), 'pair'], b: '2', a: '1'),
            $this->test->callUserFunc(new Issue2824Pair())
        );
    }

    public function testByReferenceArgumentIsReleased(): void
    {
        $this->assertSame(['a' => '1'], $this->test->positionalByReference());

        foreach (['positionalByReference', 'pregMatchNamed'] as $method) {
            for ($i = 0; $i < 200; ++$i) {
                $this->test->$method();
            }

            $before = memory_get_usage();
            for ($i = 0; $i < 2000; ++$i) {
                $this->test->$method();
            }

            $this->assertSame($before, memory_get_usage(), $method);
        }
    }

    public function testNamedArgumentIsReleasedAfterTheCall(): void
    {
        $php = function (Issue2824Pair $p): array {
            $released = [];

            $probe = new stdClass();
            $weak  = WeakReference::create($probe);
            $this->assertSame('object', gettype(value: $probe));
            $probe      = null;
            $released[] = null === $weak->get();

            $probe = new stdClass();
            $weak  = WeakReference::create($probe);
            $p->withObject(o: $probe);
            $probe      = null;
            $released[] = null === $weak->get();

            return $released;
        };

        $this->assertSame($php(new Issue2824Pair()), $this->test->namedArgumentReleased(new Issue2824Pair()));
    }

    public function testByReferenceArgumentLeftUnassignedIsReleasedOnce(): void
    {
        $php = function (): string {
            sscanf('', '%s', $word);
            sscanf('abc', '%s', $word);

            return $word;
        };

        $word = $this->test->byReferenceLeftUnassigned();
        // Takes the block a second release of the string would have freed.
        $reuse = str_repeat('z', 3);

        $this->assertSame($php(), $word);
        $this->assertSame('zzz', $reuse);
    }

    /**
     * What the PHP mirror returns, or the class and message it throws.
     */
    private function php(Closure $call): mixed
    {
        try {
            return $call();
        } catch (Throwable $e) {
            return [$e::class, $e->getMessage()];
        }
    }

    private function zephir(Closure $call): mixed
    {
        return $this->php($call);
    }

    private function mirror(): object
    {
        return new class () {
            public function pair($a, $b): string
            {
                return $a . ',' . $b;
            }

            public function withDefault($a = 'x', $b = 'y'): string
            {
                return $a . ',' . $b;
            }

            public function variadic($a, ...$rest): array
            {
                return [$a, $rest];
            }
        };
    }

    private function renamedMirror(): object
    {
        return new class () {
            public function pair($first, $second): string
            {
                return $first . ';' . $second;
            }
        };
    }
}
