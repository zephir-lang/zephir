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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;
use Zephir\Exception\CompilerException;

/**
 * Locks the MINIT-time C emitted for PHP attributes (issue #2466).
 *
 * Every target is attached from ZEPHIR_INIT_CLASS rather than from a static
 * table, because the arginfo array and the ZEPHIR_INIT_FUNCS method entry in
 * the generated header have no attribute slot: a method attribute lives on the
 * `zend_function` and a parameter attribute on `func->common.attributes` at
 * `offset + 1`, neither of which exists until the register macro has run.
 *
 * Argument values are folded here, at compile time, because the engine stores
 * an internal attribute's arguments as zvals. PHP instead keeps a constant AST
 * and evaluates it at reflection time, so anything Zephir cannot fold is a
 * compile error rather than a deferred one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
final class AttributeCodeGenTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_attributes_test_', ['stub/attributes']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    private function compileDemo(string $zep): string
    {
        $this->compileSource('Stub\\Attributes\\Demo', 'stub/attributes/demo.zep', $zep);

        return $this->generatedC('stub/attributes/demo.zep');
    }

    public function testClassAttributeIsAttachedAfterTheRegisterMacro(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[Marker]
class Demo
{
}
ZEP);

        // No arguments means no handle to keep, so no block and no unused local.
        $this->assertStringContainsString(
            'zephir_add_class_attribute(stub_attributes_demo_ce, SL("Stub\\\\Attributes\\\\Marker"), 0);',
            $c
        );

        $register = strpos($c, 'ZEPHIR_REGISTER_CLASS(');
        $attach   = strpos($c, 'zephir_add_class_attribute(');
        $this->assertNotFalse($register);
        $this->assertNotFalse($attach);
        $this->assertGreaterThan($register, $attach, 'the class entry must exist before an attribute is attached');
    }

    public function testAttributeNameIsResolvedThroughNamespaceAndAliases(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

use Other\Space\Tag as T;

#[T]
#[\Attribute]
#[Sub\Local]
class Demo
{
}
ZEP);

        $this->assertStringContainsString('SL("Other\\\\Space\\\\Tag")', $c);
        $this->assertStringContainsString('SL("Attribute")', $c);
        $this->assertStringContainsString('SL("Stub\\\\Attributes\\\\Sub\\\\Local")', $c);
    }

    public function testPositionalAndNamedArgumentsAreWrittenIntoTheHandle(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[Marker("text", 7, 1.5, true, null, flag: false)]
class Demo
{
}
ZEP);

        $this->assertStringContainsString(
            'zend_attribute *_za = zephir_add_class_attribute(stub_attributes_demo_ce, SL("Stub\\\\Attributes\\\\Marker"), 6);',
            $c
        );
        // One zval per argument, numbered across the whole attribute, so the
        // block stays flat and each argument is addressable.
        $this->assertStringContainsString('ZVAL_STRINGL(&_zc0, "text", sizeof("text") - 1);', $c);
        $this->assertStringContainsString('zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);', $c);
        $this->assertStringContainsString('ZVAL_LONG(&_zc1, 7);', $c);
        $this->assertStringContainsString('ZVAL_DOUBLE(&_zc2, 1.5);', $c);
        $this->assertStringContainsString('ZVAL_BOOL(&_zc3, 1);', $c);
        $this->assertStringContainsString('ZVAL_NULL(&_zc4);', $c);
        $this->assertStringContainsString('ZVAL_BOOL(&_zc5, 0);', $c);
        $this->assertStringContainsString('zephir_attribute_set_arg(_za, 5, SL("flag"), &_zc5);', $c);
    }

    public function testArrayArgumentIsBuiltAsAConstantArray(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[Marker([1, 2, "k": "v"], [])]
class Demo
{
}
ZEP);

        $this->assertStringContainsString('array_init_size(&_zc0, 4);', $c);
        $this->assertStringContainsString('add_next_index_long(&_zc0, 1);', $c);
        $this->assertStringContainsString('add_assoc_stringl_ex(&_zc0, SL("k"), SL("v"));', $c);
        $this->assertStringContainsString('zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);', $c);
        // An empty array reuses buildConstantArray(), which always sizes for
        // count + 1, so `[]` is array_init_size(..., 1) rather than array_init().
        $this->assertStringContainsString('array_init_size(&_zc1, 1);', $c);
        $this->assertStringContainsString('zephir_attribute_set_arg(_za, 1, NULL, 0, &_zc1);', $c);
    }

    /**
     * PHP defers an attribute argument to reflection time, so a constant
     * expression is legal there. Zephir has to produce a zval at MINIT, so the
     * same expressions are folded now.
     */
    public function testConstantExpressionArgumentsAreFolded(): void
    {
        // `self::` resolves off the compilation context, so it works in this
        // isolated harness; a cross-class `Other::CONST` needs the compiler's
        // class registry and is covered by the extension suite.
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[Marker(2 * 3, -0x7f - 1, "a" . "b", self::VERSION, self::class)]
class Demo
{
    const VERSION = "1.0";
}
ZEP);

        $this->assertStringContainsString('ZVAL_LONG(&_zc0, 6);', $c);
        $this->assertStringContainsString('ZVAL_LONG(&_zc1, -128);', $c);
        $this->assertStringContainsString('ZVAL_STRINGL(&_zc2, "ab", sizeof("ab") - 1);', $c);
        $this->assertStringContainsString('ZVAL_STRINGL(&_zc3, "1.0", sizeof("1.0") - 1);', $c);
        $this->assertStringContainsString(
            'ZVAL_STRINGL(&_zc4, "Stub\\\\Attributes\\\\Demo", sizeof("Stub\\\\Attributes\\\\Demo") - 1);',
            $c
        );
    }

    /**
     * The engine interns a function-table key lowercased, while Zephir keeps
     * the source spelling in PHP_ME, so the lookup name has to be lowered.
     */
    public function testMethodAttributeLooksTheMethodUpByItsLowercasedName(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    #[Marker(1)]
    public function getValue() -> int
    {
        return 1;
    }
}
ZEP);

        $this->assertStringContainsString(
            'zephir_add_method_attribute(stub_attributes_demo_ce, SL("getvalue"), SL("Stub\\\\Attributes\\\\Marker"), 1);',
            $c
        );
    }

    public function testParameterAttributeCarriesTheDeclaredOffset(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    public function run(a, #[Marker] string b, #[Marker("last")] ...rest)
    {
    }
}
ZEP);

        $this->assertStringContainsString(
            'zephir_add_parameter_attribute(stub_attributes_demo_ce, SL("run"), 1, SL("Stub\\\\Attributes\\\\Marker"), 0);',
            $c
        );
        $this->assertStringContainsString(
            'zephir_add_parameter_attribute(stub_attributes_demo_ce, SL("run"), 2, SL("Stub\\\\Attributes\\\\Marker"), 1);',
            $c
        );
    }

    /**
     * A property attribute needs the zend_property_info, so it can only be
     * attached after the declare. Both the typed and the untyped declare paths
     * are covered because they emit through different helpers.
     */
    public function testPropertyAttributeFollowsItsDeclare(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    #[Marker]
    public int typed = 0;

    #[Marker]
    protected untyped;
}
ZEP);

        $typedDeclare = strpos($c, 'zephir_declare_typed_property(stub_attributes_demo_ce, SL("typed")');
        $typedAttach  = strpos($c, 'zephir_add_property_attribute(stub_attributes_demo_ce, SL("typed")');
        $plainDeclare = strpos($c, 'zend_declare_property_null(stub_attributes_demo_ce, SL("untyped")');
        $plainAttach  = strpos($c, 'zephir_add_property_attribute(stub_attributes_demo_ce, SL("untyped")');

        $this->assertNotFalse($typedAttach);
        $this->assertNotFalse($plainAttach);
        $this->assertGreaterThan($typedDeclare, $typedAttach);
        $this->assertGreaterThan($plainDeclare, $plainAttach);
    }

    public function testClassConstantAttributeFollowsItsDeclare(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    #[Marker]
    const VERSION = "1.0";
}
ZEP);

        $declare = strpos($c, 'zephir_declare_class_constant_string(stub_attributes_demo_ce, SL("VERSION")');
        $attach  = strpos($c, 'zephir_add_class_constant_attribute(stub_attributes_demo_ce, SL("VERSION"), SL("Stub\\\\Attributes\\\\Marker"), 0);');

        $this->assertNotFalse($attach);
        $this->assertGreaterThan($declare, $attach);
    }

    /**
     * Method attributes must precede zend_class_implements(), because the
     * engine's interface-implementation check reads the child's attribute table
     * looking for `returntypewillchange`.
     */
    public function testMethodAttributesArePlacedBeforeInterfacesAreBound(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo implements \Countable
{
    #[\ReturnTypeWillChange]
    public function count()
    {
        return 0;
    }
}
ZEP);

        $attach     = strpos($c, 'zephir_add_method_attribute(');
        $implements = strpos($c, 'zend_class_implements(');

        $this->assertNotFalse($attach);
        $this->assertNotFalse($implements);
        $this->assertLessThan($implements, $attach);
    }

    public function testInterfaceMembersCarryAttributes(): void
    {
        $this->compileSource('Stub\\Attributes\\Contract', 'stub/attributes/contract.zep', <<<'ZEP'
namespace Stub\Attributes;

#[Marker]
interface Contract
{
    #[Marker]
    const LIMIT = 10;

    #[Marker]
    public function handle(#[Marker] string secret);
}
ZEP);
        $c = $this->generatedC('stub/attributes/contract.zep');

        $this->assertStringContainsString('zephir_add_class_attribute(stub_attributes_contract_ce,', $c);
        $this->assertStringContainsString('zephir_add_class_constant_attribute(stub_attributes_contract_ce, SL("LIMIT"),', $c);
        $this->assertStringContainsString('zephir_add_method_attribute(stub_attributes_contract_ce, SL("handle"),', $c);
        $this->assertStringContainsString('zephir_add_parameter_attribute(stub_attributes_contract_ce, SL("handle"), 0,', $c);
    }

    /**
     * Four of PHP's attributes get their behaviour from a validator the engine
     * only runs while compiling userland code, so a class registered at MINIT
     * needs the matching ZEND_ACC_* flag set as well. The flag macro is 0 on
     * the versions where the attribute does not exist, which keeps the emitted
     * C the same everywhere.
     */
    public function testEngineAttributesCoEmitTheirFlag(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[\AllowDynamicProperties]
class Demo
{
    #[\Deprecated("gone")]
    const OLD = 1;

    #[\Deprecated]
    #[\NoDiscard]
    public function legacy() -> int
    {
        return 1;
    }
}
ZEP);

        $this->assertStringContainsString(
            'zephir_mark_class_flags(stub_attributes_demo_ce, ZEPHIR_ATTR_ALLOW_DYNAMIC_PROPERTIES);',
            $c
        );
        $this->assertStringContainsString(
            'zephir_mark_method_flags(stub_attributes_demo_ce, SL("legacy"), ZEPHIR_ATTR_DEPRECATED);',
            $c
        );
        $this->assertStringContainsString(
            'zephir_mark_method_flags(stub_attributes_demo_ce, SL("legacy"), ZEPHIR_ATTR_NODISCARD);',
            $c
        );
        $this->assertStringContainsString(
            'zephir_mark_class_constant_flags(stub_attributes_demo_ce, SL("OLD"), ZEPHIR_ATTR_DEPRECATED);',
            $c
        );
    }

    /**
     * These live in the global namespace, so a same-named attribute inside a
     * namespace is a different class and must not get the flag.
     */
    public function testNamespacedLookalikeDoesNotCoEmitAFlag(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

#[AllowDynamicProperties]
class Demo
{
}
ZEP);

        $this->assertStringContainsString('SL("Stub\\\\Attributes\\\\AllowDynamicProperties")', $c);
        $this->assertStringNotContainsString('zephir_mark_class_flags', $c);
    }

    public function testOverrideWithNothingToOverrideIsRejected(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage(
            'Stub\Attributes\Demo::lonely() has #[\Override] attribute, but no matching parent method exists'
        );

        $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    #[\Override]
    public function lonely()
    {
        return 1;
    }
}
ZEP);
    }

    public function testOverrideOfAnInterfaceMethodIsAccepted(): void
    {
        $c = $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo implements \Countable
{
    #[\Override]
    public function count()
    {
        return 0;
    }
}
ZEP);

        $this->assertStringContainsString('SL("Override")', $c);
    }

    public function testNoDiscardOnAVoidMethodIsRejected(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('#[\NoDiscard] requires a return value');

        $this->compileDemo(<<<'ZEP'
namespace Stub\Attributes;

class Demo
{
    #[\NoDiscard]
    public function nothing() -> void
    {
    }
}
ZEP);
    }

    /** @return list<array{0: string, 1: string}> */
    public static function rejectedArgumentProvider(): array
    {
        return [
            'object instantiation' => ['new Marker()', 'is not a compile-time constant expression'],
            'method call'          => ['this->run()', 'is not a compile-time constant expression'],
            'char literal'         => ["'a'", 'char is a byte value'],
            'late static binding'  => ['static::class', 'late static binding'],
        ];
    }

    /**
     * @dataProvider rejectedArgumentProvider
     */
    public function testNonConstantArgumentIsRejected(string $argument, string $expected): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches('/' . preg_quote($expected, '/') . '/');

        $this->compileDemo(<<<ZEP
namespace Stub\\Attributes;

#[Marker({$argument})]
class Demo
{
    public function run()
    {
    }
}
ZEP);
    }
}
