<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

/*
 * No strict_types: the PHP side is the oracle for Zephir code, which is never
 * strict, so a typed property must coerce on both sides alike.
 */

namespace Extension;

use Closure;
use ErrorException;
use PHPUnit\Framework\TestCase;
use RuntimeException;
use stdClass;
use ArrayAccess;
use Stub\ArrayWrite;
use Stub\Issue2747;
use Stub\Issue2808;
use Stub\Issue2808Counter;
use Throwable;

use function get_class;
use function get_object_vars;
use function implode;
use function is_object;
use function restore_error_handler;
use function set_error_handler;
use function strlen;
use function ucfirst;
use function var_export;

/**
 * A compound assignment, `++` or `--` of a property must behave as PHP's
 * ASSIGN_OBJ_OP and PRE_INC_OBJ: an undefined property is created before the
 * warning (after the 8.2 dynamic property deprecation), the operator applies
 * in place without `__set()`, and a non-object throws. A plain assignment to
 * a non-object throws too, and a write that throws stops the method. PHP is
 * the oracle.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
final class Issue2808Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * Objects a property operation can meet, each built fresh per run.
     */
    public static function objects(): array
    {
        $objects = [
            'stdClass'                => static fn () => new stdClass(),
            'plain class'             => static fn () => new class () {
            },
            'AllowDynamicProperties'  => static fn () => new #[\AllowDynamicProperties] class () {
            },
            'only __set'              => static fn () => new class () {
                public array $log = [];

                public function __set($name, $value)
                {
                    $this->log[] = "__set($name)=" . var_export($value, true);
                }
            },
            'only __get'              => static fn () => new class () {
                public array $log = [];

                public function __get($name)
                {
                    $this->log[] = "__get($name)";

                    return 3;
                }
            },
            '__get and __set'         => static fn () => new class () {
                public array $log = [];

                public function __get($name)
                {
                    $this->log[] = "__get($name)";

                    return 3;
                }

                public function __set($name, $value)
                {
                    $this->log[] = "__set($name)=" . var_export($value, true);
                }
            },
            'declared null'           => static fn () => new class () {
                public $x = null;
            },
            'declared value'          => static fn () => new class () {
                public $x = 5;
            },
            'declared unset'          => static function () {
                $object = new class () {
                    public $x = 5;
                };
                unset($object->x);

                return $object;
            },
            'declared unset with __get' => static function () {
                $object = new class () {
                    public array $log = [];
                    public $x = 5;

                    public function __get($name)
                    {
                        $this->log[] = "__get($name)";

                        return 3;
                    }
                };
                unset($object->x);

                return $object;
            },
            'typed int'               => static fn () => new class () {
                public int $x = 5;
            },
            'typed int at max'        => static fn () => new class () {
                public int $x = PHP_INT_MAX;
            },
            'typed int at min'        => static fn () => new class () {
                public int $x = PHP_INT_MIN;
            },
            'typed float'             => static fn () => new class () {
                public float $x = 1.5;
            },
            'typed nullable int'      => static fn () => new class () {
                public ?int $x = null;
            },
            'typed uninitialized'     => static fn () => new class () {
                public ?int $x;
            },
            'typed string'            => static fn () => new class () {
                public string $x = 'a';
            },
            'typed int|string'        => static fn () => new class () {
                public int|string $x = 5;
            },
            'typed reference'         => static function () {
                $object = new class () {
                    public int $x = 5;
                };
                $reference = &$object->x;

                return $object;
            },
            'reference'               => static function () {
                $object = new class () {
                    public $x = 5;
                };
                $reference = &$object->x;

                return $object;
            },
        ];

        if (PHP_VERSION_ID >= 80100) {
            $objects['readonly'] = static fn () => eval(
                'return new class () { public function __construct(public readonly int $x = 5) {} };'
            );
        }

        if (PHP_VERSION_ID >= 80400) {
            $objects['hooked'] = static fn () => eval(
                'return new class () {'
                . ' public array $log = []; private $v = 5;'
                . ' public $x { get { $this->log[] = "get"; return $this->v; }'
                . ' set { $this->log[] = "set=" . var_export($value, true); $this->v = $value; } }'
                . ' };'
            );
        }

        return $objects;
    }

    /**
     * Each operation as the extension runs it and as PHP does.
     */
    public static function operations(): array
    {
        return [
            'add'                 => [
                static fn ($o) => Issue2808::add($o, 2),
                static fn ($o) => $o->x += 2,
            ],
            'sub'                 => [
                static fn ($o) => Issue2808::sub($o, 2),
                static fn ($o) => $o->x -= 2,
            ],
            'mul'                 => [
                static fn ($o) => Issue2808::mul($o, 2),
                static fn ($o) => $o->x *= 2,
            ],
            'div'                 => [
                static fn ($o) => Issue2808::div($o, 2),
                static fn ($o) => $o->x /= 2,
            ],
            'mod'                 => [
                static fn ($o) => Issue2808::mod($o, 2),
                static fn ($o) => $o->x %= 2,
            ],
            'concat'              => [
                static fn ($o) => Issue2808::concat($o, 's'),
                static fn ($o) => $o->x .= 's',
            ],
            'bitwise and'         => [
                static fn ($o) => Issue2808::bitwiseAnd($o, 3),
                static fn ($o) => $o->x &= 3,
            ],
            'bitwise or'          => [
                static fn ($o) => Issue2808::bitwiseOr($o, 3),
                static fn ($o) => $o->x |= 3,
            ],
            'bitwise xor'         => [
                static fn ($o) => Issue2808::bitwiseXor($o, 3),
                static fn ($o) => $o->x ^= 3,
            ],
            'shift left'          => [
                static fn ($o) => Issue2808::shiftLeft($o, 1),
                static fn ($o) => $o->x <<= 1,
            ],
            'shift right'         => [
                static fn ($o) => Issue2808::shiftRight($o, 1),
                static fn ($o) => $o->x >>= 1,
            ],
            'add int literal'     => [
                static fn ($o) => Issue2808::addOne($o),
                static fn ($o) => $o->x += 1,
            ],
            'add double literal'  => [
                static fn ($o) => Issue2808::addDouble($o),
                static fn ($o) => $o->x += 1.5,
            ],
            'concat literal'      => [
                static fn ($o) => Issue2808::concatLiteral($o),
                static fn ($o) => $o->x .= 's',
            ],
            'concat char'         => [
                static fn ($o) => Issue2808::concatChar($o),
                static fn ($o) => $o->x .= 'A',
            ],
            'concat char literal' => [
                static fn ($o) => Issue2808::concatCharLiteral($o),
                static fn ($o) => $o->x .= 'B',
            ],
            'add char'            => [
                static fn ($o) => Issue2808::addChar($o),
                static fn ($o) => $o->x += 65,
            ],
            'add call'            => [
                static fn ($o) => Issue2808::addCall($o, 2),
                static fn ($o) => $o->x += Issue2808::identity(2),
            ],
            'add dynamic'         => [
                static fn ($o) => Issue2808::addDynamic($o, 'x', 2),
                static function ($o) {
                    $name = 'x';
                    $o->{$name} += 2;
                },
            ],
            'concat dynamic'      => [
                static fn ($o) => Issue2808::concatDynamic($o, 'x', 's'),
                static function ($o) {
                    $name = 'x';
                    $o->{$name} .= 's';
                },
            ],
            'add string name'     => [
                static fn ($o) => Issue2808::addString($o, 2),
                static fn ($o) => $o->{'x'} += 2,
            ],
            'concat string name'  => [
                static fn ($o) => Issue2808::concatString($o, 's'),
                static fn ($o) => $o->{'x'} .= 's',
            ],
            'increment'           => [
                static fn ($o) => Issue2808::increment($o),
                static fn ($o) => $o->x++,
            ],
            'decrement'           => [
                static fn ($o) => Issue2808::decrement($o),
                static fn ($o) => $o->x--,
            ],
            'assign'              => [
                static fn ($o) => Issue2808::assign($o, 2),
                static fn ($o) => $o->x = 2,
            ],
            'assign dynamic'      => [
                static fn ($o) => Issue2808::assignDynamic($o, 'x', 2),
                static function ($o) {
                    $name = 'x';
                    $o->{$name} = 2;
                },
            ],
        ];
    }

    public static function objectOperationProvider(): array
    {
        $cases = [];
        foreach (self::objects() as $object => $make) {
            foreach (self::operations() as $operation => [$extension, $php]) {
                $cases["$operation on $object"] = [$make, $extension, $php];
            }
        }

        return $cases;
    }

    public static function nonObjectOperationProvider(): array
    {
        $values = [
            'null'   => null,
            'false'  => false,
            'int'    => 1,
            'string' => 'str',
            'array'  => [],
        ];

        $cases = [];
        foreach ($values as $label => $value) {
            foreach (self::operations() as $operation => [$extension, $php]) {
                $cases["$operation on $label"] = [static fn () => $value, $extension, $php];
            }
        }

        return $cases;
    }

    /**
     * Runs one operation and reports what it threw and the state it left.
     */
    private static function outcome(mixed $subject, Closure $operation): array
    {
        try {
            $operation($subject);
            $caught = null;
        } catch (Throwable $e) {
            $caught = get_class($e) . ': ' . $e->getMessage();
        }

        return [$caught, is_object($subject) ? get_object_vars($subject) : $subject];
    }

    /**
     * Like transcript(), with an error handler that throws, as frameworks
     * install one.
     */
    private function throwingTranscript(callable $subject): string
    {
        $diagnostics = [];

        set_error_handler(static function (int $code, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $message;

            throw new ErrorException($message, 0, $code);
        });

        try {
            $result = var_export($subject(), true);
        } finally {
            restore_error_handler();
        }

        return implode(' ; ', $diagnostics) . ' => ' . $result;
    }

    /**
     * @dataProvider objectOperationProvider
     * @dataProvider nonObjectOperationProvider
     */
    public function testOperationLikePhp(callable $make, Closure $extension, Closure $php): void
    {
        $this->assertMatchesPhp(
            static fn () => self::outcome($make(), $extension),
            static fn () => self::outcome($make(), $php)
        );
    }

    /**
     * @dataProvider objectOperationProvider
     */
    public function testOperationUnderAThrowingErrorHandlerLikePhp(callable $make, Closure $extension, Closure $php): void
    {
        $this->assertSame(
            $this->throwingTranscript(static fn () => self::outcome($make(), $php)),
            $this->throwingTranscript(static fn () => self::outcome($make(), $extension)),
            'The extension must behave exactly as PHP does'
        );
    }

    public static function valueProvider(): array
    {
        return [
            'array'             => [[1]],
            'non-numeric'       => ['abc'],
            'leading-numeric'   => ['5 apples'],
            'null'              => [null],
            'true'              => [true],
            'float'             => [2.5],
            'object'            => [new stdClass()],
            'zero'              => [0],
            'negative'          => [-1],
        ];
    }

    /**
     * The right-hand side reaches PHP's operator, with its warnings and errors.
     *
     * @dataProvider valueProvider
     */
    public function testOperandLikePhp(mixed $value): void
    {
        foreach (['add', 'sub', 'mul', 'div', 'mod', 'concat', 'bitwiseAnd', 'bitwiseOr', 'bitwiseXor', 'shiftLeft', 'shiftRight'] as $method) {
            $operator = [
                'add'        => static fn ($o) => $o->x += $value,
                'sub'        => static fn ($o) => $o->x -= $value,
                'mul'        => static fn ($o) => $o->x *= $value,
                'div'        => static fn ($o) => $o->x /= $value,
                'mod'        => static fn ($o) => $o->x %= $value,
                'concat'     => static fn ($o) => $o->x .= $value,
                'bitwiseAnd' => static fn ($o) => $o->x &= $value,
                'bitwiseOr'  => static fn ($o) => $o->x |= $value,
                'bitwiseXor' => static fn ($o) => $o->x ^= $value,
                'shiftLeft'  => static fn ($o) => $o->x <<= $value,
                'shiftRight' => static fn ($o) => $o->x >>= $value,
            ][$method];

            foreach (['int' => 6, 'string' => 'str', 'array' => [2]] as $label => $initial) {
                $make = static function () use ($initial) {
                    $object    = new stdClass();
                    $object->x = $initial;

                    return $object;
                };

                $this->assertMatchesPhp(
                    static fn () => self::outcome($make(), static fn ($o) => Issue2808::$method($o, $value)),
                    static fn () => self::outcome($make(), $operator),
                    "$method on $label"
                );
            }
        }
    }

    public static function nameProvider(): array
    {
        return [
            'int'       => [1],
            'float'     => [1.5],
            'true'      => [true],
            'null'      => [null],
            'array'     => [[]],
            'stringable' => [new class () {
                public function __toString(): string
                {
                    return 'named';
                }
            }],
            'object'    => [new stdClass()],
        ];
    }

    /**
     * A runtime name is converted as PHP converts it.
     *
     * @dataProvider nameProvider
     */
    public function testDynamicNameIsConvertedLikePhp(mixed $name): void
    {
        $this->assertMatchesPhp(
            static fn () => self::outcome(new stdClass(), static fn ($o) => Issue2808::assignDynamic($o, $name, 'v')),
            static fn () => self::outcome(new stdClass(), static fn ($o) => $o->{$name} = 'v'),
            'assign'
        );

        $this->assertMatchesPhp(
            static fn () => self::outcome(new stdClass(), static fn ($o) => Issue2808::addDynamic($o, $name, 2)),
            static fn () => self::outcome(new stdClass(), static fn ($o) => $o->{$name} += 2),
            'add'
        );

        $this->assertMatchesPhp(
            static fn () => self::outcome(null, static fn ($o) => Issue2808::addDynamic($o, $name, 2)),
            static fn () => self::outcome(null, static fn ($o) => $o->{$name} += 2),
            'add on null'
        );

        $this->assertMatchesPhp(
            static fn () => self::outcome(null, static fn ($o) => Issue2808::assignDynamic($o, $name, 'v')),
            static fn () => self::outcome(null, static fn ($o) => $o->{$name} = 'v'),
            'assign on null'
        );
    }

    public function testNativeNameIsConvertedLikePhp(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::outcome(new stdClass(), static fn ($o) => Issue2808::addNativeName($o)),
            static fn () => self::outcome(new stdClass(), static fn ($o) => $o->{7} += 1)
        );
    }

    /**
     * Runs `$write` with a fresh target, and tells whether the code after the
     * write still ran.
     */
    private static function markAfter(Closure $write): array
    {
        $target = new stdClass();

        try {
            $write($target);
            $caught = null;
        } catch (Throwable $e) {
            $caught = get_class($e) . ': ' . $e->getMessage();
        }

        return [$caught, isset($target->marked)];
    }

    public static function failingWriteProvider(): array
    {
        return [
            'typed'      => [static fn () => new class () {
                public int $x = 5;
            }],
            'throwing __set' => [static fn () => new class () {
                public function __set($name, $value)
                {
                    throw new RuntimeException("__set($name) failed");
                }
            }],
            'non-object' => [static fn () => null],
        ];
    }

    /**
     * @dataProvider failingWriteProvider
     */
    public function testFailingWriteStopsTheMethodLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static fn () => self::markAfter(static fn ($target) => Issue2808::assignThenMark($make(), 'abc', $target)),
            static fn () => self::markAfter(static function ($target) use ($make) {
                $make()->x      = 'abc';
                $target->marked = true;
            }),
            'assign'
        );

        $this->assertMatchesPhp(
            static fn () => self::markAfter(
                static fn ($target) => Issue2808::assignDynamicThenMark($make(), 'x', 'abc', $target)
            ),
            static fn () => self::markAfter(static function ($target) use ($make) {
                $name             = 'x';
                $make()->{$name}  = 'abc';
                $target->marked   = true;
            }),
            'assign dynamic'
        );

        $this->assertMatchesPhp(
            static fn () => self::markAfter(static fn ($target) => Issue2808::addThenMark($make(), [1], $target)),
            static fn () => self::markAfter(static function ($target) use ($make) {
                $make()->x     += [1];
                $target->marked = true;
            }),
            'add'
        );

        $this->assertMatchesPhp(
            static fn () => self::markAfter(static fn ($target) => Issue2808::incrementThenMark($make(), $target)),
            static fn () => self::markAfter(static function ($target) use ($make) {
                $make()->x++;
                $target->marked = true;
            }),
            'increment'
        );
    }

    /**
     * A subclass whose PHP methods mirror the extension's, so both sides
     * report the same class names.
     */
    private static function counter(): Issue2808Counter
    {
        return new class () extends Issue2808Counter {
            public function phpIncrement(): void
            {
                $this->counter++;
            }

            public function phpDecrement(): void
            {
                $this->counter--;
            }

            public function phpAddCounter($v): void
            {
                $this->counter += $v;
            }

            public function phpAppendText($v): void
            {
                $this->text .= $v;
            }

            public function phpAddPlain($v): void
            {
                $this->plain += $v;
            }

            public function phpAppendPlain($v): void
            {
                $this->plain .= $v;
            }

            public function phpIncrementPlain(): void
            {
                $this->plain++;
            }
        };
    }

    public static function counterProvider(): array
    {
        return [
            'increment at max'          => [static fn ($c) => $c->counter = PHP_INT_MAX, 'increment', []],
            'decrement at min'          => [static fn ($c) => $c->counter = PHP_INT_MIN, 'decrement', []],
            'increment'                 => [static fn ($c) => $c->counter = 1, 'increment', []],
            'add one'                   => [static fn ($c) => $c->counter = 1, 'addCounter', [1]],
            'add float'                 => [static fn ($c) => $c->counter = 1, 'addCounter', [1.5]],
            'add integral float'        => [static fn ($c) => $c->counter = 1, 'addCounter', [2.0]],
            'add numeric string'        => [static fn ($c) => $c->counter = 1, 'addCounter', ['2']],
            'add past max'              => [static fn ($c) => $c->counter = PHP_INT_MAX, 'addCounter', [1]],
            'append text'               => [static fn ($c) => $c->text = 'a', 'appendText', ['b']],
            'append array to text'      => [static fn ($c) => $c->text = 'a', 'appendText', [[]]],
            'append int to text'        => [static fn ($c) => $c->text = 'a', 'appendText', [5]],
            'add plain'                 => [static fn ($c) => $c->plain = 1, 'addPlain', [1]],
            'add to unset plain'        => [static function ($c) {
                unset($c->plain);
            }, 'addPlain', [1]],
            'append to unset plain'     => [static function ($c) {
                unset($c->plain);
            }, 'appendPlain', ['s']],
            'increment unset plain'     => [static function ($c) {
                unset($c->plain);
            }, 'incrementPlain', []],
            'increment string plain'    => [static fn ($c) => $c->plain = 'a', 'incrementPlain', []],
            'increment null plain'      => [static fn ($c) => $c->plain = null, 'incrementPlain', []],
            'increment uninit counter'  => [static function ($c) {
                unset($c->counter);
            }, 'increment', []],
        ];
    }

    /**
     * `this->p` goes through the inline cache.
     *
     * @dataProvider counterProvider
     */
    public function testOperationOnThisLikePhp(Closure $prepare, string $method, array $arguments): void
    {
        $php = 'php' . ucfirst($method);

        foreach ([false, true] as $throwing) {
            $extension = static function () use ($prepare, $method, $arguments) {
                $counter = self::counter();
                $prepare($counter);

                return self::outcome($counter, static fn ($c) => $c->$method(...$arguments));
            };
            $mirror    = static function () use ($prepare, $php, $arguments) {
                $counter = self::counter();
                $prepare($counter);

                return self::outcome($counter, static fn ($c) => $c->$php(...$arguments));
            };

            if ($throwing) {
                $this->assertSame($this->throwingTranscript($mirror), $this->throwingTranscript($extension));
            } else {
                $this->assertMatchesPhp($extension, $mirror);
            }
        }
    }

    /**
     * A message naming an anonymous class prints it as PHP does: cut at its
     * NUL byte before PHP 8.6, whole from 8.6 (GH-25352).
     */
    public function testAnonymousClassNameInMessagesLikePhp(): void
    {
        $typed = static fn () => new class () {
            public ?int $p = null;
        };
        $this->assertMatchesPhp(
            static fn () => self::outcome($typed(), static fn ($o) => (new ArrayWrite())->writeObject($o, 'k', 1)),
            static fn () => self::outcome($typed(), static fn ($o) => $o->p['k'] = 1),
            'auto-initialize'
        );

        $plain = static fn () => new class () {
        };
        $this->assertMatchesPhp(
            static fn () => self::outcome($plain(), static fn ($o) => (new Issue2747())->add($o, 'k', 1)),
            static fn () => self::outcome($plain(), static fn ($o) => $o['k'] += 1),
            'object as array'
        );

        $offsets = static fn () => new class () implements ArrayAccess {
            public function offsetExists($offset): bool
            {
                return true;
            }

            public function offsetGet($offset): mixed
            {
                return ['j' => 1];
            }

            public function offsetSet($offset, $value): void
            {
            }

            public function offsetUnset($offset): void
            {
            }
        };
        $this->assertMatchesPhp(
            static fn () => self::outcome($offsets(), static fn ($o) => (new Issue2747())->addNested($o, 'i', 'j', 1)),
            static fn () => self::outcome($offsets(), static fn ($o) => $o['i']['j'] += 1),
            'indirect modification'
        );

        $this->assertMatchesPhp(
            static fn () => self::outcome(self::objects()['typed int at max'](), static fn ($o) => Issue2808::increment($o)),
            static fn () => self::outcome(self::objects()['typed int at max'](), static fn ($o) => $o->x++),
            'increment past the limit'
        );
    }

    public static function typedOffsetProvider(): array
    {
        return [
            '?int null'      => [static fn () => new class () {
                public ?int $p = null;
            }],
            '?array null'    => [static fn () => new class () {
                public ?array $p = null;
            }],
            'int|false'      => [static fn () => new class () {
                public int|false $p = false;
            }],
            'int'            => [static fn () => new class () {
                public int $p = 5;
            }],
            'uninitialized'  => [static fn () => new class () {
                public ?int $p;
            }],
        ];
    }

    /**
     * PHP fetches the property of `o->p[k] op= v` read-write, without the
     * check a plain `o->p[k] = v` makes, so an array is auto-initialized even
     * into a property whose type does not allow one.
     *
     * @dataProvider typedOffsetProvider
     */
    public function testCompoundOffsetOnATypedPropertyLikePhp(callable $make): void
    {
        $this->assertMatchesPhp(
            static fn () => self::outcome($make(), static fn ($o) => (new Issue2747())->addObject($o, 'k', 1)),
            static fn () => self::outcome($make(), static fn ($o) => $o->p['k'] += 1),
            'compound'
        );

        $this->assertMatchesPhp(
            static fn () => self::outcome($make(), static fn ($o) => (new ArrayWrite())->writeObject($o, 'k', 1)),
            static fn () => self::outcome($make(), static fn ($o) => $o->p['k'] = 1),
            'assign'
        );
    }

    /**
     * The same object updated many times keeps one value.
     */
    public function testRepeatedOperationsAccumulate(): void
    {
        $counter = self::counter();
        for ($i = 0; $i < 1000; $i++) {
            $counter->increment();
            $counter->addCounter(2);
            $counter->appendText('x');
        }

        $this->assertSame([3000, 1000], [$counter->counter, strlen($counter->text)]);
    }
}
