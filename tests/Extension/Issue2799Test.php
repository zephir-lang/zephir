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
use ReflectionClass;
use ReflectionProperty;
use Stub\Issue2799;
use Stub\Issue2799ArrayObject;
use Stub\Issue2799Child;
use Stub\Issue2799Exception;
use Stub\Issue2799Multiline;

use function array_intersect_key;
use function array_shift;
use function count;
use function get_class_vars;

/**
 * An array property default is the class's, as in PHP: Reflection reports it,
 * a PHP subclass that redeclares the property keeps its own default, an
 * internal parent still creates the object, and every object writes to its
 * own copy. The PHP classes at the end of this file are the oracle.
 *
 * An array that reads a constant (`withConst`) is still built per object, so
 * its Reflection default stays null; only its values are compared.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
final class Issue2799Test extends TestCase
{
    use AssertsPhpParity;

    private const DECLARED = ['items' => 1, 'literal' => 1, 'typed' => 1, 'scalar' => 1];

    public function testReflectionReportsTheDefaultsAsPhpDoes(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::reflectedDefaults(Issue2799::class),
            static fn () => self::reflectedDefaults(Issue2799Php::class)
        );
    }

    public function testClassVarsReportTheDefaultsAsPhpDoes(): void
    {
        $this->assertMatchesPhp(
            static fn () => array_intersect_key(get_class_vars(Issue2799::class), self::DECLARED),
            static fn () => array_intersect_key(get_class_vars(Issue2799Php::class), self::DECLARED)
        );
    }

    public function testNewObjectStartsFromTheDefaults(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::state(new Issue2799()),
            static fn () => self::state(new Issue2799Php())
        );
    }

    public function testWritesStayInTheirObject(): void
    {
        $this->assertMatchesPhp(
            static fn () => [self::exercise(new Issue2799()), self::state(new Issue2799())],
            static fn () => [self::exercise(new Issue2799Php()), self::state(new Issue2799Php())]
        );
    }

    public function testUserlandWritesStayInTheirObject(): void
    {
        $this->assertMatchesPhp(
            static fn () => [self::writeFromOutside(new Issue2799()), self::state(new Issue2799())],
            static fn () => [self::writeFromOutside(new Issue2799Php()), self::state(new Issue2799Php())]
        );
    }

    public function testCloneWritesToItsOwnCopy(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::cloneAndWrite(new Issue2799()),
            static fn () => self::cloneAndWrite(new Issue2799Php())
        );
    }

    public function testEmptyArrayClassConstant(): void
    {
        $this->assertMatchesPhp(
            static fn () => Issue2799::EMPTY_LIST,
            static fn () => Issue2799Php::EMPTY_LIST
        );
    }

    public function testPhpSubclassKeepsTheDefaultItRedeclares(): void
    {
        $this->assertMatchesPhp(
            static fn () => [
                self::state(new Issue2799RedeclaresNull()),
                self::state(new Issue2799RedeclaresArray()),
                self::reflectedDefaults(Issue2799RedeclaresNull::class),
            ],
            static fn () => [
                self::state(new Issue2799PhpRedeclaresNull()),
                self::state(new Issue2799PhpRedeclaresArray()),
                self::reflectedDefaults(Issue2799PhpRedeclaresNull::class),
            ]
        );
    }

    public function testPhpSubclassInheritsTheDefaults(): void
    {
        $this->assertMatchesPhp(
            static fn () => [self::state(new Issue2799Inherits()), self::exercise(new Issue2799Inherits())],
            static fn () => [self::state(new Issue2799PhpInherits()), self::exercise(new Issue2799PhpInherits())]
        );
    }

    public function testZephirSubclassRedeclaresAndAddsDefaults(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::childState(new Issue2799Child()),
            static fn () => self::childState(new Issue2799PhpChild())
        );
    }

    public function testExceptionRecordsWhereItWasCreated(): void
    {
        $line      = __LINE__ + 1;
        $exception = new Issue2799Exception('message', 7);

        $this->assertSame(
            [__FILE__, $line, 'message', 7, [], [1, 2]],
            [
                $exception->getFile(),
                $exception->getLine(),
                $exception->getMessage(),
                $exception->getCode(),
                $exception->getErrors(),
                $exception->getCodes(),
            ]
        );
    }

    /**
     * A string spanning lines in a constant, a property default or a
     * parameter default compiles, and keeps its line breaks as PHP does.
     */
    public function testDeclaredStringsKeepTheirLineBreaks(): void
    {
        $this->assertMatchesPhp(
            static fn () => self::multiline(Issue2799Multiline::class),
            static fn () => self::multiline(Issue2799PhpMultiline::class)
        );
    }

    public function testArrayObjectHasItsStorage(): void
    {
        $object   = new Issue2799ArrayObject([1, 2]);
        $object[] = 3;

        $this->assertSame([3, [1, 2, 3], []], [count($object), $object->getArrayCopy(), $object->getExtra()]);
    }

    /**
     * @return array<string, array{0: bool, 1: mixed}>
     */
    private static function reflectedDefaults(string $class): array
    {
        $defaults = (new ReflectionClass($class))->getDefaultProperties();
        $result   = [];

        foreach (self::DECLARED as $name => $_) {
            $property      = new ReflectionProperty($class, $name);
            $result[$name] = [$property->hasDefaultValue(), $property->getDefaultValue(), $defaults[$name] ?? 'missing'];
        }

        return $result;
    }

    private static function state(object $object): array
    {
        return [$object->getItems(), $object->getLiteral(), $object->getWithConst(), $object->typed, $object->scalar];
    }

    private static function childState(object $object): array
    {
        return [
            self::state($object),
            $object->extra,
            (new ReflectionProperty($object, 'literal'))->getDefaultValue(),
            (new ReflectionProperty($object, 'extra'))->getDefaultValue(),
        ];
    }

    /**
     * Every write kind the compiled code has: append, key, unset of a present
     * and a missing key, a nested append (also into a nested `[]`), a compound
     * assignment, a by-reference argument and a write to the constant-bearing
     * and the typed array.
     */
    private static function exercise(object $object): array
    {
        $object->append(1);
        $object->setItem('k', 2);
        $object->unsetItem(0);
        $object->unsetItem('missing');
        $object->setKey('s', 'changed');
        $object->appendNested(4);
        $object->appendNestedEmpty(5);
        $object->unsetKey('b');
        $object->incrementF();
        $shifted = $object->shift();
        $object->appendConst('c');
        $object->appendTyped('t');

        return [$shifted, self::state($object)];
    }

    private static function writeFromOutside(object $object): array
    {
        $object->items[]       = 'u';
        $object->withConst[]   = 'u';
        $object->typed['k']    = 'u';
        $literal               = $object->getLiteral();
        $literal['a'][]        = 9;
        $literal['e'][]        = 9;
        $items                 = $object->getItems();
        $items[]               = 'copy';
        array_shift($literal);

        return [self::state($object), $literal, $items];
    }

    private static function multiline(string $class): array
    {
        $reflection = new ReflectionClass($class);

        return [
            $reflection->getConstants(),
            $reflection->getDefaultProperties(),
            $class::$shared,
            (new $class())->withDefault(),
        ];
    }

    private static function cloneAndWrite(object $object): array
    {
        $copy = clone $object;
        $copy->append(1);
        $copy->appendNested(2);
        $object->setKey('s', 'original');

        return [self::state($object), self::state($copy)];
    }
}

