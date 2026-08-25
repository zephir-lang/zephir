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

use FilesystemIterator;
use PHPUnit\Framework\TestCase;
use Psr\Log\NullLogger;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use ReflectionClass;
use Zephir\Backend\Backend;
use Zephir\Backend\StringsManager;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Expression initializers must reach the class-entry emitters already folded to
 * a literal, so the declaration is a plain constant in the generated C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2061
 */
final class ConstantExpressionCodeGenTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_constexpr_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2061', 0755, true);
        mkdir($this->tempDir . '/stub/issue2061', 0755, true);

        file_put_contents(
            $this->tempDir . '/config.json',
            json_encode(['namespace' => 'stub'], JSON_PRETTY_PRINT)
        );

        chdir($this->tempDir);

        $config = new Config();
        $disk   = new HardDisk($this->tempDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');
        $logger  = new NullLogger();
        $backend = new Backend($config, ZEPHIRPATH . 'kernel', ZEPHIRPATH . 'templates');

        $this->compiler = new Compiler(
            $config,
            $backend,
            new Manager(new Parser()),
            $disk,
            new CompilerFileFactory($config, $disk, $logger)
        );
        $this->compiler->setLogger($logger);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);

        if (is_dir($this->tempDir)) {
            $iterator = new RecursiveIteratorIterator(
                new RecursiveDirectoryIterator($this->tempDir, FilesystemIterator::SKIP_DOTS),
                RecursiveIteratorIterator::CHILD_FIRST
            );
            foreach ($iterator as $file) {
                $file->isDir() ? rmdir($file->getRealPath()) : unlink($file->getRealPath());
            }
            rmdir($this->tempDir);
        }
    }

    public static function declarationProvider(): iterable
    {
        yield 'issue expression'  => ['/zephir_declare_class_constant_long\([^,]+, SL\("INT8_MIN"\), -128\);/'];
        yield 'bitmask'           => ['/zephir_declare_class_constant_long\([^,]+, SL\("MASK"\), 65295\);/'];
        yield 'grouped division'  => ['/zephir_declare_class_constant_double\([^,]+, SL\("RATIO"\), 2\.5\);/'];
        yield 'concat'            => ['/zephir_declare_class_constant_string\([^,]+, SL\("NAME"\), "ab"\);/'];
        yield 'comparison'        => ['/zephir_declare_class_constant_bool\([^,]+, SL\("FLAG"\), 1\);/'];
        yield 'reads own const'   => ['/zephir_declare_class_constant_long\([^,]+, SL\("DOUBLED"\), 16384\);/'];
        yield 'property default'  => ['/SL\("size"\), 8192/'];
    }

    /**
     * @dataProvider declarationProvider
     */
    public function testFoldedDeclarationIsEmittedAsALiteral(string $expected): void
    {
        $this->assertMatchesRegularExpression($expected, $this->generatedC());
    }

    public function testNoOperatorNodeLeaksIntoTheEmittedSymbolName(): void
    {
        $c = $this->generatedC();

        foreach (['_sub(', '_add(', '_mul(', '_bitwise_or(', '_concat(', '_list('] as $leaked) {
            $this->assertStringNotContainsString(
                'zephir_declare_class_constant' . $leaked,
                $c,
                'An unfolded operator node reached the constant emitter.'
            );
        }
    }

    private function generatedC(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2061;

final class Limits
{
    const INT8_MIN = -0x7f - 1;
    const MASK = 0xff << 8 | 0x0f;
    const RATIO = (1 + 4) / 2;
    const NAME = "a" . "b";
    const FLAG = 1 < 2;
    const SIZE = 1024 * 8;
    const DOUBLED = self::SIZE * 2;

    public size = 1024 * 8;
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2061/limits.zep', $zep);

        $factoryProp = (new ReflectionClass($this->compiler))->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        /** @var CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create(
            'Stub\\Issue2061\\Limits',
            'stub/issue2061/limits.zep'
        );
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        return file_get_contents($this->tempDir . '/ext/stub/issue2061/limits.zep.c');
    }
}
