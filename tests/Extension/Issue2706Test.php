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

use ArrayAccess;
use ArrayObject;
use PHPUnit\Framework\TestCase;
use ReturnTypeWillChange;
use RuntimeException;
use Stub\Issue2706;
use stdClass;
use TypeError;

/**
 * Array keys PHP derives from a non-string offset, against what PHP does with
 * the same statement.
 *
 * `unset` and array_key_exists() looked a `null` offset up as a one byte NUL
 * key instead of the empty string, and converted float, bool and resource
 * keys and rejected illegal ones on their own terms.
 *
 * array_key_exists() is called unqualified from this namespace on purpose:
 * that is the function, as Zephir calls it, not the opcode PHP compiles a
 * global call into, and the two word an illegal key differently.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
final class Issue2706Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2706 $subject;

    /**
     * @var resource
     */
    private static $resource;

    protected function setUp(): void
    {
        $this->subject = new Issue2706();
    }

    public static function keyProvider(): array
    {
        self::$resource ??= fopen('php://memory', 'r');

        return [
            'null'          => [null],
            'empty string'  => [''],
            'numeric'       => ['1'],
            'string'        => ['k'],
            'int'           => [1],
            'float'         => [1.5],
            'whole float'   => [2.0],
            'nan'           => [NAN],
            'inf'           => [INF],
            'true'          => [true],
            'false'         => [false],
            'resource'      => [self::$resource],
            'array'         => [[]],
            'object'        => [new stdClass()],
        ];
    }

    public static function containerProvider(): array
    {
        return [
            'null'         => [null],
            'string'       => ['abc'],
            'int'          => [5],
            'ArrayAccess'  => [new ArrayObject(['k' => 1])],
        ];
    }

    /**
     * The issue as filed.
     */
    public function testANullKeyIsTheEmptyString(): void
    {
        $written = @$this->subject->write(null);

        $this->assertSame([''], array_map('strval', array_keys($written)));
        $this->assertSame(0, strlen((string) array_key_first($written)));
        $this->assertSame([true, true, 'written'], @$this->subject->readBack(null));
    }

    /**
     * @dataProvider keyProvider
     */
    public function testWrite(mixed $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->write($key),
            static function () use ($key) {
                $a       = [];
                $a[$key] = 'written';

                return $a;
            }
        );
    }

    /**
     * @dataProvider keyProvider
     */
    public function testReadBack(mixed $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->readBack($key),
            static function () use ($key) {
                $a       = [];
                $a[$key] = 'written';

                return [isset($a[$key]), array_key_exists($key, $a), $a[$key]];
            }
        );
    }

    /**
     * @dataProvider keyProvider
     */
    public function testUnset(mixed $key): void
    {
        $base = self::base();

        $this->assertMatchesPhp(
            fn () => $this->subject->unsetKey($base, $key),
            static function () use ($base, $key) {
                unset($base[$key]);

                return $base;
            }
        );
    }

    /**
     * @dataProvider keyProvider
     */
    public function testUnsetPath(mixed $key): void
    {
        $base = array_map(static fn () => ['q' => 1, 'r' => 2], self::base());

        $this->assertMatchesPhp(
            fn () => $this->subject->unsetPath($base, $key),
            static function () use ($base, $key) {
                unset($base[$key]['q']);

                return $base;
            }
        );
    }

    /**
     * @dataProvider keyProvider
     */
    public function testKeyExists(mixed $key): void
    {
        $base = self::base();

        $this->assertMatchesPhp(
            fn () => $this->subject->keyExists($key, $base),
            static fn () => array_key_exists($key, $base)
        );
    }

    /**
     * @dataProvider keyProvider
     */
    public function testKeyExistsMisses(mixed $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->keyExists($key, ['other' => 1]),
            static fn () => array_key_exists($key, ['other' => 1])
        );
    }

    /**
     * @dataProvider containerProvider
     */
    public function testKeyExistsOnANonArray(mixed $container): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->keyExists('k', $container),
            static fn () => array_key_exists('k', $container)
        );
    }

    /**
     * @dataProvider containerProvider
     */
    public function testInArrayOnANonArray(mixed $container): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->inArray(1, $container),
            static fn () => in_array(1, $container)
        );
    }

    public static function haystackProvider(): array
    {
        return [
            'found'          => [1, [0, 1, 2]],
            'loosely found'  => ['1', [0, 1, 2]],
            'string found'   => ['b', ['a', 'b']],
            'missing'        => [5, [0, 1, 2]],
            'null in empty'  => [null, []],
            'null loosely'   => [null, [0]],
        ];
    }

    /**
     * @dataProvider haystackProvider
     */
    public function testInArray(mixed $needle, array $haystack): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->inArray($needle, $haystack),
            static fn () => in_array($needle, $haystack)
        );
    }

    public static function literalKeyProvider(): array
    {
        return [
            'var string'       => ['literalVarKey', 'k'],
            'var numeric'      => ['literalVarKey', '3'],
            'var int'          => ['literalVarKey', 3],
            'var null'         => ['literalVarKey', null],
            'var float'        => ['literalVarKey', 1.5],
            'var bool'         => ['literalVarKey', true],
            'var illegal'      => ['literalVarKey', []],
            'string'           => ['literalStringKey', 'k'],
            'string numeric'   => ['literalStringKey', '3'],
            'string empty'     => ['literalStringKey', ''],
            'int'              => ['literalIntKey', 3],
            'int negative'     => ['literalIntKey', -1],
        ];
    }

    /**
     * @dataProvider literalKeyProvider
     */
    public function testArrayLiteralWithAVariableKey(string $method, mixed $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->{$method}($key, 'val', 7, 2.5, false, 'str'),
            static function () use ($key) {
                [$v, $iv, $dv, $bv, $sv] = ['val', 7, 2.5, false, 'str'];

                return [
                    [$key => 1], [$key => 1.5], [$key => 's'], [$key => true], [$key => null], [$key => $v],
                    [$key => [1]], [$key => $iv], [$key => $dv], [$key => $bv], [$key => $sv],
                ];
            }
        );
    }

    /**
     * The same statement in PHP for each `key*` stub form, and whether it
     * takes the array.
     *
     * @return array<string, array{0: callable, 1: bool, 2: bool}> closure, takes an array, nested base
     */
    private static function keyForms(): array
    {
        return [
            'Write'       => [static function (array $a, $k) {
                $a[$k] = 1;
                return $a;
            }, true, false],
            'WriteNested' => [static function (array $a, $k) {
                $a[$k]['x'] = 1;
                return $a;
            }, true, true],
            'AddAssign'   => [static function (array $a, $k) {
                $a[$k] += 1;
                return $a;
            }, true, false],
            'AppendUnder' => [static function (array $a, $k) {
                $a[$k][] = 1;
                return $a;
            }, true, true],
            'Read'        => [static fn (array $a, $k) => $a[$k], true, false],
            'Isset'       => [static fn (array $a, $k) => isset($a[$k]), true, false],
            // One lookup, as fetch does: isset() followed by a read would convert the key twice
            'Fetch'       => [static function (array $a, $k) {
                $x = $a[$k] ?? null;
                return [null !== $x, $x];
            }, true, false],
            'Empty'       => [static fn (array $a, $k) => empty($a[$k]), true, false],
            'Unset'       => [static function (array $a, $k) {
                unset($a[$k]);
                return $a;
            }, true, false],
            'UnsetNested' => [static function (array $a, $k) {
                unset($a[$k]['x']);
                return $a;
            }, true, true],
            'Literal'     => [static fn (array $a, $k) => [$k => 'v'], false, false],
            'PropWrite'   => [static function (array $a, $k) {
                $p = [];
                $p[$k] = 1;
                return $p;
            }, false, false],
            'PropRead'    => [static fn (array $a, $k) => $a[$k], true, false],
            'PropUnset'   => [static function (array $a, $k) {
                unset($a[$k]);
                return $a;
            }, true, false],
            'StaticWrite' => [static function (array $a, $k) {
                $p = [];
                $p[$k] = 1;
                return $p;
            }, false, false],
        ];
    }

    public static function scalarKeyProvider(): array
    {
        $keys = [
            'Double' => ['1.5' => 1.5, '2.0' => 2.0, '-0.5' => -0.5, 'NAN' => NAN, 'INF' => INF],
            'Bool'   => ['true' => true, 'false' => false],
        ];
        $literals = ['LiteralDouble' => 1.5, 'LiteralTrue' => true, 'LiteralNull' => null];

        $cases = [];
        foreach (self::keyForms() as $form => [$php, $takesArray, $nested]) {
            foreach ($keys as $kind => $values) {
                foreach ($values as $label => $key) {
                    $cases["$kind $form $label"] = ["key$kind$form", $key, true, $takesArray, $nested, $php];
                }
            }
            foreach ($literals as $kind => $key) {
                $cases["$kind $form"] = ["key$kind$form", $key, false, $takesArray, $nested, $php];
            }
        }

        return $cases;
    }

    /**
     * @dataProvider scalarKeyProvider
     */
    public function testDoubleAndBoolKeys(
        string $method,
        mixed $key,
        bool $passesKey,
        bool $takesArray,
        bool $nested,
        callable $php
    ): void {
        $base = $nested
            ? [0 => ['x' => 0, 'y' => 0], 1 => ['x' => 1, 'y' => 1], 2 => ['x' => 2, 'y' => 2]]
            : [0 => 10, 1 => 0, 2 => 30];

        $arguments = array_merge($passesKey ? [$key] : [], $takesArray ? [$base] : []);

        $this->assertMatchesPhp(
            fn () => $this->subject->{$method}(...$arguments),
            static fn () => $php($base, $key)
        );
    }

    public static function appendUnderProvider(): array
    {
        return [
            'existing'       => [['k' => ['a']], 'k'],
            'missing'        => [[], 'k'],
            'numeric string' => [[3 => []], '3'],
            'scalar element' => [['k' => 5], 'k'],
            'string element' => [['k' => 'abc'], 'k'],
        ];
    }

    /**
     * @dataProvider appendUnderProvider
     */
    public function testAppendUnderAnArrayParameter(array $a, string $k): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->appendUnderArrayParam($a, $k),
            static function () use ($a, $k) {
                $a[$k][] = 'v';

                return $a;
            }
        );
    }

    /**
     * @dataProvider appendUnderProvider
     */
    public function testAppendUnderAVarParameter(array $a, string $k): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->appendUnderVarParam($a, $k),
            static function () use ($a, $k) {
                $a[$k][] = 'v';

                return $a;
            }
        );
    }

    public function testAppendUnderADeepArrayParameter(): void
    {
        $this->assertSame(['x' => [1 => ['v']]], $this->subject->appendUnderDeepArrayParam([]));
    }

    public function testKeyExistsIsNotEvaluatedWhenShortCircuited(): void
    {
        $this->assertTrue($this->subject->keyExistsOr(true, 'k', null));
    }

    public function testKeyExistsStillThrowsBehindAShortCircuit(): void
    {
        $this->expectException(TypeError::class);

        $this->subject->keyExistsOr(false, 'k', null);
    }

    public static function stopProvider(): array
    {
        $throwing = new class () implements ArrayAccess {
            public function offsetExists($offset): bool
            {
                throw new RuntimeException('offsetExists');
            }

            #[ReturnTypeWillChange]
            public function offsetGet($offset)
            {
                throw new RuntimeException('offsetGet');
            }

            public function offsetSet($offset, $value): void
            {
                throw new RuntimeException('offsetSet');
            }

            public function offsetUnset($offset): void
            {
                throw new RuntimeException('offsetUnset');
            }
        };

        $empty   = [];
        $illegal = [];
        $string  = 'abc';

        return [
            'write illegal key' => ['writeStops', [[], []], static function () use ($illegal): void {
                $a           = [];
                $a[$illegal] = 1;
            }],
            'write scalar' => ['writeStops', [5, 'k'], static function (): void {
                $a      = 5;
                $a['k'] = 1;
            }],
            'write offsetSet' => ['writeStops', [$throwing, 'k'], static function () use ($throwing): void {
                $throwing['k'] = 1;
            }],
            'write nested illegal key' => ['writeMultiStops', [[], []], static function () use ($illegal): void {
                $a                = [];
                $a[$illegal]['x'] = 1;
            }],
            'append string' => ['appendStops', ['abc'], static function (): void {
                $a   = 'abc';
                $a[] = 1;
            }],
            'append offsetSet' => ['appendStops', [$throwing], static function () use ($throwing): void {
                $throwing[] = 1;
            }],
            'read illegal key' => ['readStops', [[], []], static fn () => $empty[$illegal]],
            'read offsetGet' => ['readStops', [$throwing, 'k'], static fn () => $throwing['k']],
            'isset illegal key' => ['issetStops', [[], []], static fn () => isset($empty[$illegal])],
            'isset offsetExists' => ['issetStops', [$throwing, 'k'], static fn () => isset($throwing['k'])],
            'fetch illegal key' => ['fetchStops', [[], []], static fn () => isset($empty[$illegal])],
            'fetch offsetExists' => ['fetchStops', [$throwing, 'k'], static fn () => isset($throwing['k'])],
            'empty illegal key' => ['emptyStops', [[], []], static fn () => empty($empty[$illegal])],
            'empty offsetExists' => ['emptyStops', [$throwing, 'k'], static fn () => empty($throwing['k'])],
            'literal illegal key' => ['literalStops', [[], 1], static fn () => [$illegal => 1]],
            'property write illegal key' => ['propertyWriteStops', [[]], static function () use ($illegal): void {
                $a           = [];
                $a[$illegal] = 1;
            }],
            'string offset illegal' => ['stringOffsetStops', ['abc', []], static fn () => $string[$illegal]],
            'key exists illegal key' => ['keyExistsStops', [[], ['k' => 1]], static fn () => array_key_exists($illegal, ['k' => 1])],
            'in_array non-array' => ['inArrayStops', [1, null], static fn () => in_array(1, null)],
            'key exists non-array' => ['keyExistsStops', ['k', null], static fn () => array_key_exists('k', null)],
        ];
    }

    /**
     * @dataProvider stopProvider
     */
    public function testExecutionStopsAtAThrowingArrayOperation(string $method, array $arguments, callable $php): void
    {
        $this->assertMatchesPhp(fn () => $this->subject->{$method}(...$arguments), $php);
        $this->assertFalse($this->subject->reached, 'The method went on after the operation threw.');
    }

    public function testExecutionStopsWhenKeyExistsThrows(): void
    {
        try {
            $this->subject->keyExistsStops([], ['k' => 1]);
            $this->fail('array_key_exists() must throw for an illegal key.');
        } catch (TypeError) {
        }

        $this->assertFalse($this->subject->reached);
    }

    /**
     * Holds a key for every conversion target, the resource's id included.
     */
    private static function base(): array
    {
        self::$resource ??= fopen('php://memory', 'r');

        return ['' => 'e', 'k' => 'k', 0 => 'z', 1 => 'o', 2 => 't', (int) self::$resource => 'r'];
    }
}
