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
 * `unset this->prop[offset]` must emit one kernel call, and its offset must be
 * a zval the memory manager owns.
 *
 * Two defects met in the generated C. The offset temp was taken from a
 * read-only property fetch and then overwritten with ZVAL_STRING, so nothing
 * released the zend_string it came to own; and the property branch fell
 * through into the generic array-access branch, so the unset was emitted a
 * second time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2702
 */
final class UnsetPropertyArrayTest extends TestCase
{
    use CompilesZephirSource;

    private ?string $generated = null;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_unset_property_array_test_', ['stub/issue2702']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * The offset must be boxed into a tracked temp, the same shape the write
     * counterpart `let this->prop["k"] = v` already emits.
     */
    public function testStringLiteralOffsetIsBoxedIntoATrackedTemp(): void
    {
        $body = $this->bodyOf('unsetStringLiteral');

        $this->assertMatchesRegularExpression(
            '/ZEPHIR_INIT_[N]?VAR\(&(\w+)\);\s*\n\s*ZVAL_STRING\(&\1, "key_a"\);/',
            $body,
            "The literal offset must be assigned into a memory manager owned temp.\n$body"
        );

        $this->assertStringNotContainsString(
            'zephir_read_property',
            $this->upToTheUnsetCall($body),
            "No property read may precede the unset: the old code took its zval slot from one.\n$body"
        );
    }

    public function testIntLiteralOffsetIsBoxedIntoATrackedTemp(): void
    {
        $body = $this->bodyOf('unsetIntLiteral');

        $this->assertMatchesRegularExpression(
            '/ZEPHIR_INIT_[N]?VAR\(&(\w+)\);\s*\n\s*ZVAL_LONG\(&\1, 3\);/',
            $body,
            "The literal offset must be assigned into a memory manager owned temp.\n$body"
        );
    }

    /**
     * A native `int` local is not a zval. `getVariableCode()` hands back its
     * bare name, so passing it straight to a `zval *` parameter does not
     * compile; it has to be boxed like a literal.
     */
    public function testNativeIntOffsetIsBoxedIntoAZval(): void
    {
        $body = $this->bodyOf('unsetIntOffset');

        $this->assertMatchesRegularExpression(
            '/ZEPHIR_INIT_[N]?VAR\(&(\w+)\);\s*\n\s*ZVAL_LONG\(&\1, key\);/',
            $body,
            "A native int offset must be boxed before it reaches a zval * parameter.\n$body"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_unset_property_array\([^;]*, &\w+\);/',
            $body,
            "The offset argument must be a zval pointer, not a bare zend_long.\n$body"
        );
    }

    /**
     * A `var` offset is already a zval and must be passed through untouched.
     */
    public function testVariableOffsetIsPassedThrough(): void
    {
        $body = $this->bodyOf('unsetVarOffset');

        $this->assertStringContainsString(
            'zephir_unset_property_array(this_ptr, ZEND_STRL("data"), key);',
            $body,
            "A var offset needs no boxing.\n$body"
        );
    }

    /**
     * @dataProvider offsetKinds
     */
    public function testTheUnsetIsEmittedExactlyOnce(string $method): void
    {
        $body = $this->bodyOf($method);

        $this->assertSame(
            1,
            substr_count($body, 'zephir_unset_property_array('),
            "Expected exactly one property unset call.\n$body"
        );

        $this->assertStringNotContainsString(
            'zephir_array_unset',
            $body,
            "The generic array-access unset must not be emitted for a property.\n$body"
        );
    }

    public static function offsetKinds(): array
    {
        return [
            'string literal' => ['unsetStringLiteral'],
            'int literal'    => ['unsetIntLiteral'],
            'var offset'     => ['unsetVarOffset'],
            'int offset'     => ['unsetIntOffset'],
        ];
    }

    /**
     * An offset type neither the property path nor `Backend::arrayUnset()`
     * accepts must stay a compile error. Removing the fall through to the
     * generic branch is what makes this worth asserting: the old code silently
     * passed the property's own value as the offset, and only the branch it
     * fell into raised the error.
     */
    public function testAnUnsupportedOffsetTypeIsACompileError(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches('/as array index without cast/');

        $this->compileSource(
            'Stub\\Issue2702\\BadOffset',
            'stub/issue2702/badoffset.zep',
            <<<'ZEP'
            namespace Stub\Issue2702;

            class BadOffset
            {
                protected data = ["key_a": 1];

                public function unsetCharLiteral() -> void
                {
                    unset this->data['a'];
                }
            }
            ZEP
        );
    }

    private function bodyOf(string $method): string
    {
        if (null === $this->generated) {
            $this->compileSource(
                'Stub\\Issue2702\\Holder',
                'stub/issue2702/holder.zep',
                <<<'ZEP'
                namespace Stub\Issue2702;

                class Holder
                {
                    protected data = ["key_a": 1, 3: "three"];

                    public function unsetStringLiteral() -> void
                    {
                        unset this->data["key_a"];
                    }

                    public function unsetIntLiteral() -> void
                    {
                        unset this->data[3];
                    }

                    public function unsetVarOffset(var key) -> void
                    {
                        unset this->data[key];
                    }

                    public function unsetIntOffset(int key) -> void
                    {
                        unset this->data[key];
                    }
                }
                ZEP
            );

            $this->generated = $this->generatedC('stub/issue2702/holder.zep');
        }

        $body = $this->methodBody($this->generated, 'PHP_METHOD(Stub_Issue2702_Holder, ' . $method . ')');
        $this->assertNotSame('', $body, "Could not locate $method() in the generated C.");

        return $body;
    }

    private function upToTheUnsetCall(string $body): string
    {
        $position = strpos($body, 'zephir_unset_property_array(');

        return false === $position ? $body : substr($body, 0, $position);
    }
}
