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

use Extension\Issue2391\PhpAccessors;
use Extension\Issue2391\PhpStaticAccessors;
use PHPUnit\Framework\TestCase;
use ReflectionMethod;
use stdClass;
use Stub\Issue2391\Accessors;
use Stub\Issue2391\Collaborator;
use Stub\Issue2391\DocblockTypes;
use Stub\Issue2391\StaticAccessors;
use Throwable;

/**
 * Property shortcuts stay supported: a getter never returns a value its own
 * return type rejects, a static property shortcut uses the static property,
 * and a PHPDoc `@var` type never breaks the build.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
final class Issue2391Test extends TestCase
{
    /**
     * @dataProvider storedValueProvider
     */
    public function testGetterReturnsOrRejectsTheStoredValueAsInPhp(string $getter, string $property, $value): void
    {
        $this->assertSame(
            $this->outcomeOf(static fn () => (new PhpAccessors())->poke($property, $value)->{$getter}()),
            $this->outcomeOf(static fn () => (new Accessors())->poke($property, $value)->{$getter}()),
        );
    }

    public function storedValueProvider(): array
    {
        return [
            'bool holds true'               => ['getFlag', 'flag', true],
            'bool holds null'               => ['getFlag', 'flag', null],
            'bool holds an array'           => ['getFlag', 'flag', [1]],
            'bool holds an int'             => ['getFlag', 'flag', 1],
            'class holds the class'         => ['getThing', 'thing', new Collaborator()],
            'class holds null'              => ['getThing', 'thing', null],
            'class holds a string'          => ['getThing', 'thing', 'text'],
            'class holds another object'    => ['getThing', 'thing', new stdClass()],
            'object holds an object'        => ['getObj', 'obj', new stdClass()],
            'object holds a string'         => ['getObj', 'obj', 'text'],
            'nullable class holds null'     => ['getMaybe', 'maybe', null],
            'nullable class holds a string' => ['getMaybe', 'maybe', 'text'],
            'nullable class holds stdClass' => ['getMaybe', 'maybe', new stdClass()],
            'int|string holds an int'       => ['getKey', 'key', 1],
            'int|string holds a string'     => ['getKey', 'key', 'a'],
            'int|string holds a float'      => ['getKey', 'key', 1.5],
            'int|string holds null'         => ['getKey', 'key', null],
            'array|false holds an array'    => ['getListOrFalse', 'listOrFalse', [1]],
            'array|false holds false'       => ['getListOrFalse', 'listOrFalse', false],
            'array|false holds true'        => ['getListOrFalse', 'listOrFalse', true],
            'double holds a float'          => ['getRatio', 'ratio', 1.5],
            'double holds an int'           => ['getRatio', 'ratio', 2],
            'double holds a string'         => ['getRatio', 'ratio', '1.5'],
            'string holds null'             => ['getName', 'name', null],
            'hand-written bool holds null'  => ['flagByHand', 'flag', null],
            'hand-written class holds null' => ['thingByHand', 'thing', null],
            'hand-written class holds text' => ['thingByHand', 'thing', 'text'],
            'hand-written ?class holds null' => ['maybeByHand', 'maybe', null],
            'hand-written ?class holds text' => ['maybeByHand', 'maybe', 'text'],
            'hand-written object holds text' => ['objByHand', 'obj', 'text'],
        ];
    }

    /**
     * @dataProvider labelProvider
     */
    public function testToStringReturnsOrRejectsTheStoredValueAsInPhp($value): void
    {
        $this->assertSame(
            $this->outcomeOf(static fn () => (string) (new PhpAccessors())->poke('label', $value)),
            $this->outcomeOf(static fn () => (string) (new Accessors())->poke('label', $value)),
        );
    }

    public function labelProvider(): array
    {
        return [
            'string' => ['zephir'],
            'null'   => [null],
            'int'    => [5],
        ];
    }

    /**
     * @dataProvider getterNameProvider
     */
    public function testGetterReturnTypeMatchesPhp(string $getter): void
    {
        $this->assertSame(
            $this->typeOf(new ReflectionMethod(PhpAccessors::class, $getter), null),
            $this->typeOf(new ReflectionMethod(Accessors::class, $getter), null),
        );
    }

    public function getterNameProvider(): array
    {
        $names = [];
        foreach ($this->storedValueProvider() as [$getter]) {
            $names[$getter] = [$getter];
        }

        return $names;
    }

    public function testStaticShortcutsReadAndWriteTheStaticProperty(): void
    {
        $this->assertSame(
            $this->staticRoundTrip(new PhpStaticAccessors()),
            $this->staticRoundTrip(new StaticAccessors()),
        );
        $this->assertSame(9, StaticAccessors::plainValue());
        $this->assertSame(4, StaticAccessors::countValue());
    }

    public function testStaticShortcutRejectsAWrongTypeAsInPhp(): void
    {
        $this->assertSame(
            $this->outcomeOf(static fn () => (new PhpStaticAccessors())->getFlag()),
            $this->outcomeOf(static fn () => (new StaticAccessors())->getFlag()),
        );
        $this->assertSame(
            $this->outcomeOf(static fn () => (new PhpStaticAccessors())->setCount('text')),
            $this->outcomeOf(static fn () => (new StaticAccessors())->setCount('text')),
        );
    }

    /**
     * @dataProvider docblockTypeProvider
     */
    public function testDocblockTypeTypesTheAccessors(string $property, string $getterType, string $setterType): void
    {
        $suffix = ucfirst($property);

        $this->assertSame(
            [$getterType, $setterType],
            [
                $this->typeOf(new ReflectionMethod(DocblockTypes::class, 'get' . $suffix), null),
                $this->typeOf(new ReflectionMethod(DocblockTypes::class, 'set' . $suffix), 0),
            ]
        );
    }

    public function docblockTypeProvider(): array
    {
        return [
            'float'                => ['floatValue', 'float', 'float'],
            'integer'              => ['integerValue', 'int', 'int'],
            'boolean'              => ['booleanValue', 'bool', 'bool'],
            '?int'                 => ['nullableInt', '?int', '(none)'],
            '?Collaborator'        => ['nullableThing', '?' . Collaborator::class, '(none)'],
            'string[]'             => ['strings', 'array', 'array'],
            'array<int, string>'   => ['generic', 'array', 'array'],
            'Collaborator[]'       => ['things', 'array', 'array'],
            'numeric'              => ['numericValue', '(none)', '(none)'],
            'scalar'               => ['scalarValue', '(none)', '(none)'],
            'self'                 => ['selfValue', '(none)', '(none)'],
            'iterable'             => ['iterableValue', '(none)', '(none)'],
            'void'                 => ['voidValue', '(none)', '(none)'],
            'true'                 => ['trueValue', '(none)', '(none)'],
            'null'                 => ['nullValue', '(none)', '(none)'],
            'numeric|null'         => ['numericOrNull', '(none)', '(none)'],
        ];
    }

    public function testUntypedDocblockAccessorsAcceptAnyValue(): void
    {
        $object = new DocblockTypes();

        $this->assertSame(5, $object->setNumericValue(5)->getNumericValue());
        $this->assertSame('x', $object->setScalarValue('x')->getScalarValue());
        $this->assertSame([1], $object->setSelfValue([1])->getSelfValue());
        $this->assertSame(1.5, $object->setFloatValue(1.5)->getFloatValue());
        $this->assertSame(['a'], $object->setStrings(['a'])->getStrings());
    }

    /**
     * Sets and reads every static shortcut, recording each value and any
     * notice, warning or deprecation raised on the way.
     */
    private function staticRoundTrip(object $object): array
    {
        $raised = [];
        set_error_handler(static function (int $errno, string $message) use (&$raised): bool {
            $raised[] = $message;

            return true;
        });

        try {
            $object->setPlain(5)->setCount(1);
            $trace = [$object->getPlain(), $object->getCount()];
            $object->setPlain(9)->setCount(4);
            $trace[] = $object->getPlain();
            $trace[] = $object->getCount();
            $trace[] = (string) $object;
            $trace[] = get_object_vars($object);
        } finally {
            restore_error_handler();
        }

        return [$trace, $raised];
    }

    /**
     * The getter's return type, or the setter's first parameter type.
     */
    private function typeOf(ReflectionMethod $method, ?int $parameter): string
    {
        $type = null === $parameter
            ? $method->getReturnType()
            : $method->getParameters()[$parameter]->getType();

        return null === $type ? '(none)' : (string) $type;
    }

    /**
     * The returned value, or the error raised, with the class names of the
     * oracle and the extension normalised away.
     */
    private function outcomeOf(callable $call): string
    {
        try {
            $result = $call();
            $outcome = is_object($result) ? 'object ' . get_class($result) : var_export($result, true);
        } catch (Throwable $e) {
            // `, called in <file> on line <n>` is only added for a userland callee.
            $message = preg_replace('/, called in .* on line \\d+$/', '', $e->getMessage());
            $outcome = get_class($e) . ': ' . $message;
        }

        return str_replace(
            [PhpStaticAccessors::class, StaticAccessors::class, PhpAccessors::class, Accessors::class],
            'CLASS',
            $outcome
        );
    }
}
