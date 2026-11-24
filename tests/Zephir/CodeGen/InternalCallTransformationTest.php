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
use Zephir\Config;

/**
 * Code generated with `internal-call-transformation` enabled.
 *
 * The optimization replaces a method call with a direct call to the callee's C
 * function, which skips PHP's method dispatch entirely. That is only correct
 * where PHP would not have dispatched virtually anyway, so these tests pin down
 * exactly which call sites get the direct call and which keep normal dispatch.
 *
 * Runtime behaviour is covered by tests/zept/internal_call_transformation.zept;
 * whether the optimization is still *applied* is only observable in the C, which
 * is what this asserts.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2021
 */
final class InternalCallTransformationTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen(
            'zephir_ict_test_',
            ['stub/issue2021'],
            [
                'namespace'     => 'stub',
                'optimizations' => ['internal-call-transformation' => true],
            ]
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testOptionIsEnabledForTheseFixtures(): void
    {
        $this->assertTrue(
            (new Config())->get('internal-call-transformation', 'optimizations'),
            'The fixture config must enable the optimization, or these tests prove nothing.'
        );
    }

    /**
     * A protected, non-final method of a non-final class can be overridden, so
     * the call runs the copy only when the method the object has is this one,
     * and makes PHP's call otherwise.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testOverridableMethodGetsAGuardedDirectCall(): void
    {
        $body = $this->compileAndSlice(
            'Overridable',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class Overridable
                {
                    protected function step(var b) -> string
                    {
                        return "step";
                    }

                    public function run(var b) -> string
                    {
                        return this->step(b);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString(
            'if (zephir_cached_method_is(&_zephir_call_cache[0], this_ptr, ZEND_MN(Stub_Issue2021_Overridable_step), stub_issue2021_overridable_ce)) {',
            $body,
            "The direct call must be guarded by the method the call site resolved.\nEmitted body:\n" . $body
        );
        $this->assertStringContainsString('step_zephir_internal_call', $body);
        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_METHOD_CACHED(this_ptr, "step", &_zephir_call_cache[0], b);',
            $body,
            "PHP's call stays as the other branch, on the same cache record.\nEmitted body:\n" . $body
        );
    }

    /**
     * A class type on a variable is not checked, so a final method called on
     * a variable is guarded too: the variable may hold null or another class.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testFinalMethodOnAVariableIsGuarded(): void
    {
        $body = $this->compileAndSlice(
            'FinalOnVariable',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class FinalOnVariable
                {
                    final public function step() -> string
                    {
                        return "step";
                    }

                    public function run(var x) -> string
                    {
                        var v;

                        let v = <FinalOnVariable> x;

                        return v->step();
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString(
            'zephir_cached_method_is(&_zephir_call_cache[0], &v, ZEND_MN(Stub_Issue2021_FinalOnVariable_step)',
            $body
        );
    }

    /**
     * Each argument must already have the type the parameter parsing would
     * accept without a conversion; anything else makes PHP's call, which
     * converts or throws as PHP does. A native argument needs no check.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testArgumentsAreCheckedAgainstTheParameterTypes(): void
    {
        $c = $this->compileZep(
            'TypedStep',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class TypedStep
                {
                    final public function step(long a, string s, array arr, <TypedStep> o, var v, bool b, double d, string n = null) -> string
                    {
                        return "step";
                    }

                    public function run(var a, var s, var arr, var o, var v, var b, var d, var n) -> string
                    {
                        return this->step(a, s, arr, o, v, b, d, n);
                    }

                    public function natives(long a, bool b, double d) -> string
                    {
                        return this->step(a, "s", [], this, null, b, d);
                    }
                }
                ZEP
        );

        $run = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2021_TypedStep, run)');
        foreach (
            [
                'Z_TYPE_P(a) == IS_LONG',
                'Z_TYPE_P(s) == IS_STRING',
                'Z_TYPE_P(arr) == IS_ARRAY',
                '(Z_TYPE_P(o) == IS_OBJECT && instanceof_function(Z_OBJCE_P(o), stub_issue2021_typedstep_ce))',
                '(Z_TYPE_P(b) == IS_TRUE || Z_TYPE_P(b) == IS_FALSE)',
                '(Z_TYPE_P(d) == IS_DOUBLE || Z_TYPE_P(d) == IS_LONG)',
                '(Z_TYPE_P(n) == IS_STRING || Z_TYPE_P(n) == IS_NULL)',
            ] as $check
        ) {
            $this->assertStringContainsString($check, $run, "Missing argument check.\nEmitted body:\n" . $run);
        }
        $this->assertStringNotContainsString('Z_TYPE_P(v)', $run, 'A var parameter takes any value.');

        $natives = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2021_TypedStep, natives)');
        $this->assertStringContainsString('step_zephir_internal_call', $natives);
        $this->assertStringNotContainsString('IS_LONG', $natives, "A native long needs no check.\n" . $natives);
        $this->assertStringNotContainsString('IS_TRUE', $natives, "A native bool needs no check.\n" . $natives);
        $this->assertStringNotContainsString('IS_DOUBLE', $natives, "A native double needs no check.\n" . $natives);
    }

    /**
     * The call sites of a copy pass a string to a string parameter, so the
     * copy borrows its zend_string as the method does, instead of converting
     * the argument into a zval it has to manage.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testTwinBorrowsANativeStringArgument(): void
    {
        $c = $this->compileZep(
            'StringStep',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class StringStep
                {
                    final public function step(string s) -> string
                    {
                        return s;
                    }
                }
                ZEP
        );

        $twin = $this->methodBody($c, 'void zep_Stub_Issue2021_StringStep_step_zephir_internal_call');
        $this->assertStringContainsString('s = zephir_internal_str_arg(s_param_ext);', $twin, $twin);
        $this->assertStringNotContainsString('zephir_get_strval', $twin, $twin);
    }

    /**
     * A union parameter is checked by its own arginfo, which the copy has no
     * frame for, so the call stays PHP's.
     */
    public function testUnionParameterKeepsTheCall(): void
    {
        $body = $this->compileAndSlice(
            'UnionStep',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class UnionStep
                {
                    final public function step(int|string a) -> string
                    {
                        return "step";
                    }

                    public function run(var a) -> string
                    {
                        return this->step(a);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringNotContainsString('_zephir_internal_call', $body);
    }

    /**
     * The copy skips zend_call_function(), where PHP reports a deprecated
     * method, and runs on a frame without arguments, which func_get_args() and
     * its siblings read. Neither can have a copy. A variadic method neither.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testMethodsThatNeedTheirOwnCallGetNoTwin(): void
    {
        $c = $this->compileZep(
            'OwnCall',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class OwnCall
                {
                    final public deprecated function old() -> string
                    {
                        return "old";
                    }

                    final public function args(var a) -> int
                    {
                        if a {
                            return func_num_args();
                        }

                        return 0;
                    }

                    final public function rest(var a, ...b) -> int
                    {
                        return 1;
                    }

                    final public function plain() -> int
                    {
                        return 1;
                    }
                }
                ZEP
        );

        $this->assertStringNotContainsString('old_zephir_internal_call', $c);
        $this->assertStringNotContainsString('args_zephir_internal_call', $c);
        $this->assertStringNotContainsString('rest_zephir_internal_call', $c);
        $this->assertStringContainsString('plain_zephir_internal_call', $c);
    }

    /**
     * A trait's method runs with the scope of the class that uses it, which
     * the copy cannot know.
     */
    public function testTraitMethodGetsNoTwin(): void
    {
        $c = $this->compileZep(
            'Greets',
            <<<'ZEP'
                namespace Stub\Issue2021;

                trait Greets
                {
                    final public function greet() -> string
                    {
                        return "hi";
                    }
                }
                ZEP
        );

        $this->assertStringNotContainsString('greet_zephir_internal_call', $c);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testUlongParameterIsPassedToTheTwin(): void
    {
        $c = $this->compileZep(
            'UlongStep',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class UlongStep
                {
                    final public function step(ulong n) -> ulong
                    {
                        return n;
                    }
                }
                ZEP
        );

        $this->assertStringContainsString('zval *n_param_ext)', $c);
    }

    /**
     * An argument left out reaches the copy as NULL, where it takes the
     * default exactly as the method does when PHP leaves it out.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testOptionalParametersGetATwin(): void
    {
        $body = $this->compileAndSlice(
            'OptionalStep',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class OptionalStep
                {
                    final public function step(var a, long b = 2, string c = "c") -> string
                    {
                        return "step";
                    }

                    public function run(var a) -> string
                    {
                        return this->step(a);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString('step_zephir_internal_call, a, ZEPHIR_NO_ARG, ZEPHIR_NO_ARG);', $body);
    }

    /**
     * `new` calls the constructor through a method call, which takes the
     * constructor's copy with the same guard.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testNewCallsTheConstructorTwin(): void
    {
        $c = $this->compileZep(
            'Point',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class Point
                {
                    public x;

                    public function __construct(long x)
                    {
                        let this->x = x;
                    }

                    public static function make(var x) -> <Point>
                    {
                        return new Point(x);
                    }
                }
                ZEP
        );

        $make = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2021_Point, make)');
        $this->assertStringContainsString('ZEND_MN(Stub_Issue2021_Point___construct)', $make, $make);
        $this->assertStringContainsString('__construct_zephir_internal_call', $make, $make);
        $this->assertStringContainsString('zephir_constructor_finish(', $make, $make);
    }

    /**
     * `self::` reaches a non-static method with the current $this, and passes
     * on the called class to a static one.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testSelfCallsKeepThisAndTheCalledClass(): void
    {
        $c = $this->compileZep(
            'SelfCalls',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class SelfCalls
                {
                    protected function who(long n) -> string
                    {
                        return "who";
                    }

                    protected static function staticWho() -> string
                    {
                        return "staticWho";
                    }

                    public function viaSelf(var n) -> string
                    {
                        return self::who(n);
                    }

                    public function viaSelfStatic() -> string
                    {
                        return self::staticWho();
                    }
                }
                ZEP
        );

        $viaSelf = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2021_SelfCalls, viaSelf)');
        $this->assertStringContainsString('if (Z_TYPE_P(n) == IS_LONG', $viaSelf, $viaSelf);
        $this->assertStringContainsString(
            'ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P1(this_ptr, stub_issue2021_selfcalls_ce, zep_Stub_Issue2021_SelfCalls_who_zephir_internal_call, n);',
            $viaSelf,
            $viaSelf
        );

        $viaSelfStatic = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2021_SelfCalls, viaSelfStatic)');
        $this->assertStringContainsString(
            'stub_issue2021_selfcalls_ce, zephir_get_called_scope(execute_data), zep_Stub_Issue2021_SelfCalls_staticWho_zephir_internal_call',
            $viaSelfStatic,
            $viaSelfStatic
        );
    }

    public function testFinalMethodGetsTheDirectCall(): void
    {
        $body = $this->compileAndSlice(
            'FinalMethod',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class FinalMethod
                {
                    final protected function step(var b) -> string
                    {
                        return "step";
                    }

                    public function run(var b) -> string
                    {
                        return this->step(b);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString('step_zephir_internal_call', $body);
    }

    public function testPrivateMethodGetsTheDirectCall(): void
    {
        $body = $this->compileAndSlice(
            'PrivateMethod',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class PrivateMethod
                {
                    private function step(var b) -> string
                    {
                        return "step";
                    }

                    public function run(var b) -> string
                    {
                        return this->step(b);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString('step_zephir_internal_call', $body);
    }

    public function testMethodOfFinalClassGetsTheDirectCall(): void
    {
        $body = $this->compileAndSlice(
            'FinalClass',
            <<<'ZEP'
                namespace Stub\Issue2021;

                final class FinalClass
                {
                    protected function step(var b) -> string
                    {
                        return "step";
                    }

                    public function run(var b) -> string
                    {
                        return this->step(b);
                    }
                }
                ZEP,
            'run'
        );

        $this->assertStringContainsString('step_zephir_internal_call', $body);
    }

    /**
     * The generated twin is called C-to-C with the *caller's* execute_data, so
     * parsing that frame for its own parameters read the wrong arguments (and
     * crashed when the arities differed).
     */
    public function testGeneratedInternalMethodDoesNotParseTheCallersFrame(): void
    {
        $c = $this->compileZep(
            'FinalTyped',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class FinalTyped
                {
                    final protected function step(long a, long b) -> long
                    {
                        return a + b;
                    }

                    public function run() -> long
                    {
                        return this->step(3, 4);
                    }
                }
                ZEP
        );

        $internal = $this->sliceBetween(
            $c,
            'void zep_Stub_Issue2021_FinalTyped_step_zephir_internal_call',
            "\n}"
        );

        $this->assertNotSame('', $internal, "Could not locate the generated internal function.\n" . $c);
        $this->assertStringNotContainsString('ZEND_PARSE_PARAMETERS_START', $internal);

        // Exactly once: the twin used to share the original's Parameters instance.
        $this->assertSame(
            1,
            substr_count($internal, 'a = zephir_get_intval(a_param);'),
            "The parameter conversion must be emitted exactly once.\n" . $internal
        );
    }

    /**
     * An abstract method has no body to copy. The twin's visibility is only
     * `internal`, so it does not inherit `isAbstract()` and used to be emitted
     * as a body-less C function that silently returned nothing.
     */
    public function testAbstractMethodGetsNoTwin(): void
    {
        $c = $this->compileZep(
            'AbstractShape',
            <<<'ZEP'
                namespace Stub\Issue2021;

                abstract class AbstractShape
                {
                    abstract protected function label() -> string;

                    final public function describe() -> string
                    {
                        return "shape";
                    }
                }
                ZEP
        );

        $this->assertStringNotContainsString('label_zephir_internal_call', $c);
        $this->assertStringContainsString('describe_zephir_internal_call', $c);
    }

    /**
     * GeneratorTransformer moves a generator's body into a step and nulls the
     * creator's statements before setupOptimized() runs, so a twin would return
     * null in place of a Generator object.
     */
    public function testGeneratorMethodGetsNoTwin(): void
    {
        $c = $this->compileZep(
            'Counter',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class Counter
                {
                    public function numbers()
                    {
                        yield 1;
                        yield 2;
                    }
                }
                ZEP
        );

        $this->assertStringNotContainsString('numbers_zephir_internal_call', $c);
    }

    /**
     * A method called C-to-C runs on a frame named after it, set up when the
     * class registers, so a warning it raises names it. A generator's step is
     * resumed by the generator, from a frame of its own, and gets none.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    public function testOnlyMethodsCalledDirectlyGetAFrame(): void
    {
        $c = $this->compileZep(
            'Frames',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class Frames
                {
                    public function run()
                    {
                        return this->helper();
                    }

                    internal function helper()
                    {
                        return 1;
                    }

                    public function numbers()
                    {
                        yield 1;
                    }
                }
                ZEP
        );

        $this->assertStringContainsString(
            'zephir_init_internal_frame(&zephir_frame_zep_Stub_Issue2021_Frames_helper, stub_issue2021_frames_ce, SL("helper"));',
            $c
        );
        $this->assertStringNotContainsString('zephir_frame_zep_Stub_Issue2021_Frames_zephir_gen_step_numbers', $c);
    }

    /**
     * `__FUNCTION__`/`__METHOD__` are folded at compile time from the method
     * being compiled. The twin is compiled as a method in its own right, so the
     * fold ran a second time against the mangled name and baked it into the
     * emitted string literal.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2643
     */
    public function testMagicConstantsFoldToTheDeclaredName(): void
    {
        $c = $this->compileZep(
            'MagicNames',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class MagicNames
                {
                    private function privateStep() -> string
                    {
                        return __FUNCTION__;
                    }

                    final protected function finalStep() -> string
                    {
                        return __FUNCTION__;
                    }

                    protected static function selfStep() -> string
                    {
                        return __METHOD__;
                    }

                    public function viaPrivate() -> string
                    {
                        return this->privateStep();
                    }

                    public function viaFinal() -> string
                    {
                        return this->finalStep();
                    }

                    public function viaSelf() -> string
                    {
                        return self::selfStep();
                    }
                }
                ZEP
        );

        // The twin must still be emitted: this test is about the folded value,
        // not about disabling the optimization.
        $this->assertStringContainsString('privateStep_zephir_internal_call', $c);

        // Quoting is the invariant: the mangled name is legitimate as a C
        // function name and never legitimate as a string literal.
        $leaked = "The twin's name reached a folded string literal.\nEmitted C:\n" . $c;

        $this->assertStringNotContainsString('"privateStep_zephir_internal_call"', $c, $leaked);
        $this->assertStringNotContainsString('"finalStep_zephir_internal_call"', $c, $leaked);
        $this->assertStringNotContainsString('"Stub\\\\Issue2021\\\\MagicNames::selfStep_zephir_internal_call"', $c, $leaked);

        $folded = "The declared name was not folded into the twin's body.\nEmitted C:\n" . $c;

        $this->assertStringContainsString('"privateStep"', $c, $folded);
        $this->assertStringContainsString('"finalStep"', $c, $folded);
        $this->assertStringContainsString('"Stub\\\\Issue2021\\\\MagicNames::selfStep"', $c, $folded);
    }

    /**
     * The same defect without the optimization: GeneratorTransformer moves the
     * body into a step method named `zephir_gen_step_<name>`, and the fold runs
     * against the step.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2643
     */
    public function testGeneratorStepMagicConstantsFoldToTheDeclaredName(): void
    {
        $c = $this->compileZep(
            'GenNames',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class GenNames
                {
                    public function numbers()
                    {
                        yield __FUNCTION__;
                        yield __METHOD__;
                    }
                }
                ZEP
        );

        $leaked = "The step's name reached a folded string literal.\nEmitted C:\n" . $c;

        $this->assertStringNotContainsString('"zephir_gen_step_numbers"', $c, $leaked);
        $this->assertStringNotContainsString('"Stub\\\\Issue2021\\\\GenNames::zephir_gen_step_numbers"', $c, $leaked);

        $folded = "The declared name was not folded into the step's body.\nEmitted C:\n" . $c;

        $this->assertStringContainsString('"numbers"', $c, $folded);
        $this->assertStringContainsString('"Stub\\\\Issue2021\\\\GenNames::numbers"', $c, $folded);
    }

    /**
     * Compile a single-class fixture and return the whole emitted `.zep.c`.
     */
    /**
     * A by-reference captured parameter keeps its native value under a shadow
     * C identifier, and an internal method reads its arguments through
     * trailing `_ext` pointers named after that same identifier. The twin's C
     * signature and its body have to agree on the name, or the twin references
     * an undeclared one.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2668
     */
    public function testTheInternalTwinNamesTheShadowOfAByReferenceCapturedParameter(): void
    {
        $c = $this->compileZep(
            'ByRefCapture',
            <<<'ZEP'
                namespace Stub\Issue2021;

                class ByRefCapture
                {
                    public function make(int seed) -> <\Closure>
                    {
                        return function () use (&seed) {
                            let seed = seed + 1;

                            return seed;
                        };
                    }

                    public function call() -> int
                    {
                        var closure;

                        let closure = this->make(1);

                        return call_user_func(closure);
                    }
                }
                ZEP
        );

        $this->assertStringContainsString(
            'zval *seed_byref_param_ext)',
            $c,
            "The twin's signature has to declare the shadow's _ext pointer.\n$c"
        );
        $this->assertStringContainsString(
            'seed_byref_param = seed_byref_param_ext;',
            $c,
            "And its body has to read that same pointer.\n$c"
        );
        $this->assertStringContainsString(
            'seed_byref = zephir_get_intval(seed_byref_param);',
            $c,
            "The declared type is still applied to the shadow inside the twin.\n$c"
        );
        $this->assertStringContainsString(
            'ZVAL_LONG(Z_REFVAL_P(&seed), seed_byref);',
            $c,
            "The reference is still seeded from the shadow inside the twin.\n$c"
        );
    }

    private function compileZep(string $className, string $zep): string
    {
        $relPath = 'stub/issue2021/' . strtolower($className) . '.zep';

        $this->compileSource('Stub\\Issue2021\\' . $className, $relPath, $zep);

        return $this->generatedC($relPath);
    }

    /**
     * Compile a fixture and return only the body of one PHP_METHOD.
     */
    private function compileAndSlice(string $className, string $zep, string $method): string
    {
        $c    = $this->compileZep($className, $zep);
        $body = $this->sliceBetween($c, 'PHP_METHOD(Stub_Issue2021_' . $className . ', ' . $method . ')', "\n}");

        $this->assertNotSame('', $body, "Could not locate {$className}::{$method} in:\n" . $c);

        return $body;
    }

    private function sliceBetween(string $haystack, string $start, string $end): string
    {
        $startPos = strpos($haystack, $start);
        if ($startPos === false) {
            return '';
        }

        $endPos = strpos($haystack, $end, $startPos);
        if ($endPos === false) {
            return '';
        }

        return substr($haystack, $startPos, $endPos - $startPos);
    }
}
