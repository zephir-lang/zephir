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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * A compile error must reach the user, never a deprecation raised while the
 * exception carrying it is being built.
 *
 * PHP 8.4 deprecates an implicitly nullable parameter, and it reports that
 * from the compiler (`Zend/zend_compile.c`), so the diagnostic is emitted while
 * the *class file* is compiled, which for a lazily autoloaded exception is the
 * moment of the very first `throw new ...`. The CLI's error handler turns any
 * reportable diagnostic into an `ErrorException`, so the deprecation was thrown
 * out of the `new` expression and the intended exception was never constructed,
 * so `zephir generate` printed the deprecation text where the compile error
 * belonged. `generate` is the whole blast radius: `compile` and `install` never
 * run the front end, and `build` funnels through `generate` first.
 *
 * A subprocess per case is what makes this observable. A class is compiled once
 * per process, so any test that has already autoloaded the exception class sees
 * no deprecation and passes regardless of the defect. Each case therefore gets
 * its own project and its own `php zephir` run, so each one owns the first
 * autoload of the class it is about.
 *
 * Both cases stay silent on PHP 8.0 to 8.3, where the deprecation does not
 * exist. `Zephir\Test\ImplicitNullableParameterTest` is the guard that holds on
 * every supported version.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2663
 */
final class ImplicitNullableParameterTest extends TestCase
{
    use RunsZephirCommands;

    /**
     * The text PHP 8.4+ emits for an implicitly nullable parameter. It must
     * never be what a compile error looks like to the user.
     */
    private const DEPRECATION = 'Implicitly marking parameter';

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * `IllegalOperationException` is thrown with three arguments only from the
     * array-assignment path, so an arithmetic operator on two arrays is the one
     * fixture that exercises the `$extra` parameter this test is about. PHP
     * rejects the same expression with
     * `TypeError: Unsupported operand types: array * array`, so it cannot
     * become valid later and quietly stop covering anything.
     */
    public function testIllegalOperationErrorIsReported(): void
    {
        [$exitCode, $stderr] = $this->generateProject('implicitnullableop', [
            't.zep' => "namespace Implicitnullableop;\n\nclass T\n{\n    public function f()\n    {\n"
                . "        array a = [];\n        array b = [];\n        let a *= b;\n    }\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringNotContainsString(self::DEPRECATION, $stderr);
        $this->assertStringContainsString(
            "Operator 'mul-assign' is not supported for variable type: variable",
            $stderr,
        );
    }

    /**
     * `InvalidTypeException` reaches the user from every return-type check in
     * `ReturnStatement`, and a return type the returned expression cannot
     * satisfy is permanently an error.
     */
    public function testInvalidTypeErrorIsReported(): void
    {
        [$exitCode, $stderr] = $this->generateProject('implicitnullablereturn', [
            't.zep' => "namespace Implicitnullablereturn;\n\nclass T\n{\n"
                . "    public function f() -> string\n    {\n        return 1;\n    }\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringNotContainsString(self::DEPRECATION, $stderr);
        $this->assertStringContainsString(
            'Returning type: int but this type is not compatible with return-type hints declared in the method',
            $stderr,
        );
    }

    /**
     * The positive control. Both cases above assert a *failing* compile, so a
     * compiler that rejected everything would satisfy them; this one proves the
     * fixtures fail for the reason claimed and not because the project shape is
     * broken.
     */
    public function testAValidProjectStillGenerates(): void
    {
        [$exitCode, $stderr] = $this->generateProject('implicitnullableok', [
            't.zep' => "namespace Implicitnullableok;\n\nclass T\n{\n"
                . "    public function f() -> string\n    {\n        return \"1\";\n    }\n}\n",
        ]);

        $this->assertSame(0, $exitCode, $stderr);
        $this->assertStringNotContainsString(self::DEPRECATION, $stderr);
    }

    /**
     * @param array<string, string> $files
     *
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, array $files): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        foreach ($files as $fileName => $source) {
            file_put_contents($projectDir . '/' . $name . '/' . $fileName, $source);
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }
}
