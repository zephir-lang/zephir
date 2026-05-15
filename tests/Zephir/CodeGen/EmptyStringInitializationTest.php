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
 * Regression coverage for `let s = "";` on string-typed locals.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2393
 */
final class EmptyStringInitializationTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_emptystr_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2393', 0755, true);
        mkdir($this->tempDir . '/stub/issue2393', 0755, true);

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

    public function testEmptyStringDeclarationInitializerIsEmittedBeforeSwitch(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2393;

class Decl
{
    public function getSpecial(int special) -> string
    {
        string suffix = "";

        switch special {
            case 1:
                let suffix = "alpha";
                break;
        }

        return suffix;
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2393/decl.zep', $zep);

        $c = $this->compileZep(
            'Stub\\Issue2393\\Decl',
            'stub/issue2393/decl.zep',
            'stub/issue2393/decl'
        );

        $beforeSwitch = $this->sliceBetween($c, 'PHP_METHOD(Stub_Issue2393_Decl, getSpecial)', 'do {');
        $this->assertNotSame(
            '',
            $beforeSwitch,
            'Could not locate the getSpecial body up to the switch.'
        );

        $this->assertTrue(
            str_contains($beforeSwitch, 'ZVAL_EMPTY_STRING(&suffix)')
                || str_contains($beforeSwitch, 'ZVAL_STRING(&suffix, "")'),
            "The empty-string initializer for 'suffix' must be emitted before the switch.\n"
            . "Pre-switch body:\n" . $beforeSwitch
        );
    }

    public function testExplicitLetEmptyStringIsEmitted(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2393;

class Lett
{
    public function getSpecial(int special) -> string
    {
        string suffix;
        let suffix = "";

        switch special {
            case 1:
                let suffix = "alpha";
                break;
        }

        return suffix;
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2393/lett.zep', $zep);

        $c = $this->compileZep(
            'Stub\\Issue2393\\Lett',
            'stub/issue2393/lett.zep',
            'stub/issue2393/lett'
        );

        $beforeSwitch = $this->sliceBetween($c, 'PHP_METHOD(Stub_Issue2393_Lett, getSpecial)', 'do {');
        $this->assertNotSame(
            '',
            $beforeSwitch,
            'Could not locate the getSpecial body up to the switch.'
        );

        $this->assertTrue(
            str_contains($beforeSwitch, 'ZVAL_EMPTY_STRING(&suffix)')
                || str_contains($beforeSwitch, 'ZVAL_STRING(&suffix, "")'),
            "let suffix = \"\" must emit the empty-string assignment before the switch.\n"
            . "Pre-switch body:\n" . $beforeSwitch
        );
    }

    private function compileZep(string $className, string $filePath, string $extPath): string
    {
        $factory     = new \ReflectionClass($this->compiler);
        $factoryProp = $factory->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        /** @var \Zephir\CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create($className, $filePath);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        return file_get_contents($this->tempDir . '/ext/' . $extPath . '.zep.c');
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
