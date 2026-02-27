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
use Zephir\Exception;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Tests that the C and H files generated from Zephir source code are
 * 100% identical to the pre-generated reference fixtures.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1803
 */
final class ConstructorsCodeGenTest extends TestCase
{
    private string $originalCwd;
    private string $tempDir;
    private string $fixturesDir;
    private Compiler $compiler;

    protected function setUp(): void
    {
        $this->originalCwd  = getcwd();
        // Resolve the fixtures path to absolute BEFORE any chdir(), so reads
        // work correctly even after we switch the working directory.
        $this->fixturesDir  = realpath(FIXTURES_PATH . '/constructors');

        // Create an isolated temp working directory so that all relative
        // paths used by CompilerFile (e.g. "ext/stub/constructors/…") land
        // inside it and do not touch the real project tree.
        $this->tempDir = sys_get_temp_dir() . '/zephir_codegen_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/constructors', 0755, true);
        mkdir($this->tempDir . '/stub/constructors', 0755, true);

        // Write the Zephir source under the path that matches the namespace
        // Stub\Constructors\Issue1803 → stub/constructors/issue1803.zep
        $zepCode = <<<'ZEP'

namespace Stub\Constructors;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1803
 */
class Issue1803
{
    public value = "Original value";

    public function Issue1803()
    {
        let this->value = "Value that won't be updated on class init";
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/constructors/issue1803.zep',
            $zepCode
        );

        // Config::populate() reads config.json from CWD; write a minimal one
        // so the namespace is available without touching the project's own file.
        $configData = json_encode(['namespace' => 'stub'], JSON_PRETTY_PRINT);
        file_put_contents($this->tempDir . '/config.json', $configData);

        // Change CWD so that all relative paths resolve inside $tempDir.
        chdir($this->tempDir);

        $config  = new Config();
        $disk    = new HardDisk($this->tempDir . '/.zephir');
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

        // Recursively remove the temp directory.
        if (is_dir($this->tempDir)) {
            $iterator = new \RecursiveIteratorIterator(
                new \RecursiveDirectoryIterator(
                    $this->tempDir,
                    \FilesystemIterator::SKIP_DOTS
                ),
                \RecursiveIteratorIterator::CHILD_FIRST
            );
            foreach ($iterator as $file) {
                $file->isDir() ? rmdir($file->getRealPath()) : unlink($file->getRealPath());
            }
            rmdir($this->tempDir);
        }
    }

    /**
     * Compiles stub/constructors/issue1803.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileIssue1803(): array
    {
        $factory      = new \ReflectionClass($this->compiler);
        $factoryProp  = $factory->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        $className = 'Stub\Constructors\Issue1803';
        $filePath  = 'stub/constructors/issue1803.zep';

        /** @var \Zephir\CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create($className, $filePath);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        $cFile = $this->tempDir . '/ext/stub/constructors/issue1803.zep.c';
        $hFile = $this->tempDir . '/ext/stub/constructors/issue1803.zep.h';

        return [
            file_get_contents($cFile),
            file_get_contents($hFile),
        ];
    }

    /**
     * The generated .c file must be 100% identical to the reference fixture.
     */
    public function testGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileIssue1803();

        $fixture = file_get_contents($this->fixturesDir . '/issue1803.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated .c file does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file must be 100% identical to the reference fixture.
     */
    public function testGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileIssue1803();

        $fixture = file_get_contents($this->fixturesDir . '/issue1803.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated .h file does not match the reference fixture.'
        );
    }
}

