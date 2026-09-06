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

    public function testOverridableMethodKeepsVirtualDispatch(): void
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

        $this->assertStringNotContainsString(
            '_zephir_internal_call',
            $body,
            "A protected, non-final method of a non-final class can be overridden, so run() "
            . "must go through PHP dispatch.\nEmitted body:\n" . $body
        );
        $this->assertStringContainsString('ZEPHIR_RETURN_CALL_METHOD', $body);
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
        $this->assertStringNotContainsString('"MagicNames:selfStep_zephir_internal_call"', $c, $leaked);

        $folded = "The declared name was not folded into the twin's body.\nEmitted C:\n" . $c;

        $this->assertStringContainsString('"privateStep"', $c, $folded);
        $this->assertStringContainsString('"finalStep"', $c, $folded);
        $this->assertStringContainsString('"MagicNames:selfStep"', $c, $folded);
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
        $this->assertStringNotContainsString('"GenNames:zephir_gen_step_numbers"', $c, $leaked);

        $folded = "The declared name was not folded into the step's body.\nEmitted C:\n" . $c;

        $this->assertStringContainsString('"numbers"', $c, $folded);
        $this->assertStringContainsString('"GenNames:numbers"', $c, $folded);
    }

    /**
     * Compile a single-class fixture and return the whole emitted `.zep.c`.
     */
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
