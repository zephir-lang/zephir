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
 * `use (&x)` where `x` is a parameter.
 *
 * #2652 made a captured **local** a `zend_reference`; a parameter was rejected
 * because it does not arrive in a shape that can become one - an untyped
 * parameter is a `zval *` borrowed from the caller, a typed one is an unboxed C
 * scalar written by `Z_PARAM_LONG()`.
 *
 * The parameter therefore keeps its native C shape under an internal
 * `<name>_byref` name, and the user-facing symbol becomes a plain zval that the
 * prologue turns into a reference and seeds from that shadow. Everything the
 * parameter needs on the way in - arg-info, the `ZEND_PARSE_PARAMETERS` block,
 * coercion, strict-type checks, default values, the native `zend_string *`
 * strategy - keeps running against the shadow, untouched.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2668
 */
final class ByRefParameterCaptureTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_byref_param_test_', ['stub/issue2668']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * The shape from the issue. The reference is created before the fetch and
     * seeded from the shadow after it, and the carrier stores the reference
     * itself so the closure and the method share one slot.
     */
    public function testAnUntypedParameterIsBoxedIntoAReference(): void
    {
        $body = $this->compileMake('Untyped', 'untyped', <<<'ZEP'
    public function make(var seed) -> <\Closure>
    {
        return function () use (&seed) {
            let seed = seed + 1;

            return seed;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_make_local_reference(&seed);',
            $body,
            "The captured parameter has to become a reference.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY_DEREF(Z_REFVAL_P(&seed), seed_byref);',
            $body,
            "The reference is seeded from the shadow parameter.\n$body"
        );
        $this->assertStringContainsString(
            'zephir_update_property_reference(',
            $body,
            "The carrier stores the reference undereferenced.\n$body"
        );
    }

    /**
     * `zephir_memory_observe()` is what makes `ZEPHIR_MM_RESTORE()` drop the
     * method's reference on return, and it has to run before the reference is
     * created, not after.
     */
    public function testTheReferenceIsRegisteredWithTheMemoryFrameBeforeItIsCreated(): void
    {
        $body = $this->compileMake('Observed', 'observed', <<<'ZEP'
    public function make(var seed) -> <\Closure>
    {
        return function () use (&seed) {
            return seed;
        };
    }
ZEP);

        $this->assertMatchesRegularExpression(
            '/zephir_memory_observe\(&seed\);\s*\n\s*zephir_make_local_reference\(&seed\);/',
            $body,
            "observe() has to precede make_local_reference().\n$body"
        );
    }

    /**
     * The seeding must land after `zephir_fetch_params()`; the shadow holds
     * nothing before it.
     */
    public function testTheSeedingLandsAfterTheParameterFetch(): void
    {
        $body = $this->compileMake('Ordered', 'ordered', <<<'ZEP'
    public function make(var seed) -> <\Closure>
    {
        return function () use (&seed) {
            return seed;
        };
    }
ZEP);

        $fetch = strpos($body, 'zephir_fetch_params');
        $seed  = strpos($body, 'ZVAL_COPY_DEREF(Z_REFVAL_P(&seed)');

        $this->assertIsInt($fetch, "Expected a parameter fetch.\n$body");
        $this->assertIsInt($seed, "Expected the reference to be seeded.\n$body");
        $this->assertGreaterThan(
            $fetch,
            $seed,
            "The reference is seeded from the fetched argument, so it comes after the fetch.\n$body"
        );
    }

    /**
     * The whole point of the shadow: the typed `ZEND_PARSE_PARAMETERS` macro
     * survives, so weak-mode coercion and the arg-info Zephir advertises still
     * agree. Dropping to `Z_PARAM_ZVAL` would make a ZEND_DEBUG build fatal
     * with "Arginfo / zpp mismatch".
     */
    public function testATypedParameterKeepsItsTypedParseMacro(): void
    {
        $body = $this->compileMake('TypedInt', 'typedint', <<<'ZEP'
    public function make(int n) -> <\Closure>
    {
        return function () use (&n) {
            let n = n + 1;

            return n;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'Z_PARAM_LONG(n_byref)',
            $body,
            "An `int` parameter keeps Z_PARAM_LONG(), pointed at the shadow.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_LONG(Z_REFVAL_P(&n), n_byref);',
            $body,
            "The reference is seeded from the unboxed C scalar.\n$body"
        );
    }

    public function testADoubleParameterIsSeededAsADouble(): void
    {
        $body = $this->compileMake('TypedDouble', 'typeddouble', <<<'ZEP'
    public function make(double d) -> <\Closure>
    {
        return function () use (&d) {
            return d;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'ZVAL_DOUBLE(Z_REFVAL_P(&d), d_byref);',
            $body,
            "A `double` parameter is seeded with ZVAL_DOUBLE.\n$body"
        );
    }

    public function testABoolParameterIsSeededAsABool(): void
    {
        $body = $this->compileMake('TypedBool', 'typedbool', <<<'ZEP'
    public function make(bool flag) -> <\Closure>
    {
        return function () use (&flag) {
            return flag;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'ZVAL_BOOL(Z_REFVAL_P(&flag), flag_byref);',
            $body,
            "A `bool` parameter is seeded with ZVAL_BOOL.\n$body"
        );
    }

    /**
     * A `string` parameter that is never reassigned is held as a native
     * `zend_string *`, so seeding it takes an owning ZVAL_STR_COPY.
     */
    public function testANativeStringParameterIsSeededFromTheZendString(): void
    {
        $body = $this->compileMake('NativeString', 'nativestring', <<<'ZEP'
    public function make(string s) -> <\Closure>
    {
        return function () use (&s) {
            return s;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'Z_PARAM_STR(s_byref)',
            $body,
            "The native-string strategy still applies to the shadow.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_STR_COPY(&s_byref_zv, s_byref);',
            $body,
            "The prologue still fills the native string's companion zval.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY(Z_REFVAL_P(&s), &s_byref_zv);',
            $body,
            "The reference is seeded from that companion, which is also what an "
            . "optional `string s = null` leaves behind.\n$body"
        );
    }

    /**
     * Reassigning the name in the body normally disqualifies a `string`
     * parameter from the native `zend_string *` strategy. A by-reference
     * captured one keeps it: the write lands in the reference, so the shadow is
     * still filled exactly once.
     */
    public function testAMutatedStringParameterStillUsesTheNativeShadow(): void
    {
        $body = $this->compileMake('MutatedString', 'mutatedstring', <<<'ZEP'
    public function make(string s) -> <\Closure>
    {
        let s = s . "!";

        return function () use (&s) {
            return s;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'zend_string *s_byref',
            $body,
            "The shadow is written once, so it stays a native zend_string *.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY(Z_REFVAL_P(&s), &s_byref_zv);',
            $body,
            "The reference is seeded from the companion zval.\n$body"
        );
        $this->assertStringContainsString(
            'ZEPHIR_CONCAT_VS(&_0, Z_REFVAL_P(&s), "!");',
            $body,
            "The body's write reads and writes the reference, never the shadow.\n$body"
        );
        $this->assertStringNotContainsString(
            'Z_REFVAL_P(&s_byref)',
            $body,
            "The shadow is never a reference.\n$body"
        );
    }

    public function testAnArrayParameterKeepsItsCoercionAndIsSeededFromIt(): void
    {
        $body = $this->compileMake('TypedArray', 'typedarray', <<<'ZEP'
    public function make(array items) -> <\Closure>
    {
        return function () use (&items) {
            return items;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_get_arrval(&items_byref, items_byref_param);',
            $body,
            "The array coercion keeps running against the shadow.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY(Z_REFVAL_P(&items), &items_byref);',
            $body,
            "An array parameter is seeded with ZVAL_COPY.\n$body"
        );
    }

    public function testAClassHintedParameterIsBoxed(): void
    {
        $body = $this->compileMake('Hinted', 'hinted', <<<'ZEP'
    public function make(<\ArrayObject> bag) -> <\Closure>
    {
        return function () use (&bag) {
            return bag;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'ZVAL_COPY_DEREF(Z_REFVAL_P(&bag), bag_byref);',
            $body,
            "A class-hinted parameter is a borrowed zval * like any other.\n$body"
        );
    }

    /**
     * The default value is assigned to the shadow by the existing optional
     * parameter machinery, so the reference only ever sees a populated shadow.
     */
    public function testAnOptionalParameterKeepsItsDefaultValueHandling(): void
    {
        $body = $this->compileMake('Optional', 'optional', <<<'ZEP'
    public function make(var seed = 3) -> <\Closure>
    {
        return function () use (&seed) {
            let seed = seed * 2;

            return seed;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'ZVAL_LONG(seed_byref, 3);',
            $body,
            "The default is still assigned by assignDefaultValue(), against the shadow.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY_DEREF(Z_REFVAL_P(&seed), seed_byref);',
            $body,
            "The reference is seeded from the shadow either way.\n$body"
        );
    }

    public function testAVariadicParameterIsBoxed(): void
    {
        $body = $this->compileMake('Variadic', 'variadic', <<<'ZEP'
    public function make(var... rest) -> <\Closure>
    {
        return function () use (&rest) {
            return rest;
        };
    }
ZEP);

        $this->assertStringContainsString(
            'zephir_get_args_from(&rest_byref, 0);',
            $body,
            "The variadic is still collected into the shadow.\n$body"
        );
        $this->assertStringContainsString(
            'ZVAL_COPY(Z_REFVAL_P(&rest), &rest_byref);',
            $body,
            "The collected array is copied into the reference.\n$body"
        );
    }

    /**
     * The reference slot is private to this call, so there is nothing to
     * separate from the caller.
     */
    public function testACapturedParameterIsNotSeparated(): void
    {
        $body = $this->compileMake('NoSeparate', 'noseparate', <<<'ZEP'
    public function make(var seed) -> <\Closure>
    {
        let seed = seed + 1;

        return function () use (&seed) {
            let seed = seed + 1;

            return seed;
        };
    }
ZEP);

        $this->assertStringNotContainsString(
            'ZEPHIR_SEPARATE_PARAM(seed)',
            $body,
            "A by-reference captured parameter is never separated.\n$body"
        );
    }

    /**
     * Two closures over one parameter get the same reference, exactly as PHP's
     * second `ZEND_BIND_LEXICAL` reuses the reference the first one made.
     */
    public function testTwoClosuresOverOneParameterShareOneReference(): void
    {
        $body = $this->compileMake('Shared', 'shared', <<<'ZEP'
    public function make(var seed) -> array
    {
        var bump, read;

        let bump = function () use (&seed) {
            let seed = seed + 1;

            return seed;
        };

        let read = function () use (&seed) {
            return seed;
        };

        return [bump, read];
    }
ZEP);

        $this->assertSame(
            1,
            substr_count($body, 'zephir_make_local_reference(&seed);'),
            "One parameter is one reference, however many closures capture it.\n$body"
        );
        $this->assertSame(
            2,
            substr_count($body, 'zephir_update_property_reference('),
            "Both carriers store the same reference.\n$body"
        );
    }

    /**
     * An inner closure capturing its own local named like an outer parameter
     * says nothing about that parameter. The scan used to recurse into closure
     * bodies and attribute the name to the enclosing method.
     */
    public function testACaptureInsideANestedClosureDoesNotTouchTheOuterParameter(): void
    {
        $body = $this->compileMake('Nested', 'nested', <<<'ZEP'
    public function make(int seed) -> int
    {
        var outer;

        let outer = function () {
            var seed;

            let seed = 1;

            return function () use (&seed) {
                return seed;
            };
        };

        return seed;
    }
ZEP);

        $this->assertStringContainsString(
            'zend_long seed',
            $body,
            "An untouched `int` parameter keeps its native C shape.\n$body"
        );
        $this->assertStringNotContainsString(
            'seed_byref',
            $body,
            "The outer parameter is not captured, so it is not shadowed.\n$body"
        );
    }

    /**
     * A by-reference capture is a write channel into the slot. The enclosing
     * scope already refuses to write a `const` parameter, and the capture clone
     * inside the closure is a fresh writable variable, so allowing this would
     * hand the closure a way around the modifier.
     */
    public function testAConstParameterCannotBeCapturedByReference(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches("/read[- ]only parameter 'seed'/");

        $this->compileMake('ConstParam', 'constparam', <<<'ZEP'
    public function make(const var seed) -> <\Closure>
    {
        return function () use (&seed) {
            return seed;
        };
    }
ZEP);
    }

    /**
     * A generator's parameters are re-registered as locals of the generated
     * step and restored from the generator object on every resume, which would
     * overwrite a freshly made reference each time.
     */
    public function testAGeneratorParameterCannotBeCapturedByReference(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches("/generator/i");

        $this->compileMake('GenParam', 'genparam', <<<'ZEP'
    public function make(var seed)
    {
        yield function () use (&seed) {
            return seed;
        };
    }
ZEP);
    }

    private function compileMake(string $className, string $fileName, string $method): string
    {
        $relPath = "stub/issue2668/$fileName.zep";

        $this->compileSource(
            "Stub\\Issue2668\\$className",
            $relPath,
            "namespace Stub\\Issue2668;\n\nclass $className\n{\n$method\n}\n"
        );

        $c    = $this->generatedC($relPath);
        $body = $this->methodBody($c, "PHP_METHOD(Stub_Issue2668_$className, make)");

        $this->assertNotSame('', $body, "Could not locate make() in the generated C.\n$c");

        return $body;
    }
}
