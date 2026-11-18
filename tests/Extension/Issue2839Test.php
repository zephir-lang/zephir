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

use ArrayObject;
use PHPUnit\Framework\TestCase;
use ReflectionClass;
use stdClass;
use Stub\Issue2839;
use Stub\Issue2839Pair;
use WeakReference;

use function get_class;

/**
 * `create_instance_params()` creates the object as `new $class(...$parameters)`
 * does: a class without a constructor ignores positional arguments and rejects
 * named ones, and a constructor receives positional and named arguments.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2839
 */
final class Issue2839Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2839 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2839();
    }

    public static function withoutConstructorProvider(): array
    {
        $cases = [];

        foreach (['internal' => stdClass::class, 'zephir' => Issue2839::class] as $kind => $class) {
            $cases[$kind . ', no arguments']        = [$class, []];
            $cases[$kind . ', positional']          = [$class, [1]];
            $cases[$kind . ', positional array']    = [$class, [[]]];
            $cases[$kind . ', named']               = [$class, ['name' => 1]];
            $cases[$kind . ', positional, named']   = [$class, [1, 'name' => 2]];
            $cases[$kind . ', named, positional']   = [$class, ['name' => 1, 2]];
        }

        return $cases;
    }

    /**
     * @dataProvider withoutConstructorProvider
     */
    public function testClassWithoutConstructorMatchesNew(string $class, array $parameters): void
    {
        $this->assertMatchesPhp(
            fn () => $this->describe($this->test->build($class, $parameters)),
            fn () => $this->describe(new $class(...$parameters))
        );
    }

    public static function withConstructorProvider(): array
    {
        return [
            'positional'               => [[1, 2]],
            'named, out of order'      => [['b' => 2, 'a' => 1]],
            'positional, named'        => [[1, 'b' => 2]],
            'positional, sparse keys'  => [[5 => 'x', 2 => 'y']],
            'unknown named'            => [['a' => 1, 'zz' => 2]],
        ];
    }

    /**
     * @dataProvider withConstructorProvider
     */
    public function testConstructorReceivesArgumentsAsNew(array $parameters): void
    {
        $this->assertMatchesPhp(
            fn () => $this->describe($this->test->build(Issue2839Pair::class, $parameters)),
            fn () => $this->describe(new Issue2839Pair(...$parameters))
        );
    }

    /**
     * The engine's call rejects a positional argument after a named one, as
     * ReflectionClass::newInstanceArgs() and call_user_func_array() do. `new`
     * adds "during unpacking" to the same message.
     */
    public function testPositionalAfterNamedFailsAsNewInstanceArgs(): void
    {
        $parameters = ['a' => 1, 2];

        $this->assertMatchesPhp(
            fn () => $this->describe($this->test->build(Issue2839Pair::class, $parameters)),
            fn () => $this->describe((new ReflectionClass(Issue2839Pair::class))->newInstanceArgs($parameters))
        );
    }

    /**
     * Only calls that succeed: Xdebug's develop mode keeps the last exception,
     * and the arguments in its trace, alive.
     */
    public static function releaseProvider(): array
    {
        return [
            'positional, no constructor'        => [stdClass::class, static fn (object $value): array => [$value]],
            'positional, constructor'           => [Issue2839Pair::class, static fn (object $value): array => [$value]],
            'named, constructor'                => [Issue2839Pair::class, static fn (object $value): array => ['b' => $value, 'a' => 1]],
            'positional and named, constructor' => [Issue2839Pair::class, static fn (object $value): array => [1, 'b' => $value]],
        ];
    }

    /**
     * @dataProvider releaseProvider
     */
    public function testArgumentIsReleasedAfterTheCall(string $class, callable $parameters): void
    {
        $value     = new stdClass();
        $reference = WeakReference::create($value);

        $this->test->build($class, $parameters($value));
        unset($value);

        $this->assertNull($reference->get());
    }

    private function describe(object $instance): array
    {
        return [
            get_class($instance),
            $instance instanceof ArrayObject ? $instance->getArrayCopy() : null,
        ];
    }
}
