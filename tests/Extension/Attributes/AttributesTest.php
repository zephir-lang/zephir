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

namespace Extension\Attributes;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use ReflectionMethod;
use ReflectionProperty;
use Stub\Attributes\Contract;
use Stub\Attributes\Counter;
use Stub\Attributes\CounterUser;
use Stub\Attributes\Demo;
use Stub\Attributes\Marker;

/**
 * PHP attributes on a compiled Zephir class, checked through the same
 * Reflection API a hand-written PHP class would be (issue #2466).
 *
 * A Zephir class is registered at MINIT, so its attributes are attached with
 * `zend_add_*_attribute()` and their arguments are stored as already-folded
 * zvals — the same way php-src declares the attributes of its own internal
 * classes. What that has to buy is exactly this: getAttributes(),
 * getArguments() and newInstance() behaving as they do for userland code.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
final class AttributesTest extends TestCase
{
    /**
     * @param \ReflectionAttribute[] $attributes
     *
     * @return list<string>
     */
    private function names(array $attributes): array
    {
        return array_map(static fn ($attribute): string => $attribute->getName(), $attributes);
    }

    public function testClassCarriesItsAttributesInSourceOrder(): void
    {
        $attributes = (new ReflectionClass(Demo::class))->getAttributes();

        $this->assertSame([Marker::class, Marker::class], $this->names($attributes));
        $this->assertSame(['class-level', 'flag' => true], $attributes[0]->getArguments());
        $this->assertSame(['second'], $attributes[1]->getArguments());
    }

    /**
     * A repeated attribute is accepted because the repetition check
     * ReflectionAttribute::newInstance() performs is user-class-only: an
     * internal attribute is taken as already validated.
     */
    public function testRepeatedAttributeIsReadable(): void
    {
        $attributes = (new ReflectionClass(Demo::class))->getAttributes(Marker::class);

        $this->assertCount(2, $attributes);
        $this->assertSame('second', $attributes[1]->newInstance()->text);
    }

    public function testClassConstantCarriesItsAttribute(): void
    {
        $constant = (new ReflectionClass(Demo::class))->getReflectionConstant('VERSION');

        $this->assertSame([Marker::class], $this->names($constant->getAttributes()));
        $this->assertSame(['on-a-constant'], $constant->getAttributes()[0]->getArguments());
    }

    /**
     * The typed and untyped property declares go through different kernel
     * helpers, so both are checked.
     */
    public function testTypedAndUntypedPropertiesCarryTheirAttributes(): void
    {
        $typed   = new ReflectionProperty(Demo::class, 'counter');
        $untyped = new ReflectionProperty(Demo::class, 'legacy');

        $this->assertSame([Marker::class], $this->names($typed->getAttributes()));
        $this->assertSame([Marker::class], $this->names($untyped->getAttributes()));

        // `self::VERSION` is folded at compile time, as PHP cannot be asked to
        // resolve it later from a zval.
        $this->assertSame(['1.0'], $untyped->getAttributes()[0]->getArguments());
    }

    public function testMethodCarriesItsAttributeAndAnUnattributedOneDoesNot(): void
    {
        $this->assertSame(
            [Marker::class],
            $this->names((new ReflectionMethod(Demo::class, 'getValue'))->getAttributes())
        );
        $this->assertSame([], (new ReflectionMethod(Demo::class, 'plain'))->getAttributes());
    }

    /**
     * A parameter attribute is stored on the function at `offset + 1`, so the
     * offsets have to line up with the declared positions.
     */
    public function testParameterAttributesMatchTheirDeclaredOffsets(): void
    {
        $parameters = (new ReflectionMethod(Demo::class, 'getValue'))->getParameters();

        $this->assertSame([Marker::class], $this->names($parameters[0]->getAttributes()));
        $this->assertSame(['first'], $parameters[0]->getAttributes()[0]->getArguments());
        $this->assertSame(['SensitiveParameter'], $this->names($parameters[1]->getAttributes()));
    }

    public function testInterfaceAndItsMembersCarryAttributes(): void
    {
        $interface = new ReflectionClass(Contract::class);

        $this->assertSame([Marker::class], $this->names($interface->getAttributes()));
        $this->assertSame(
            [Marker::class],
            $this->names($interface->getReflectionConstant('LIMIT')->getAttributes())
        );
        $this->assertSame([Marker::class], $this->names($interface->getMethod('handle')->getAttributes()));
        $this->assertSame(
            [Marker::class],
            $this->names($interface->getMethod('handle')->getParameters()[0]->getAttributes())
        );
    }

    /**
     * Zephir merges traits at compile time, re-instantiating each member from
     * the same raw AST node, so the using class carries the trait member's
     * attributes — which is what PHP does natively.
     */
    public function testTraitMemberAttributesReachTheUsingClass(): void
    {
        $trait = new ReflectionClass(Counter::class);
        $user  = new ReflectionClass(CounterUser::class);

        $this->assertSame([Marker::class], $this->names($trait->getAttributes()));
        $this->assertSame(['trait-property'], $trait->getProperty('hits')->getAttributes()[0]->getArguments());

        $this->assertSame(['trait-property'], $user->getProperty('hits')->getAttributes()[0]->getArguments());
        $this->assertSame(['trait-method'], $user->getMethod('bump')->getAttributes()[0]->getArguments());
    }

    /**
     * The whole point of storing the arguments as zvals: the engine can still
     * construct the attribute object from them.
     */
    public function testNewInstanceConstructsTheAttributeObject(): void
    {
        $instance = (new ReflectionClass(Demo::class))->getAttributes()[0]->newInstance();

        $this->assertInstanceOf(Marker::class, $instance);
        $this->assertSame('class-level', $instance->text);
        $this->assertTrue($instance->flag);
    }

    /**
     * A named argument that skips an earlier parameter needs that parameter's
     * default in arg_info, which only a typed parameter carries.
     */
    public function testNewInstanceHonoursNamedArgumentsAndFoldedExpressions(): void
    {
        $instance = (new ReflectionProperty(Demo::class, 'counter'))->getAttributes()[0]->newInstance();

        $this->assertSame('on-a-property', $instance->text);
        $this->assertFalse($instance->flag);
        $this->assertSame([1, 'k' => 'v'], $instance->extra);
        $this->assertSame(6, $instance->number, '`2 * 3` is folded at compile time');
    }

    /**
     * A Zephir class can itself be an attribute class: `newInstance()` looks
     * the `#[\Attribute]` marker up on the class entry, and an internal class
     * carries it exactly as a userland one does.
     */
    public function testZephirClassIsUsableAsAnAttributeClass(): void
    {
        $marker = (new ReflectionClass(Marker::class))->getAttributes();

        $this->assertSame(['Attribute'], $this->names($marker));
        $this->assertSame(
            \Attribute::TARGET_ALL | \Attribute::IS_REPEATABLE,
            $marker[0]->newInstance()->flags,
            'the internal class constants are folded against the PHP that built the extension'
        );
    }

    /**
     * A top-level function has no class initializer of its own, so its
     * attributes are attached from MINIT next to the class inits. The engine
     * registers a module's functions before MINIT runs, so the function table
     * is already populated by then.
     */
    public function testFunctionAndItsParameterCarryAttributes(): void
    {
        $function = new \ReflectionFunction('Stub\\Attributes\\tagged');

        $this->assertSame([Marker::class], $this->names($function->getAttributes()));
        $this->assertSame(
            ['on-a-function', 'number' => 7],
            $function->getAttributes()[0]->getArguments(),
            '`4 + 3` is folded at compile time'
        );
        $this->assertSame(
            [Marker::class],
            $this->names($function->getParameters()[0]->getAttributes())
        );
        $this->assertSame(
            'on-a-function-parameter',
            $function->getParameters()[0]->getAttributes()[0]->newInstance()->text
        );
    }

    public function testFilteringByAttributeNameWorks(): void
    {
        $class = new ReflectionClass(Demo::class);

        $this->assertCount(2, $class->getAttributes(Marker::class));
        $this->assertCount(0, $class->getAttributes(\Deprecated::class));
    }
}
