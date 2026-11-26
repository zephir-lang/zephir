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
use Error;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Issue2784;
use Stub\Issue2784Base;
use Stub\Issue2784Child;
use Stub\Issue2784Magic;

/**
 * A property read, write or unset from outside the class that declares it
 * behaves as PHP's: an inaccessible property throws an Error once, or goes to
 * `__get()` or `__unset()` when the class has it, an undefined one warns, and
 * the method stops there. The calling class decides, never the class of the
 * object, and a function has no class whoever calls it. PHP is the oracle; the
 * closures below run in this test's scope, which is as unrelated to the Stub
 * classes as the issue's `POther`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
final class Issue2784Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * An object whose inaccessible property goes to `__unset()`.
     */
    private static function withUnset(): object
    {
        return new class () {
            public array $log = [];

            protected $prot = 'prot';

            public function __unset($name)
            {
                $this->log[] = '__unset(' . $name . ')';
            }
        };
    }

    /**
     * An object whose methods hand `$this` to a callable, so the callable is
     * called from inside the object's class.
     */
    private static function holder(): object
    {
        return new class () {
            protected $prot = 'prot';

            public function run(callable $callable)
            {
                return $callable($this);
            }
        };
    }

    /**
     * The PHP counterpart of a Zephir function: code with no class scope.
     */
    private static function unscoped(callable $callable): callable
    {
        return Closure::bind(Closure::fromCallable($callable), null, null);
    }

    /**
     * An object answering an undefined or inaccessible `x` with magic, logging
     * every call.
     */
    private static function magicOffsetHolder(bool $declared): object
    {
        $object = $declared
            ? new class () {
                public array $log = [];

                protected $x = ['k' => 1, 'j' => 2];

                public function __isset($name)
                {
                    $this->log[] = '__isset(' . $name . ')';

                    return true;
                }

                public function __get($name)
                {
                    $this->log[] = '__get(' . $name . ')';

                    return ['k' => 1];
                }

                public function __set($name, $value)
                {
                    $this->log[] = '__set(' . $name . ')';
                }
            }
            : new class () {
                public array $log = [];

                public function __isset($name)
                {
                    $this->log[] = '__isset(' . $name . ')';

                    return true;
                }

                public function __get($name)
                {
                    $this->log[] = '__get(' . $name . ')';

                    return ['k' => 1];
                }

                public function __set($name, $value)
                {
                    $this->log[] = '__set(' . $name . ')';
                }
            };

        return $object;
    }

    public static function functionProvider(): array
    {
        return [
            'read'  => [
                'Stub\issue2784_read',
                static fn ($object) => $object->prot,
            ],
            'write' => [
                'Stub\issue2784_write',
                static function ($object) {
                    $object->prot = 'changed';

                    return 'written';
                },
            ],
            'unset' => [
                'Stub\issue2784_unset',
                static function ($object) {
                    unset($object->prot);

                    return 'unset';
                },
            ],
            'isset' => [
                'Stub\issue2784_isset',
                static fn ($object) => isset($object->prot),
            ],
        ];
    }

    /**
     * @dataProvider functionProvider
     */
    public function testFunctionCalledFromAClassHasNoScopeLikePhp(string $function, callable $php): void
    {
        $this->assertMatchesPhp(
            static fn () => self::holder()->run($function),
            static fn () => self::holder()->run(self::unscoped($php))
        );
    }

    public static function offsetUnsetProvider(): array
    {
        return [
            'undefined property' => [false],
            'protected property' => [true],
        ];
    }

    /**
     * PHP fetches the property for unset: `__get()` alone answers, and the
     * engine notes that unsetting an offset of what it returned does nothing.
     *
     * @dataProvider offsetUnsetProvider
     */
    public function testUnsettingAnOffsetOfAMagicPropertyMatchesPhp(bool $declared): void
    {
        $this->assertMatchesPhp(
            static function () use ($declared) {
                $object = self::magicOffsetHolder($declared);
                Issue2784::unsetOffset($object);

                return $object->log;
            },
            static function () use ($declared) {
                $object = self::magicOffsetHolder($declared);
                unset($object->x['k']);

                return $object->log;
            }
        );
    }

    /**
     * What a write to an inaccessible property throws, with what it chains.
     */
    private static function writeError(callable $write): array
    {
        try {
            $write();
        } catch (Error $e) {
            return [get_class($e), $e->getMessage(), $e->getPrevious()];
        }

        return [];
    }

    public static function inaccessibleWriteProvider(): array
    {
        return [
            'offset write'        => [
                static fn ($object) => Issue2784::writeProtOffset($object),
                static function ($object) {
                    $object->prot['k'] = 1;
                },
            ],
            'by-reference argument' => [
                static fn ($object) => Issue2784::pushIntoProt($object),
                static function ($object) {
                    array_push($object->prot, 1);
                },
            ],
        ];
    }

    /**
     * PHP stops at the Error the fetch for the write throws; it is not thrown
     * a second time with the first as its previous.
     *
     * @dataProvider inaccessibleWriteProvider
     */
    public function testWriteThroughAnInaccessiblePropertyThrowsOnceLikePhp(callable $extension, callable $php): void
    {
        $this->assertSame(
            self::writeError(static fn () => $php(new Issue2784Base())),
            self::writeError(static fn () => $extension(new Issue2784Base()))
        );
    }

    public function testUnsettingAnOffsetLeavesACopyOfThePropertyAlone(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object    = new stdClass();
                $object->x = ['k' => 1, 'j' => 2];
                $copy      = $object->x;
                Issue2784::unsetOffset($object);

                return [$object->x, $copy];
            },
            static function () {
                $object    = new stdClass();
                $object->x = ['k' => 1, 'j' => 2];
                $copy      = $object->x;
                unset($object->x['k']);

                return [$object->x, $copy];
            }
        );
    }

    public function testReadingAProtectedPropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2784())->readProt(new Issue2784Base()),
            static fn () => (new Issue2784Base())->prot
        );
    }

    public function testReadingAPrivatePropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2784())->readPriv(new Issue2784Base()),
            static fn () => (new Issue2784Base())->priv
        );
    }

    public function testReadingAnUndefinedPropertyWarnsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2784())->readUndefined(new Issue2784Base()),
            static fn () => (new Issue2784Base())->nope
        );
    }

    public function testWritingAProtectedPropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => (new Issue2784())->writeProt(new Issue2784Base()),
            static function () {
                $object       = new Issue2784Base();
                $object->prot = 'changed';

                return 'written';
            }
        );
    }

    public function testUnsettingAProtectedPropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::unsetProt(new Issue2784Base()),
            static function () {
                $object = new Issue2784Base();
                unset($object->prot);

                return 'unset';
            }
        );
    }

    public function testUnsettingAPrivatePropertyThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::unsetPriv(new Issue2784Base()),
            static function () {
                $object = new Issue2784Base();
                unset($object->priv);

                return 'unset';
            }
        );
    }

    public function testUnsettingAProtectedPropertyByNameThrowsLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::unsetNamed(new Issue2784Base(), 'prot'),
            static function () {
                $object = new Issue2784Base();
                $name   = 'prot';
                unset($object->{$name});

                return 'unset';
            }
        );
    }

    public function testUnsetThatThrowsStopsTheMethod(): void
    {
        $target = new stdClass();

        try {
            Issue2784::unsetProtThenMark(new Issue2784Base(), $target);
            $this->fail('The unset must throw');
        } catch (Error $e) {
            $this->assertSame('Cannot access protected property Stub\Issue2784Base::$prot', $e->getMessage());
        }

        $this->assertFalse(isset($target->marked));
    }

    public function testUnsettingAnInaccessiblePropertyCallsUnsetLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = self::withUnset();
                Issue2784::unsetProt($object);

                return $object->log;
            },
            static function () {
                $object = self::withUnset();
                unset($object->prot);

                return $object->log;
            }
        );
    }

    public function testUnsettingAnIntNamedPropertyConvertsTheNameLikePhp(): void
    {
        $this->assertMatchesPhp(
            static function () {
                $object = new stdClass();
                $object->{1} = 'one';
                $object->{2} = 'two';
                Issue2784::unsetNamed($object, 1);

                return (array) $object;
            },
            static function () {
                $object = new stdClass();
                $object->{1} = 'one';
                $object->{2} = 'two';
                $name = 1;
                unset($object->{$name});

                return (array) $object;
            }
        );
    }

    public function testUnsettingAPropertyOfANonObjectIsSilentLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::unsetNamed(null, 'prot'),
            static function () {
                $object = null;
                $name   = 'prot';
                unset($object->{$name});

                return 'unset';
            }
        );
    }

    /**
     * PHP cannot bind a closure to the scope of an internal class, so the
     * oracle here is PHP's rule itself: a class reaches its own private
     * property on an instance of a subclass.
     */
    public function testParentMethodUnsetsItsPrivatePropertyOnASubclassInstance(): void
    {
        $this->assertFalse((new Issue2784Child())->unsetOwnPrivate());
    }

    public function testParentMethodUnsetsItsPrivatePropertyOnItsOwnInstance(): void
    {
        $this->assertFalse((new Issue2784Base())->unsetOwnPrivate());
    }

    public function testSubclassUnsetsAProtectedPropertyOfItsParentsInstance(): void
    {
        $this->assertFalse(Issue2784Child::unsetParentProtected(new Issue2784Base()));
    }

    public static function magicReadProvider(): array
    {
        return [
            'protected property of a typed receiver'  => [
                static fn () => Issue2784::readMagicProtected(new Issue2784Magic()),
                static fn () => (new Issue2784Magic())->prot,
            ],
            'private property of a typed receiver'    => [
                static fn () => Issue2784::readMagicPrivate(new Issue2784Magic()),
                static fn () => (new Issue2784Magic())->priv,
            ],
            'undeclared property of a typed receiver' => [
                static fn () => Issue2784::readMagicUndeclared(new Issue2784Magic()),
                static fn () => (new Issue2784Magic())->virtual,
            ],
            'private property of a parent read through this'     => [
                static fn () => (new Issue2784Magic())->readParentPrivate(),
                static fn () => (new Issue2784Magic())->priv,
            ],
            'private property of a parent returned through this' => [
                static fn () => (new Issue2784Magic())->returnParentPrivate(),
                static fn () => (new Issue2784Magic())->priv,
            ],
            'undeclared property returned through this'          => [
                static fn () => (new Issue2784Magic())->returnVirtual(),
                static fn () => (new Issue2784Magic())->virtual,
            ],
        ];
    }

    /**
     * The class of the reader cannot reach the property, or it is not
     * declared, so PHP asks `__get()`; so does the code calling the reader,
     * which is what the PHP side runs.
     *
     * @dataProvider magicReadProvider
     */
    public function testReadThatCannotReachThePropertyCallsGetLikePhp(callable $extension, callable $php): void
    {
        $this->assertMatchesPhp($extension, $php);
    }

    public function testIssetOfAnOffsetOfAPrivatePropertyIsFalseLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::issetPrivateOffset(new Issue2784Base()),
            static function () {
                $object = new Issue2784Base();

                return isset($object->priv['k']);
            }
        );
    }

    public function testEmptyOfAPropertyOfAProtectedPropertyIsTrueLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::emptyPropertyOfProtected(new Issue2784Base()),
            static function () {
                $object = new Issue2784Base();

                return empty($object->prot->x);
            }
        );
    }

    public function testFetchOfAnOffsetOfAPrivatePropertyFindsNothingLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2784::fetchPrivateOffset(new Issue2784Base()),
            static function () {
                $object = new Issue2784Base();

                return isset($object->priv['k']) ? $object->priv['k'] : 'none';
            }
        );
    }

    public function testClosureReadsAPrivatePropertyOfItsClass(): void
    {
        $closure = Issue2784Base::closureReadingPrivate();

        $this->assertSame('Issue2784Base::priv', $closure(new Issue2784Base()));
        $this->assertSame('Issue2784Base::priv', $closure(new Issue2784Child()));
    }
}