class Issue2799Php
{
    public const K = 'k';

    public const EMPTY_LIST = [];

    public $items = [];

    protected $literal = [1, 'a' => [2, 3], 'e' => [], 's' => 'str', 'n' => null, 'f' => 1.5, 'b' => true];

    public $withConst = [PHP_INT_SIZE, self::K];

    public array $typed = [];

    public $scalar = 5;

    public function getItems()
    {
        return $this->items;
    }

    public function getLiteral()
    {
        return $this->literal;
    }

    public function getWithConst()
    {
        return $this->withConst;
    }

    public function append($v): void
    {
        $this->items[] = $v;
    }

    public function setItem($k, $v): void
    {
        $this->items[$k] = $v;
    }

    public function unsetItem($k): void
    {
        unset($this->items[$k]);
    }

    public function setKey($k, $v): void
    {
        $this->literal[$k] = $v;
    }

    public function appendNested($v): void
    {
        $this->literal['a'][] = $v;
    }

    public function appendNestedEmpty($v): void
    {
        $this->literal['e'][] = $v;
    }

    public function unsetKey($k): void
    {
        unset($this->literal[$k]);
    }

    public function incrementF(): void
    {
        $this->literal['f'] += 1;
    }

    public function shift()
    {
        return array_shift($this->literal);
    }

    public function appendConst($v): void
    {
        $this->withConst[] = $v;
    }

    public function appendTyped($v): void
    {
        $this->typed[] = $v;
    }
}

class Issue2799PhpChild extends Issue2799Php
{
    protected $literal = ['child'];

    public $extra = ['x' => []];
}

class Issue2799RedeclaresNull extends Issue2799
{
    public $items = null;

    protected $literal = null;
}

class Issue2799PhpRedeclaresNull extends Issue2799Php
{
    public $items = null;

    protected $literal = null;
}

class Issue2799RedeclaresArray extends Issue2799
{
    public $items = [9];
}

class Issue2799PhpRedeclaresArray extends Issue2799Php
{
    public $items = [9];
}

class Issue2799Inherits extends Issue2799
{
}

class Issue2799PhpInherits extends Issue2799Php
{
}

class Issue2799PhpMultiline
{
    public const TEXT = "c1
c2";

    public const LINES = ["l1
l2", "k1
k2" => "v1
v2"];

    public static $shared = "s1
s2";

    public $template = ["main" => "
<nav>
    <ol>
%items%
    </ol>
</nav>", "k1
k2" => 1];

    public $text = "t1
t2";

    public string $typedText = "y1
y2";

    public function withDefault(string $x = "p1
p2"): string
    {
        return $x;
    }
}
