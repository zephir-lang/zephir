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
use Psr\Log\NullLogger;
use Zephir\Backend\Backend;
use Zephir\Backend\StringsManager;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

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
    private string $originalCwd;
    private string $tempDir;
    private Compiler $compiler;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();

        $this->tempDir = sys_get_temp_dir() . '/zephir_ict_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2021', 0755, true);
        mkdir($this->tempDir . '/stub/issue2021', 0755, true);

        file_put_contents(
            $this->tempDir . '/config.json',
            json_encode(
                [
                    'namespace'     => 'stub',
                    'optimizations' => ['internal-call-transformation' => true],
                ],
                JSON_PRETTY_PRINT
            )
        );

        chdir($this->tempDir);

        $config = new Config();
        $disk   = new HardDisk($this->tempDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');
        $logger  = new NullLogger();
        $backend = new Backend($config, ZEPHIRPATH . 'kernel', ZEPHIRPATH . 'templates');

        $compilerFactory = new CompilerFileFactory($config, $disk, $logger);

        $this->compiler = new Compiler(
            $config,
            $backend,
            new Manager(new Parser()),
            $disk,
            $compilerFactory
        );
        $this->compiler->setLogger($logger);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);
        $this->removeDirectory($this->tempDir);
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
     * Compile a single-class fixture and return the whole emitted `.zep.c`.
     */
    private function compileZep(string $className, string $zep): string
    {
        $lower = strtolower($className);

        file_put_contents($this->tempDir . '/stub/issue2021/' . $lower . '.zep', $zep);

        $factoryProp = (new \ReflectionClass($this->compiler))->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        $compilerFile = $compilerFileFactory->create(
            'Stub\\Issue2021\\' . $className,
            'stub/issue2021/' . $lower . '.zep'
        );
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        return file_get_contents($this->tempDir . '/ext/stub/issue2021/' . $lower . '.zep.c');
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

    private function removeDirectory(string $dir): void
    {
        if (!is_dir($dir)) {
            return;
        }

        $items = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($dir, \FilesystemIterator::SKIP_DOTS),
            \RecursiveIteratorIterator::CHILD_FIRST
        );

        foreach ($items as $item) {
            $item->isDir() ? rmdir($item->getPathname()) : unlink($item->getPathname());
        }

        rmdir($dir);
    }
}
