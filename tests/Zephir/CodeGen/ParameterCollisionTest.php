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
use Zephir\Exception\CompilerException;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Regression coverage for redeclaring a parameter as a local `var`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2009
 */
final class ParameterCollisionTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_paramcollision_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2009', 0755, true);
        mkdir($this->tempDir . '/stub/issue2009', 0755, true);

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

    public function testThrowsWhenLocalVarReusesParameterName(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2009;

class Collision
{
    public static function decodes(b32)
    {
        var b32;
        let b32 = strtoupper(b32);
        return strlen(b32);
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2009/collision.zep', $zep);

        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches(
            "/Variable 'b32' was already declared as a parameter of method 'decodes'/"
        );

        $this->compileZep(
            'Stub\\Issue2009\\Collision',
            'stub/issue2009/collision.zep',
            'stub/issue2009/collision'
        );
    }

    public function testAllowsLocalVarWithDifferentName(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2009;

class NoCollision
{
    public static function decodes(b32)
    {
        var local;
        let local = strtoupper(b32);
        return strlen(local);
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2009/nocollision.zep', $zep);

        $this->compileZep(
            'Stub\\Issue2009\\NoCollision',
            'stub/issue2009/nocollision.zep',
            'stub/issue2009/nocollision'
        );

        $c = file_get_contents($this->tempDir . '/ext/stub/issue2009/nocollision.zep.c');

        // Parameter must be declared as zval pointer.
        $this->assertStringContainsString('zval *b32', $c);
        // The local must be a stack zval.
        $this->assertMatchesRegularExpression('/zval [^;]*\\blocal\\b/', $c);
        // No bogus ZEPHIR_SEPARATE_PARAM(b32) — the param is not mutated here.
        $this->assertStringNotContainsString('ZEPHIR_SEPARATE_PARAM(b32)', $c);
    }

    private function compileZep(string $className, string $filePath, string $extPath): void
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
    }
}
