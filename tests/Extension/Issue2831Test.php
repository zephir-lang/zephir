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
use RuntimeException;
use stdClass;
use Stub\Issue2831;
use Stub\Issue2831Base;
use Stub\Issue2831Child;
use Throwable;

use function get_class;

/**
 * A property read must behave as PHP's `$o->x`: `__get()` is called without
 * asking `__isset()` first, an undefined property and a non-object warn, an
 * inaccessible or uninitialized property throws, and the method stops there.
 * `empty` and `isset` keep PHP's silent fetch. PHP is the oracle.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 * @see https://github.com/zephir-lang/zephir/issues/2784
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
final class Issue2831Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * An object with both magic methods that logs every call.
     */
    private static function magic(bool $answer = false, mixed $value = 'from __get'): object
    {
        return new class ($answer, $value) {
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

            public function __set($name, $value)
            {
                $this->log[] = "__set($name)=" . var_export($value, true);
            }
        };
    }

    private static function onlyGet(): object
    {
        return new class () {
            public function __get($name)
            {
                return "get:$name";
            }
        };
    }

    private static function onlyIsset(): object
    {
        return new class () {
            public function __isset($name)
            {
                return true;
            }
        };
    }

    private static function throwingGet(): object
    {
        return new class () {
            public function __get($name)
            {
                throw new RuntimeException("__get($name) failed");
            }
        };
    }

    /**
     * A declared property PHP hands to `__get()` once it has been unset.
     */
    private static function unsetDeclared(): object
    {
        $object = new class () {
            public array $log = [];
            public $declared = 'declared';

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
        unset($object->declared);

        return $object;
    }

    private static function protectedWithGet(): Issue2831Base
    {
        return new class () extends Issue2831Base {
            public function __get($name)
            {
                return "magic:$name";
            }
        };
    }

    private static function names(): stdClass
    {
        $object        = new stdClass();
        $object->{'1'} = 'one';
        $object->{'1.5'} = 'float';
        $object->x     = 'x';

        return $object;
    }

    /**
     * Runs `$read` with a fresh target, and tells whether the code after the
     * read still ran.
     */
    private static function markAfter(Closure $read): array
    {
        $target = new stdClass();

        try {
            $value = $read($target);
        } catch (Throwable $e) {
            $value = get_class($e) . ': ' . $e->getMessage();
        }

        return [$value, isset($target->marked)];
    }

    public static function magicProvider(): array
    {
        return [
            '__isset false' => [static fn () => self::magic()],
            '__isset true'  => [static fn () => self::magic(true)],
            '__get only'    => [static fn () => self::onlyGet()],
            '__isset only'  => [static fn () => self::onlyIsset()],
            'stdClass'      => [static fn () => new stdClass()],
        ];
    }

    /**
     * @dataProvider magicProvider
     */
    public function testReadCallsGetLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::read($object), $object->log ?? null];
            },
            static function () use ($make) {
                $object = $make();

                return [$object->x, $object->log ?? null];
            }
        );
    }

    /**
     * @dataProvider magicProvider
     */
    public function testDynamicReadCallsGetLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::readDynamic($object, 'x'), $object->log ?? null];
            },
            static function () use ($make) {
                $object = $make();
                $name   = 'x';

                return [$object->{$name}, $object->log ?? null];
            }
        );
    }

    /**
     * @dataProvider magicProvider
     */
    public function testReadInConditionLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::readInCondition($object), $object->log ?? null];
            },
            static function () use ($make) {
                $object = $make();

                return [$object->x ? 'yes' : 'no', $object->log ?? null];
            }
        );
    }

    /**
     * @dataProvider magicProvider
     */
    public function testEmptyAsksIssetFirstLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::isEmpty($object), $object->log ?? null];
            },
            static function () use ($make) {
                $object = $make();

                return [empty($object->x), $object->log ?? null];
            }
        );
    }

    /**
     * `isset` in one block and `empty` after it name the same property.
     *
     * @dataProvider magicProvider
     */
    public function testIssetAndEmptyInSeparateBlocksLikePhp(callable $make): void
    {
        foreach ([true, false] as $askIsset) {
            $this->assertMatchesPhp(
                static function () use ($make, $askIsset) {
                    $object = $make();

                    return [Issue2831::issetOrEmpty($object, $askIsset), $object->log ?? null];
                },
                static function () use ($make, $askIsset) {
                    $object = $make();

                    return [$askIsset ? isset($object->x) : empty($object->x), $object->log ?? null];
                },
                $askIsset ? 'isset' : 'empty'
            );
        }
    }

    /**
     * @dataProvider magicProvider
     */
    public function testIssetOfAnOffsetAsksIssetFirstLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::issetDim($object), $object->log ?? null];
            },
            static function () use ($make) {
                $object = $make();

                return [isset($object->x['k']), $object->log ?? null];
            }
        );
    }

    public function testReadOfAnUnsetDeclaredPropertyCallsGetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = self::unsetDeclared();

                return [Issue2831::readDynamic($object, 'declared'), $object->log];
            },
            static function () {
                $object = self::unsetDeclared();

                return [$object->declared, $object->log];
            }
        );
    }

    public function testCompoundAssignmentCallsGetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = self::magic(false, 1);

                return [Issue2831::addAssign($object), $object->log];
            },
            static function () {
                $object = self::magic(false, 1);
                $object->c += 1;

                return [$object->c, $object->log];
            }
        );
    }

    public function testCompoundAssignmentOfAnUndefinedPropertyWarnsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::addAssign(new stdClass()),
            static function () {
                $object = new stdClass();
                $object->c += 1;

                return $object->c;
            }
        );
    }

    public function testDynamicCompoundAssignmentCallsGetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = self::magic(false, 1);

                return [Issue2831::addAssignDynamic($object, 'c'), $object->log];
            },
            static function () {
                $object = self::magic(false, 1);
                $name   = 'c';
                $object->{$name} += 1;

                return [$object->{$name}, $object->log];
            }
        );
    }

    public function testDynamicCompoundAssignmentOfAnUndefinedPropertyWarnsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::addAssignDynamic(new stdClass(), 'c'),
            static function () {
                $object = new stdClass();
                $name   = 'c';
                $object->{$name} += 1;

                return $object->{$name};
            }
        );
    }

    public function testIncrementCallsGetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = self::magic(false, 1);

                return [Issue2831::increment($object), $object->log];
            },
            static function () {
                $object = self::magic(false, 1);
                $object->c++;

                return [$object->c, $object->log];
            }
        );
    }

    public function testIncrementOfAnUndefinedPropertyWarnsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::increment(new stdClass()),
            static function () {
                $object = new stdClass();
                $object->c++;

                return $object->c;
            }
        );
    }

    public function testUndefinedDynamicPropertyWarnsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readDynamic(new stdClass(), 'nope'),
            static function () {
                $name = 'nope';

                return (new stdClass())->{$name};
            }
        );
    }

    public function testProtectedPropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readProtected(new Issue2831Base()),
            static fn () => (new Issue2831Base())->prot
        );
    }

    public function testPrivatePropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readPrivate(new Issue2831Base()),
            static fn () => (new Issue2831Base())->priv
        );
    }

    public function testProtectedPropertyWithGetCallsGetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readProtected(self::protectedWithGet()),
            static fn () => self::protectedWithGet()->prot
        );
    }

    /**
     * PHP cannot bind a closure to the scope of an internal class, so the
     * oracle here is PHP's rule itself: the scope may read a protected
     * property declared by a class it shares the hierarchy with.
     */
    public function testProtectedPropertyOfTheSameClassIsRead(): void
    {
        $this->assertSame('prot', Issue2831Base::readOtherProtected(new Issue2831Child()));
    }

    public function testProtectedPropertyOfTheParentIsRead(): void
    {
        $this->assertSame('prot', Issue2831Child::readParentProtected(new Issue2831Base()));
    }

    public function testUninitializedTypedPropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readTyped(new Issue2831Base()),
            static fn () => (new Issue2831Base())->typed
        );
    }

    public function testUninitializedTypedPropertyOfThisThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2831Base())->readOwnTyped(),
            static fn () => (new Issue2831Base())->typed
        );
    }

    public function testUninitializedTypedPropertyOfThisInConditionThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2831Base())->readOwnTypedInCondition(),
            static fn () => (new Issue2831Base())->typed ? 'yes' : 'no'
        );
    }

    public function testInitializedTypedPropertyIsRead(): void
    {
        $object        = new Issue2831Base();
        $object->typed = 7;

        $this->assertSame(7, Issue2831::readTyped($object));
        $this->assertSame(7, $object->readOwnTyped());
        $this->assertSame('yes', $object->readOwnTypedInCondition());
    }

    public static function nonObjectProvider(): array
    {
        return [
            'null'   => [null],
            'int'    => [5],
            'float'  => [1.5],
            'true'   => [true],
            'false'  => [false],
            'string' => ['s'],
            'array'  => [[]],
        ];
    }

    /**
     * @dataProvider nonObjectProvider
     */
    public function testReadOfANonObjectWarnsLikePhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::read($value),
            static fn () => $value->x
        );
        $this->assertMatchesPhp(
            static fn () => Issue2831::readDynamic($value, 'x'),
            static function () use ($value) {
                $name = 'x';

                return $value->{$name};
            }
        );
    }

    /**
     * @dataProvider nonObjectProvider
     */
    public function testEmptyOfANonObjectIsSilentLikePhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::isEmpty($value),
            static fn () => empty($value->x)
        );
    }

    public static function nameProvider(): array
    {
        return [
            'int'          => [static fn () => 1],
            'float'        => [static fn () => 1.5],
            'true'         => [static fn () => true],
            'false'        => [static fn () => false],
            'null'         => [static fn () => null],
            'empty string' => [static fn () => ''],
            'nul byte'     => [static fn () => "\0x"],
            'array'        => [static fn () => []],
            'object'       => [static fn () => new stdClass()],
            'stringable'   => [
                static fn () => new class () {
                    public function __toString(): string
                    {
                        return 'x';
                    }
                },
            ],
        ];
    }

    /**
     * @dataProvider nameProvider
     */
    public function testDynamicNameIsConvertedLikePhp(callable $name): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::readDynamic(self::names(), $name()),
            static function () use ($name) {
                $property = $name();

                return self::names()->{$property};
            }
        );
    }

    /**
     * @dataProvider nameProvider
     */
    public function testEmptyOfADynamicNameIsConvertedLikePhp(callable $name): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::isEmptyDynamic(self::names(), $name()),
            static function () use ($name) {
                $property = $name();

                return empty(self::names()->{$property});
            }
        );
    }

    public static function emptyProvider(): array
    {
        return [
            'protected'         => ['prot'],
            'private'           => ['priv'],
            'uninitialized'     => ['typed'],
            'undefined'         => ['nope'],
        ];
    }

    /**
     * @dataProvider emptyProvider
     */
    public function testEmptyOfAnUnreadablePropertyIsSilentLikePhp(string $name): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::isEmptyDynamic(new Issue2831Base(), $name),
            static fn () => empty((new Issue2831Base())->{$name})
        );
    }

    /**
     * @dataProvider nameProvider
     */
    public function testIssetOfADynamicNameIsConvertedLikePhp(callable $name): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2831::issetDynamic(self::names(), $name()),
            static function () use ($name) {
                $property = $name();

                return isset(self::names()->{$property});
            }
        );
    }

    public static function nestedProvider(): array
    {
        return [
            '__isset false' => [static fn () => self::magic()],
            '__isset true'  => [static fn () => self::magic(true)],
            '__get only'    => [static fn () => self::onlyGet()],
            'stdClass'      => [static fn () => new stdClass()],
            'holding magic' => [
                static function () {
                    $object    = new stdClass();
                    $object->x = self::magic(true);

                    return $object;
                },
            ],
            'unreadable'    => [static fn () => new Issue2831Base()],
            'null'          => [static fn () => null],
        ];
    }

    /**
     * The object of `isset o->x->y` and `empty o->x->y` is fetched as
     * silently as PHP's `FETCH_OBJ_IS` does.
     *
     * @dataProvider nestedProvider
     */
    public function testNestedIssetAndEmptyLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static function () use ($make) {
                $object = $make();

                return [Issue2831::issetNested($object), Issue2831::isEmptyNested($object)];
            },
            static function () use ($make) {
                $object = $make();

                return [isset($object->x->y), empty($object->x->y)];
            }
        );
    }

    public function testThrowingIssetStopsIssetAndEmptyLikePhp(): void
    {
        $make = static fn () => new class () {
            public function __isset($name)
            {
                throw new RuntimeException("__isset($name) failed");
            }
        };

        $this->assertSame(
            self::markAfter(static function (stdClass $target) use ($make) {
                $result = isset($make()->x);
                $target->marked = true;

                return $result;
            }),
            self::markAfter(static fn (stdClass $target) => Issue2831::issetThenMark($make(), $target))
        );
        $this->assertSame(
            self::markAfter(static function (stdClass $target) use ($make) {
                $result = empty($make()->x);
                $target->marked = true;

                return $result;
            }),
            self::markAfter(static fn (stdClass $target) => Issue2831::isEmptyThenMark($make(), $target))
        );
    }

    public function testThrowingGetStopsTheMethodLikePhp(): void
    {
        $this->assertSame(
            self::markAfter(static function (stdClass $target) {
                $object = self::throwingGet();
                $value  = $object->x;
                $target->marked = true;

                return $value;
            }),
            self::markAfter(static fn (stdClass $target) => Issue2831::readThenMark(self::throwingGet(), $target))
        );
    }

    public function testProtectedReadStopsTheMethodLikePhp(): void
    {
        $this->assertSame(
            self::markAfter(static function (stdClass $target) {
                $value = (new Issue2831Base())->prot;
                $target->marked = true;

                return $value;
            }),
            self::markAfter(
                static fn (stdClass $target) => Issue2831::readProtectedThenMark(new Issue2831Base(), $target)
            )
        );
    }

    public function testUndefinedReadDoesNotStopTheMethod(): void
    {
        $this->assertSame(
            [null, true],
            @self::markAfter(static fn (stdClass $target) => Issue2831::readThenMark(new stdClass(), $target))
        );
    }
}
