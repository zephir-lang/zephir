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

use AllowDynamicProperties;
use ArrayObject;
use Closure;
use PHPUnit\Framework\TestCase;
use RuntimeException;
use stdClass;
use Stub\Issue2832;
use Stub\Issue2832Base;
use Throwable;

use function class_exists;
use function get_class;
use function get_object_vars;
use function is_object;

/**
 * `fetch c, o->x` is PHP's `isset($o->x)` followed by `$o->x`: `__isset()`
 * is asked, an inaccessible or uninitialized property is not found, a name
 * is converted as PHP converts it, and an exception stops the method. A
 * property that exists and holds null is found, as `fetch` of an array key
 * holding null is. In an `isset`, `empty` or `fetch` chain every container is
 * fetched silently and every key or property name is read as PHP reads it.
 * PHP is the oracle.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2832
 */
final class Issue2832Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * An object with both magic methods that logs every call.
     */
    private static function magic(bool $answer = false, mixed $value = 'from __get'): object
    {
        return new #[AllowDynamicProperties] class ($answer, $value) {
            public array $log = [];

            public function __construct(public bool $answer, public mixed $value)
            {
            }

            public function __isset($name)
            {
                $this->log[] = "__isset($name)";

                return $this->answer;
            }

            public function __get($name)
            {
                $this->log[] = "__get($name)";

                return $this->value;
            }
        };
    }

    private static function child(): Issue2832Base
    {
        return new class () extends Issue2832Base {
        };
    }

    private static function onlyIsset(): object
    {
        return new class () {
            public array $log = [];

            public function __isset($name)
            {
                $this->log[] = "__isset($name)";

                return true;
            }
        };
    }

    private static function onlyGet(): object
    {
        return new class () {
            public array $log = [];

            public function __get($name)
            {
                $this->log[] = "__get($name)";

                return "get:$name";
            }
        };
    }

    private static function throwing(string $method): object
    {
        return new class ($method) {
            public array $log = [];

            public function __construct(private string $method)
            {
            }

            public function __isset($name)
            {
                if ('__isset' === $this->method) {
                    throw new RuntimeException("__isset($name) failed");
                }

                return true;
            }

            public function __get($name)
            {
                throw new RuntimeException("__get($name) failed");
            }
        };
    }

    private static function visibility(): object
    {
        return new class () {
            public array $log = [];
            public int $typed;
            public $nul = null;
            protected $prot = 'prot';
            private $priv = 'priv';
        };
    }

    /**
     * A declared property PHP hands to the magic methods once it is unset.
     */
    private static function unsetDeclared(): object
    {
        $object = new class () {
            public array $log = [];
            public $x = 'declared';

            public function __isset($name)
            {
                $this->log[] = "__isset($name)";

                return false;
            }

            public function __get($name)
            {
                $this->log[] = "__get($name)";

                return 'from __get';
            }
        };
        unset($object->x);

        return $object;
    }

    private static function names(): stdClass
    {
        $object          = new stdClass();
        $object->{'1'}   = 'one';
        $object->{'1.5'} = 'float';
        $object->{''}    = 'empty';
        $object->x       = 'x';

        return $object;
    }

    private static function stringable(): object
    {
        return new class () {
            public function __toString(): string
            {
                return 'x';
            }
        };
    }

    private static function log(mixed $subject): ?array
    {
        return is_object($subject) ? ($subject->log ?? null) : null;
    }

    /**
     * Runs `$call` with a fresh target, and tells whether the code after the
     * fetch still ran.
     */
    private static function markAfter(Closure $call): array
    {
        $target = new stdClass();

        try {
            $value = $call($target);
        } catch (Throwable $e) {
            $value = get_class($e) . ': ' . $e->getMessage();
        }

        return [$value, isset($target->marked)];
    }

    public static function propertyProvider(): array
    {
        return [
            '__isset true'               => [static fn () => self::magic(true), 'x'],
            '__isset false'              => [static fn () => self::magic(), 'x'],
            '__isset true, no __get'     => [static fn () => self::onlyIsset(), 'x'],
            '__get, no __isset'          => [static fn () => self::onlyGet(), 'x'],
            'protected'                  => [static fn () => self::visibility(), 'prot'],
            'private'                    => [static fn () => self::visibility(), 'priv'],
            'uninitialized typed'        => [static fn () => self::visibility(), 'typed'],
            'unset declared'             => [static fn () => self::unsetDeclared(), 'x'],
            'undefined'                  => [static fn () => new stdClass(), 'x'],
            'int name'                   => [static fn () => self::names(), 1],
            'float name'                 => [static fn () => self::names(), 1.5],
            'bool name'                  => [static fn () => self::names(), true],
            'null name'                  => [static fn () => self::names(), null],
            'stringable name'            => [static fn () => self::names(), self::stringable()],
            'object name'                => [static fn () => self::names(), new stdClass()],
            'array name'                 => [static fn () => self::names(), []],
            'null container'             => [static fn () => null, 'x'],
            'array container'            => [static fn () => ['x' => 1], 'x'],
            'string container'           => [static fn () => 'x', 'x'],
            'ArrayObject as props'       => [static fn () => new ArrayObject(['x' => 1], ArrayObject::ARRAY_AS_PROPS), 'x'],
            'ArrayObject as props, miss' => [static fn () => new ArrayObject(['x' => 1], ArrayObject::ARRAY_AS_PROPS), 'y'],
        ];
    }

    /**
     * @dataProvider propertyProvider
     */
    public function testFetchIsIssetThenReadLikePhp(callable $make, mixed $name): void
    {
        $this->assertMatchesPhp(
            static function () use ($make, $name) {
                $object = $make();

                return [Issue2832::fetchName($object, $name), self::log($object)];
            },
            static function () use ($make, $name) {
                $object = $make();

                return [isset($object->{$name}) ? [true, $object->{$name}] : [false, null], self::log($object)];
            }
        );
    }

    /**
     * @dataProvider propertyProvider
     */
    public function testFetchStatementLikePhp(callable $make, mixed $name): void
    {
        $this->assertMatchesPhp(
            static function () use ($make, $name) {
                $object = $make();

                return [Issue2832::fetchStatement($object, $name), self::log($object)];
            },
            static function () use ($make, $name) {
                $object = $make();

                return [isset($object->{$name}) ? $object->{$name} : null, self::log($object)];
            }
        );
    }

    public static function literalProvider(): array
    {
        return [
            '__isset true'           => [static fn () => self::magic(true)],
            '__isset false'          => [static fn () => self::magic()],
            '__isset true, no __get' => [static fn () => self::onlyIsset()],
            'unset declared'         => [static fn () => self::unsetDeclared()],
            'undefined'              => [static fn () => new stdClass()],
            'null container'         => [static fn () => null],
        ];
    }

    /**
     * @dataProvider literalProvider
     */
    public function testFetchOfALiteralNameLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2832::fetchLiteral($object), self::log($object)];
            },
            static function () use ($make) {
                $object = $make();

                return [isset($object->x) ? [true, $object->x] : [false, null], self::log($object)];
            }
        );
    }

    /**
     * @dataProvider literalProvider
     */
    public function testFetchOfAStringNameLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2832::fetchString($object), self::log($object)];
            },
            static function () use ($make) {
                $object = $make();

                return [isset($object->{'x'}) ? [true, $object->{'x'}] : [false, null], self::log($object)];
            }
        );
    }

    /**
     * A name written as a string keeps PHP's escapes: `o->{"a\"b\\c"}` is
     * the property `a"b\c` for every operation, not only `fetch`.
     */
    public function testEscapedStringNameLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = (object) ['a"b\c' => 'q'];
                $written = new stdClass();
                Issue2832::writeEscaped($written, 'w');

                return [
                    Issue2832::fetchEscaped($object),
                    Issue2832::issetEscaped($object),
                    Issue2832::emptyEscaped($object),
                    Issue2832::readEscaped($object),
                    get_object_vars($written),
                ];
            },
            static function () {
                $object = (object) ['a"b\c' => 'q'];
                $written = new stdClass();
                $written->{"a\"b\\c"} = 'w';

                return [
                    isset($object->{"a\"b\\c"}) ? [true, $object->{"a\"b\\c"}] : [false, null],
                    isset($object->{"a\"b\\c"}),
                    empty($object->{"a\"b\\c"}),
                    $object->{"a\"b\\c"},
                    get_object_vars($written),
                ];
            }
        );
    }

    public function testFetchOfAnIntNameLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => [Issue2832::fetchIntName(self::names(), 1), Issue2832::fetchIntName(self::names(), 2)],
            static function () {
                $object = self::names();

                return [isset($object->{1}) ? [true, $object->{1}] : [false, null], [false, null]];
            }
        );
    }

    /**
     * The one difference from `isset()`: `fetch` finds a property that
     * exists and holds null, without asking `__isset()`.
     */
    public function testPropertyHoldingNullIsFound(): void
    {
        $object = self::magic(true);
        $object->nul = null;

        $this->assertSame(
            [[true, null], [false, null], []],
            [Issue2832::fetchName($object, 'nul'), Issue2832::fetchName(new stdClass(), 'nul'), $object->log]
        );
    }

    public function testDeclaredPropertyHoldingNullIsFound(): void
    {
        $this->assertSame([true, null], Issue2832::fetchName(self::visibility(), 'nul'));
    }

    public static function throwingProvider(): array
    {
        return [
            '__isset throws' => [static fn () => self::throwing('__isset'), 'x', 'RuntimeException: __isset(x) failed'],
            '__get throws'   => [static fn () => self::throwing('__get'), 'x', 'RuntimeException: __get(x) failed'],
            'name throws'    => [
                static fn () => self::names(),
                new stdClass(),
                'Error: Object of class stdClass could not be converted to string',
            ],
        ];
    }

    /**
     * @dataProvider throwingProvider
     */
    public function testExceptionDuringFetchStopsTheMethod(callable $make, mixed $name, string $error): void
    {
        $object = $make();

        $this->assertSame(
            [$error, false],
            self::markAfter(static fn ($target) => Issue2832::fetchThenMark($object, $name, $target))
        );
    }

    /**
     * PHP 8.5 deprecates binding a closure to the scope of an internal class,
     * so the expected values are written out: they are the same on every
     * PHP version.
     */
    public static function scopeProvider(): array
    {
        return [
            'protected'                   => [static fn () => new Issue2832Base(), 'prot', [true, 'prot']],
            'private'                     => [static fn () => new Issue2832Base(), 'priv', [true, 'priv']],
            'private of a parent'         => [static fn () => self::child(), 'priv', [true, 'priv']],
            'protected of another object' => [static fn () => self::visibility(), 'prot', [false, null]],
        ];
    }

    /**
     * Visibility is judged from the class the `fetch` is written in.
     *
     * @dataProvider scopeProvider
     */
    public function testFetchSeesPropertiesVisibleFromItsOwnClass(callable $make, string $name, array $expected): void
    {
        $this->assertSame($expected, Issue2832Base::fetchName($make(), $name));
    }

    public function testDeclaredNullIsFoundFromTheSameClass(): void
    {
        $this->assertSame([true, null], Issue2832Base::fetchName(new Issue2832Base(), 'nul'));
    }

    /**
     * `fetch a, a->{b}` reads the object before it replaces it.
     */
    public function testFetchIntoTheObjectItReads(): void
    {
        $this->assertSame(
            [[true, 'value'], [false, null]],
            [Issue2832::fetchSelf(self::magic(true, 'value'), 'x'), Issue2832::fetchSelf(new stdClass(), 'x')]
        );
    }

    public function testFetchIntoTheNameItReads(): void
    {
        $this->assertSame([true, 'x'], Issue2832::fetchIntoName(self::names(), 'x'));
    }

    public function testFetchIntoTheArrayItReads(): void
    {
        $this->assertSame(
            [[true, 5], [false, null]],
            [Issue2832::fetchSelfArray([5], 0), Issue2832::fetchSelfArray([5], 1)]
        );
    }

    public function testFetchIntoTheKeyItReads(): void
    {
        $this->assertSame([true, 'v'], Issue2832::fetchIntoKey(['k' => 'v'], 'k'));
    }

    public static function keyProvider(): array
    {
        return [
            'magic key'     => [static fn () => self::magic(false, 'x')],
            'magic, isset'  => [static fn () => self::magic(true, 'x')],
            'undefined key' => [static fn () => new stdClass()],
        ];
    }

    /**
     * A key is read as PHP reads it: `__get()` without `__isset()`, and a
     * warning for an undefined property.
     *
     * @dataProvider keyProvider
     */
    public function testKeyIsReadLikePhp(callable $make): void
    {
        $array = ['x' => 1, '' => 0];
        $cases = [
            'isset' => [
                static fn ($key) => Issue2832::issetKey($array, $key),
                static fn ($key) => isset($array[$key->k]),
            ],
            'fetch' => [
                static fn ($key) => Issue2832::fetchKey($array, $key),
                // `fetch` reads its key and looks it up once; no value in `$array` is null
                static fn ($key) => null !== ($value = $array[$key->k] ?? null) ? [true, $value] : [false, null],
            ],
            'empty' => [
                static fn ($key) => Issue2832::emptyKey($array, $key),
                static fn ($key) => empty($array[$key->k]),
            ],
        ];

        foreach ($cases as $case => [$extension, $php]) {
            $this->assertMatchesPhp(
                static function () use ($make, $extension) {
                    $key = $make();

                    return [$extension($key), self::log($key)];
                },
                static function () use ($make, $php) {
                    $key = $make();

                    return [$php($key), self::log($key)];
                },
                $case
            );
        }
    }

    public static function deepArrayProvider(): array
    {
        return [
            'empty'   => [[]],
            'level 1' => [[1 => []]],
            'level 2' => [[1 => [2 => []]]],
            'found'   => [[1 => [2 => [3 => 'v']]]],
            'string'  => [[1 => 'abc']],
        ];
    }

    /**
     * Every container of the chain is fetched silently.
     *
     * @dataProvider deepArrayProvider
     */
    public function testDeepArrayContainersAreSilentLikePhp(array $array): void
    {
        $this->assertMatchesPhp(
            static fn () => [Issue2832::issetDeep($array), Issue2832::emptyDeep($array), Issue2832::fetchDeep($array)],
            static fn () => [
                isset($array[1][2][3]),
                empty($array[1][2][3]),
                isset($array[1][2][3]) ? [true, $array[1][2][3]] : [false, null],
            ]
        );
    }

    public static function deepObjectProvider(): array
    {
        return [
            '__isset false' => [static fn () => self::magic()],
            '__isset true'  => [static fn () => self::magic(true, ['k' => ['j' => 1]])],
            '__get only'    => [static fn () => self::onlyGet()],
            'undefined'     => [static fn () => new stdClass()],
        ];
    }

    /**
     * @dataProvider deepObjectProvider
     */
    public function testDeepPropertyContainersAreSilentLikePhp(callable $make): void
    {
        $cases = [
            'isset o->x->y->z' => [
                static fn ($o) => Issue2832::issetPropDeep($o),
                static fn ($o) => isset($o->x->y->z),
            ],
            'fetch o->x->y->z' => [
                static fn ($o) => Issue2832::fetchPropDeep($o),
                static fn ($o) => isset($o->x->y->z) ? [true, $o->x->y->z] : [false, null],
            ],
            'isset o->x->{n}->z' => [
                static fn ($o) => Issue2832::issetDynamicDeep($o, 'y'),
                static fn ($o) => isset($o->x->{'y'}->z),
            ],
            'isset o->p[k][j]' => [
                static fn ($o) => Issue2832::issetPropArray($o),
                static fn ($o) => isset($o->p['k']['j']),
            ],
        ];

        foreach ($cases as $case => [$extension, $php]) {
            $this->assertMatchesPhp(
                static function () use ($make, $extension) {
                    $object = $make();

                    return [$extension($object), self::log($object)];
                },
                static function () use ($make, $php) {
                    $object = $make();

                    return [$php($object), self::log($object)];
                },
                $case
            );
        }
    }

    /**
     * PHP 8.4 property hooks: `isset()` runs the get hook, and a set-only
     * virtual property throws.
     *
     * @requires PHP >= 8.4
     */
    public function testFetchOfAHookedPropertyLikePhp(): void
    {
        self::defineHooks();

        foreach (['virtual', 'setOnly', 'backed'] as $name) {
            $this->assertMatchesPhp(
                static function () use ($name) {
                    $object = new \Issue2832Hooks();

                    return [Issue2832::fetchName($object, $name), $object->log];
                },
                static function () use ($name) {
                    $object = new \Issue2832Hooks();

                    return [isset($object->{$name}) ? [true, $object->{$name}] : [false, null], $object->log];
                },
                $name
            );
        }
    }

    /**
     * A hooked property whose get hook gives null exists, so it is found:
     * `isset()` runs the hook once and the read runs it again.
     *
     * @requires PHP >= 8.4
     */
    public function testHookedPropertyGivingNullIsFound(): void
    {
        self::defineHooks();
        $virtual = new \Issue2832Hooks();
        $backed  = new \Issue2832Hooks();

        $this->assertSame(
            [[true, null], ['virtualNull', 'virtualNull'], [true, null], ['backedNull', 'backedNull']],
            [
                Issue2832::fetchName($virtual, 'virtualNull'),
                $virtual->log,
                Issue2832::fetchName($backed, 'backedNull'),
                $backed->log,
            ]
        );
    }

    /**
     * Hooks are PHP 8.4 syntax, so the class is compiled only where it parses.
     */
    private static function defineHooks(): void
    {
        if (class_exists('Issue2832Hooks', false)) {
            return;
        }

        eval(<<<'PHP'
            class Issue2832Hooks
            {
                public array $log = [];

                public $virtual {
                    get {
                        $this->log[] = 'virtual';

                        return 'v';
                    }
                }

                public $setOnly {
                    set($value) {
                        $this->log[] = 'set';
                    }
                }

                public $backed = 'b' {
                    get {
                        $this->log[] = 'backed';

                        return $this->backed;
                    }
                }

                public $virtualNull {
                    get {
                        $this->log[] = 'virtualNull';

                        return null;
                    }
                }

                public $backedNull = null {
                    get {
                        $this->log[] = 'backedNull';

                        return $this->backedNull;
                    }
                }
            }
            PHP);
    }
}
