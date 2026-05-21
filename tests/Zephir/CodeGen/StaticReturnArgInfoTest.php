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
 * Locks the arginfo macro chosen for each of PHP's three reserved
 * return-type keywords. Until PR #2537, `-> <static>` was emitted with
 * the same `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX` macro as `<self>` /
 * `<parent>`, with the literal lowercase string `static` substituted
 * for the class name. PHP would then try to resolve `static` as a
 * class during MINIT (when a subclass overrides the method), and abort
 * with "static must be registered before <Class>". The fix routes
 * `<static>` through `ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX` with
 * the `MAY_BE_STATIC` mask bit instead.
 *
 * `<self>` and `<parent>` continue to use the OBJ_INFO macro because
 * PHP's class-name resolver recognizes both as reserved keywords.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
final class StaticReturnArgInfoTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_arginfo_static_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2537', 0755, true);
        mkdir($this->tempDir . '/stub/issue2537', 0755, true);

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

    public function testStaticReturnUsesTypeMaskWithMayBeStatic(): void
    {
        $h = $this->compileSingle('Returns', 'returns.zep', <<<'ZEP'
namespace Stub\Issue2537;

class Returns
{
    public function make() -> <static>
    {
        return this;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537_returns_make, 0, 0, MAY_BE_STATIC)',
            $h,
            'A non-nullable `-> <static>` must emit the MAY_BE_STATIC mask.'
        );

        $this->assertStringNotContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2537_returns_make, 0, 0, static',
            $h,
            'The literal `static` class-name form is the bug PR #2537 fixes; it must not regress.'
        );
    }

    public function testNullableStaticReturnOrsInMayBeNull(): void
    {
        $h = $this->compileSingle('ReturnsNull', 'returnsnull.zep', <<<'ZEP'
namespace Stub\Issue2537;

class ReturnsNull
{
    public function make() -> <static> | null
    {
        return this;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537_returnsnull_make, 0, 0, MAY_BE_NULL|MAY_BE_STATIC)',
            $h,
            'Nullable `-> <static> | null` must OR MAY_BE_NULL into the mask.'
        );
    }

    public function testSelfReturnKeepsObjInfoMacroWithReservedKeyword(): void
    {
        $h = $this->compileSingle('ReturnsSelf', 'returnsself.zep', <<<'ZEP'
namespace Stub\Issue2537;

class ReturnsSelf
{
    public function make() -> <self>
    {
        return this;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2537_returnsself_make, 0, 0, self, 0)',
            $h,
            '`<self>` must reach the engine as the reserved lowercase keyword via the OBJ_INFO macro.'
        );

        $this->assertStringNotContainsString(
            'MAY_BE_STATIC',
            $h,
            '`<self>` must not be conflated with the `<static>` mask path.'
        );
    }

    public function testParentReturnKeepsObjInfoMacroWithReservedKeyword(): void
    {
        $h = $this->compileSingle('ReturnsParent', 'returnsparent.zep', <<<'ZEP'
namespace Stub\Issue2537;

class ReturnsParent
{
    public function make() -> <parent>
    {
        return this;
    }
}
ZEP);

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2537_returnsparent_make, 0, 0, parent, 0)',
            $h,
            '`<parent>` must reach the engine as the reserved lowercase keyword via the OBJ_INFO macro.'
        );
    }

    public function testNonReservedClassReturnStillEscapedAndPrefixed(): void
    {
        $h = $this->compileSingle('ReturnsCustom', 'returnscustom.zep', <<<'ZEP'
namespace Stub\Issue2537;

class ReturnsCustom
{
    public function make() -> <ReturnsCustom>
    {
        return this;
    }
}
ZEP);

        $this->assertMatchesRegularExpression(
            '/ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX\(arginfo_stub_issue2537_returnscustom_make, 0, 0, Stub\\\\\\\\Issue2537\\\\\\\\ReturnsCustom, 0\)/',
            $h,
            'Ordinary class return types must keep the namespace-prefixed + backslash-escaped form. The reserved-keyword path must not capture them.'
        );

        $this->assertStringNotContainsString(
            'MAY_BE_STATIC',
            $h,
            'Custom class return types must not be routed through the type-mask path.'
        );
    }

    private function compileSingle(string $simpleClassName, string $fileName, string $zep): string
    {
        $extDir = $this->tempDir . '/ext/stub/issue2537';
        if (!is_dir($extDir)) {
            mkdir($extDir, 0755, true);
        }

        $srcRel = 'stub/issue2537/' . $fileName;
        $headerRel = 'stub/issue2537/' . pathinfo($fileName, PATHINFO_FILENAME);

        file_put_contents($this->tempDir . '/' . $srcRel, $zep);

        $factoryProp = (new \ReflectionClass($this->compiler))->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        $fqcn = 'Stub\\Issue2537\\' . $simpleClassName;
        /** @var \Zephir\CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create($fqcn, $srcRel);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        $headerPath = $this->tempDir . '/ext/' . $headerRel . '.zep.h';
        $this->assertFileExists($headerPath, 'Compiler should emit a header file.');

        return file_get_contents($headerPath);
    }
}
