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

/**
 * A by-reference call argument is a write context, and PHP's write context runs
 * on the container's storage slot rather than on a copy of it. Reading the
 * property borrowed and then wrapping the borrow in a reference is what lost
 * the write, aliased what had already been handed out, and freed the property's
 * array under it when the callee replaced its argument.
 *
 * The invariant these assertions exist for: a `PH_WRITE` fetch never sits on
 * top of a `PH_READONLY` read. That is what makes the kernel's
 * `SEPARATE_ARRAY()` on the container safe.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
final class WriteContextPropertyTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_write_context_test_', ['stub/issue2691']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * A subscript of a property. The property must be fetched as a slot, so the
     * kernel has a container it is allowed to separate.
     */
    public function testAPropertySubscriptArgumentFetchesThePropertySlot(): string
    {
        $body = $this->compileMethod('PushDim', 'pushdim', <<<'ZEP'
    protected prefixes = [];

    public function write(string key, string value)
    {
        array_push(this->prefixes[key], value);

        return this->prefixes;
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_fetch_property_write(',
            $body,
            "The container of a write context must be fetched as a slot.\n$body"
        );
        $this->assertStringContainsString(
            'PH_WRITE',
            $body,
            "A by-reference subscript argument is fetched in write context.\n$body"
        );
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A write context is never a borrow, container included.\n$body"
        );

        return $body;
    }

    /**
     * The slot is a live pointer into the object, so it is passed un-`&`-ed and
     * must never be registered with the memory frame: releasing the frame would
     * destroy the property.
     *
     * @depends testAPropertySubscriptArgumentFetchesThePropertySlot
     */
    public function testThePropertySlotIsNeverOwnedByTheMemoryFrame(string $body): void
    {
        $this->assertMatchesRegularExpression(
            '/zephir_array_fetch\(&\w+, _\w+, /',
            $body,
            "The slot is a zval *, so the fetch takes it without an ampersand.\n$body"
        );

        preg_match('/(\w+) = zephir_fetch_property_write\(/', $body, $slot);
        $this->assertNotEmpty($slot, "Could not find the slot temp.\n$body");

        $this->assertStringNotContainsString(
            'zephir_memory_observe(' . $slot[1] . ')',
            $body,
            "The memory frame must not own a property slot.\n$body"
        );
    }

    /**
     * The property itself as the by-reference argument. The slot becomes the
     * reference, so `ZEPHIR_MAKE_REF()` on a borrowed copy has to go, and with
     * it the unref: PHP leaves the property a reference.
     */
    public function testABarePropertyArgumentIsReferencedThroughItsSlot(): void
    {
        $body = $this->compileMethod('ShiftProp', 'shiftprop', <<<'ZEP'
    protected rows = [];

    public function write(string key, string value)
    {
        return array_shift(this->rows);
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_fetch_property_write(',
            $body,
            "A property handed to a by-reference parameter is fetched as a slot.\n$body"
        );
        $this->assertStringContainsString(
            'ZEPHIR_MAKE_WRITE_REF(',
            $body,
            "The slot itself becomes the reference the callee writes through.\n$body"
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_MAKE_REF(',
            $body,
            "ZEPHIR_MAKE_REF() moves a borrowed value into a reference that never took it.\n$body"
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_UNREF(',
            $body,
            "PHP leaves the property a reference, and ZVAL_UNREF() would efree it.\n$body"
        );
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A write context is never a borrow.\n$body"
        );
    }

    /**
     * A property named at runtime. PHP reaches it through the same handler, so
     * the only difference here is that the name is a zval.
     */
    public function testADynamicallyNamedPropertyAlsoFetchesItsSlot(): void
    {
        $body = $this->compileMethod('PushDynamic', 'pushdynamic', <<<'ZEP'
    protected prefixes = [];

    public function write(string key, string value)
    {
        array_push(this->{key}[key], value);

        return this->prefixes;
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_fetch_property_write_zval(',
            $body,
            "A runtime property name is fetched as a slot too.\n$body"
        );
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A write context is never a borrow.\n$body"
        );
    }

    private function compileMethod(string $className, string $fileName, string $method): string
    {
        $relPath = "stub/issue2691/$fileName.zep";

        $this->compileSource(
            "Stub\\Issue2691\\$className",
            $relPath,
            "namespace Stub\\Issue2691;\n\nclass $className\n{\n$method\n}\n"
        );

        $c    = $this->generatedC($relPath);
        $body = $this->methodBody($c, "PHP_METHOD(Stub_Issue2691_$className, write)");

        $this->assertNotSame('', $body, "Could not locate write() in the generated C.\n$c");

        return $body;
    }
}
