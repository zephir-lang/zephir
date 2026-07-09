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
 * Locks the codegen for `for` loops that iterate the `..` / `...` range
 * operators. Before #2433, `for i in 0..n` desugared to `(array) range(0, n)`
 * and was iterated with `ZEND_HASH_FOREACH`, i.e. it materialised an
 * intermediate array — even though the identical `for i in range(0, n)`
 * already compiled to a bare integer counting loop via
 * `ForStatement::compileRange()`. This test pins the operator form to the
 * same counting-loop codegen (no array, no `range()` call, no hash foreach).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2433
 */
final class RangeOperatorLoopTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_range_operator_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2433', 0755, true);
        mkdir($this->tempDir . '/stub/issue2433', 0755, true);

        file_put_contents(
            $this->tempDir . '/config.json',
            json_encode(['namespace' => 'stub'], JSON_PRETTY_PRINT)
        );

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

    public function testInclusiveRangeOperatorCompilesToCountingLoop(): void
    {
        $c = $this->compileSingle('InclusiveLoop', 'inclusiveloop.zep', <<<'ZEP'
namespace Stub\Issue2433;

class InclusiveLoop
{
    public function sum() -> int
    {
        int total = 0, i = 0;
        for i in 0..10 {
            let total += i;
        }
        return total;
    }
}
ZEP);

        $this->assertStringContainsString(
            'while (1) {',
            $c,
            'The `..` range loop must emit the integer counting loop (`while (1)`) used by compileRange().'
        );

        $this->assertStringNotContainsString(
            'ZEND_HASH_FOREACH',
            $c,
            'The `..` range loop must not materialise an array and iterate it with ZEND_HASH_FOREACH.'
        );

        $this->assertStringNotContainsString(
            '"range"',
            $c,
            'The `..` range loop must not emit a runtime range() function call.'
        );
    }

    public function testExclusiveRangeOperatorCompilesToCountingLoop(): void
    {
        $c = $this->compileSingle('ExclusiveLoop', 'exclusiveloop.zep', <<<'ZEP'
namespace Stub\Issue2433;

class ExclusiveLoop
{
    public function sum() -> int
    {
        int total = 0, i = 0;
        for i in 0...10 {
            let total += i;
        }
        return total;
    }
}
ZEP);

        $this->assertStringContainsString(
            'while (1) {',
            $c,
            'The `...` range loop must emit the integer counting loop (`while (1)`) used by compileRange().'
        );

        $this->assertStringNotContainsString(
            'ZEND_HASH_FOREACH',
            $c,
            'The `...` range loop must not materialise an array and iterate it with ZEND_HASH_FOREACH.'
        );
    }

    private function compileSingle(string $simpleClassName, string $fileName, string $zep): string
    {
        $extDir = $this->tempDir . '/ext/stub/issue2433';
        if (!is_dir($extDir)) {
            mkdir($extDir, 0755, true);
        }

        $srcRel    = 'stub/issue2433/' . $fileName;
        $sourceRel = 'stub/issue2433/' . pathinfo($fileName, PATHINFO_FILENAME);

        file_put_contents($this->tempDir . '/' . $srcRel, $zep);

        $factoryProp = (new \ReflectionClass($this->compiler))->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        $fqcn = 'Stub\\Issue2433\\' . $simpleClassName;
        /** @var \Zephir\CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create($fqcn, $srcRel);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        $sourcePath = $this->tempDir . '/ext/' . $sourceRel . '.zep.c';
        $this->assertFileExists($sourcePath, 'Compiler should emit a C source file.');

        return file_get_contents($sourcePath);
    }
}
