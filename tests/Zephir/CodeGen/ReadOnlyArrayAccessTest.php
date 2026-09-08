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
 * The read-only flag on a subscript read means "borrowed, the container owns
 * the value". An ArrayAccess object owns nothing after offsetGet() returns, so
 * the flag may only be emitted for a container that provably holds a native
 * array.
 *
 * These assertions run on the generated C, so they hold without building the
 * extension, and the last one is what keeps the fix from being "turn the
 * optimization off everywhere".
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class ReadOnlyArrayAccessTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_readonly_dim_test_', ['stub/issue2682']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * `fetch v, c["k"]` on an untyped container. The throw is what moves the
     * last call line ahead of the read, which is what used to promote the
     * target to read-only.
     */
    public function testFetchFromAnUntypedContainerIsNotReadOnly(): void
    {
        $body = $this->compileMethod('FetchDim', 'fetchdim', <<<'ZEP'
    public function read(container)
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        if fetch value, container["namespace"] {
            return value;
        }

        return null;
    }
ZEP);

        $this->assertStringContainsString('zephir_array_isset_string_fetch(', $body);
        $this->assertMatchesRegularExpression(
            '/zephir_array_isset_string_fetch\([^;]*,\s*0\)/',
            $body,
            "The fetch must ask for an owned value from a container that may be an object.\n$body"
        );
    }

    /**
     * `let x = c["k"]` on an untyped container, kernel/array.c
     * zephir_array_fetch_string().
     */
    public function testReadFromAnUntypedContainerIsNotReadOnly(): void
    {
        $body = $this->compileMethod('Read', 'read', <<<'ZEP'
    public function read(container)
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let value = container["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString('zephir_array_fetch_string(', $body);
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A container that may be an ArrayAccess object must not borrow.\n$body"
        );
    }

    /**
     * The optimization has to survive where it is valid. `table` is a `var`,
     * so a gate on the declared type would lose this; it is provable because
     * every assignment to it is an array literal.
     */
    public function testReadFromAProvenNativeArrayStaysReadOnly(): void
    {
        $body = $this->compileMethod('Native', 'native', <<<'ZEP'
    public function read(int mode)
    {
        var table, value;

        if mode < 0 {
            throw new \RuntimeException("Not reachable, it only moves the last call line");
        }

        let table = ["namespace": "kept"];
        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString(
            'PH_READONLY',
            $body,
            "A container assigned only from an array literal must keep the shortcut.\n$body"
        );
    }

    /**
     * An array parameter is enforced by PHP itself, so it is provable too.
     */
    public function testReadFromAnArrayParameterStaysReadOnly(): void
    {
        $body = $this->compileMethod('Param', 'param', <<<'ZEP'
    public function read(array table, int mode)
    {
        var value;

        if mode < 0 {
            throw new \RuntimeException("Not reachable, it only moves the last call line");
        }

        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString(
            'PH_READONLY',
            $body,
            "An array parameter cannot hold an object, so the shortcut is valid.\n$body"
        );
    }

    /**
     * The hole a declared-type gate would leave: `array` is a compile-time
     * label, and `let table = container` copies whatever the source holds.
     */
    public function testReadFromAnArrayLocalAliasedFromAVarIsNotReadOnly(): void
    {
        $body = $this->compileMethod('Aliased', 'aliased', <<<'ZEP'
    public function read(container)
    {
        array table;
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let table = container;
        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "An array local assigned from a var is not proof of anything.\n$body"
        );
    }

    /**
     * A subscript handed to a by-reference parameter is a write context, never
     * a borrow, whatever the container turns out to be. Only the kernel can
     * tell an array from an ArrayAccess object at that point, so the target is
     * observed either way and the reference wrap is left conditional.
     *
     * ZEPHIR_UNREF() must not appear: for an array container the reference
     * belongs to the container as much as to the argument.
     */
    public function testAByReferenceSubscriptArgumentFetchesInWriteContext(): void
    {
        $body = $this->compileMethod('WriteDim', 'writedim', <<<'ZEP'
    public function read(container, value)
    {
        array_push(container["bucket"], value);

        return container;
    }
ZEP);

        $this->assertStringContainsString(
            'PH_WRITE',
            $body,
            "A by-reference subscript argument must be fetched in write context.\n$body"
        );
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A write context is never a borrow.\n$body"
        );
        $this->assertStringContainsString(
            'zephir_memory_observe(',
            $body,
            "The target owns what it is given on both runtime paths.\n$body"
        );
        $this->assertStringContainsString(
            'ZEPHIR_MAKE_WRITE_REF(',
            $body,
            "The callee needs a reference, and only an ArrayAccess result needs wrapping.\n$body"
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_MAKE_REF(',
            $body,
            "An unconditional wrap would give the callee a reference to a reference.\n$body"
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_UNREF(',
            $body,
            "Unreffing would free a zend_reference the container still points at.\n$body"
        );
    }

    private function compileMethod(string $className, string $fileName, string $method): string
    {
        $relPath = "stub/issue2682/$fileName.zep";

        $this->compileSource(
            "Stub\\Issue2682\\$className",
            $relPath,
            "namespace Stub\\Issue2682;\n\nclass $className\n{\n$method\n}\n"
        );

        $c    = $this->generatedC($relPath);
        $body = $this->methodBody($c, "PHP_METHOD(Stub_Issue2682_$className, read)");

        $this->assertNotSame('', $body, "Could not locate read() in the generated C.\n$c");

        return $body;
    }
}
