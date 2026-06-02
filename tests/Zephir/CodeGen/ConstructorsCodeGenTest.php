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
use Zephir\Os;
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
    private string $argsSingleFixturesDir;
    private Compiler $compiler;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd  = getcwd();
        // Resolve the fixtures path to absolute BEFORE any chdir(), so reads
        // work correctly even after we switch the working directory.
        $this->fixturesDir  = realpath(FIXTURES_PATH . '/constructors');
        $this->argsSingleFixturesDir = realpath(FIXTURES_PATH . '/constructors/args/single');

        // Create an isolated temp working directory so that all relative
        // paths used by CompilerFile (e.g. "ext/stub/constructors/…") land
        // inside it and do not touch the real project tree.
        $this->tempDir = sys_get_temp_dir() . '/zephir_codegen_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/constructors', 0755, true);
        mkdir($this->tempDir . '/ext/stub/args/single', 0755, true);
        mkdir($this->tempDir . '/stub/constructors', 0755, true);
        mkdir($this->tempDir . '/stub/args/single', 0755, true);

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

        // Write Zephir source for Stub\Args\Single\Integer
        $integerZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class Integer
{
    public function argInt(int i) -> int
    {
        return i;
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/integer.zep',
            $integerZepCode
        );

        // Write Zephir source for Stub\Args\Single\Str
        $strZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class Str
{
    public function argString(string str) -> string
    {
        return str;
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/str.zep',
            $strZepCode
        );

        // Write Zephir source for Stub\Args\Single\StrOptional
        $strOptionalZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class StrOptional
{
    public function argStringDefault(string param = "test string") -> string
    {
        return param;
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/stroptional.zep',
            $strOptionalZepCode
        );

        // Write Zephir source for Stub\Args\Single\StrNullable
        $strNullableZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class StrNullable
{
    public function argStringNull(string param = null) -> string | null
    {
        return param;
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/strnullable.zep',
            $strNullableZepCode
        );

        // Write Zephir source for Stub\Args\Single\StrMixed
        $strMixedZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class StrMixed
{
    public function argStringAndInt(string str, int position) -> string
    {
        return str;
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/strmixed.zep',
            $strMixedZepCode
        );

        // Write Zephir source for Stub\Args\Single\ObjNullable
        // A parameter that has a class type and defaults to null must keep
        // its default value readable through reflection. The parameter type
        // points at the class itself so the generated class entry stays the
        // same in any environment.
        // @see https://github.com/zephir-lang/zephir/issues/2564
        $objNullableZepCode = <<<'ZEP'
namespace Stub\Args\Single;

class ObjNullable
{
    public function argObjNull(<\Stub\Args\Single\ObjNullable> param = null) -> void
    {
    }
}

ZEP;
        file_put_contents(
            $this->tempDir . '/stub/args/single/objnullable.zep',
            $objNullableZepCode
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
     * Compiles a single .zep file and returns a [cOutput, hOutput] pair
     * with the raw generated file contents.
     *
     * @param  string $className  Fully-qualified Zephir class name (e.g. "Stub\Args\Single\Integer")
     * @param  string $filePath   Relative path to the .zep source (e.g. "stub/args/single/integer.zep")
     * @param  string $extPath    Relative path (without extension) to the generated output under ext/
     *                            (e.g. "stub/args/single/integer")
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileZep(string $className, string $filePath, string $extPath): array
    {
        $factory      = new \ReflectionClass($this->compiler);
        $factoryProp  = $factory->getProperty('compilerFileFactory');
        $factoryProp->setAccessible(true);
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $factoryProp->getValue($this->compiler);

        /** @var \Zephir\CompilerFile $compilerFile */
        $compilerFile = $compilerFileFactory->create($className, $filePath);
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        $cFile = $this->tempDir . '/ext/' . $extPath . '.zep.c';
        $hFile = $this->tempDir . '/ext/' . $extPath . '.zep.h';

        return [
            file_get_contents($cFile),
            file_get_contents($hFile),
        ];
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
        return $this->compileZep(
            'Stub\Constructors\Issue1803',
            'stub/constructors/issue1803.zep',
            'stub/constructors/issue1803'
        );
    }

    /**
     * Compiles stub/args/single/integer.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleInteger(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\Integer',
            'stub/args/single/integer.zep',
            'stub/args/single/integer'
        );
    }

    /**
     * Compiles stub/args/single/str.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleStr(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\Str',
            'stub/args/single/str.zep',
            'stub/args/single/str'
        );
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

    /**
     * The generated .c file for Args\Single\Integer must be 100% identical to the reference fixture.
     */
    public function testArgsSingleIntegerGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleInteger();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/integer.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated integer.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\Integer must be 100% identical to the reference fixture.
     */
    public function testArgsSingleIntegerGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleInteger();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/integer.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated integer.zep.h does not match the reference fixture.'
        );
    }

    /**
     * The generated .c file for Args\Single\Str must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleStr();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated str.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\Str must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleStr();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated str.zep.h does not match the reference fixture.'
        );
    }

    /**
     * Compiles stub/args/single/stroptional.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleStrOptional(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\StrOptional',
            'stub/args/single/stroptional.zep',
            'stub/args/single/stroptional'
        );
    }

    /**
     * The generated .c file for Args\Single\StrOptional must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrOptionalGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleStrOptional();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_optional.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated str_optional.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\StrOptional must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrOptionalGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleStrOptional();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_optional.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated str_optional.zep.h does not match the reference fixture.'
        );
    }

    /**
     * Compiles stub/args/single/strnullable.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleStrNullable(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\StrNullable',
            'stub/args/single/strnullable.zep',
            'stub/args/single/strnullable'
        );
    }

    /**
     * The generated .c file for Args\Single\StrNullable must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrNullableGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleStrNullable();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_nullable.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated str_nullable.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\StrNullable must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrNullableGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleStrNullable();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_nullable.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated str_nullable.zep.h does not match the reference fixture.'
        );
    }

    /**
     * Compiles stub/args/single/strmixed.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleStrMixed(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\StrMixed',
            'stub/args/single/strmixed.zep',
            'stub/args/single/strmixed'
        );
    }

    /**
     * The generated .c file for Args\Single\StrMixed must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrMixedGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleStrMixed();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_mixed.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated str_mixed.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\StrMixed must be 100% identical to the reference fixture.
     */
    public function testArgsSingleStrMixedGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleStrMixed();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/str_mixed.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated str_mixed.zep.h does not match the reference fixture.'
        );
    }

    /**
     * Compiles stub/args/single/objnullable.zep and returns a
     * [cOutput, hOutput] pair with the raw generated file contents.
     *
     * @return array{0: string, 1: string}
     * @throws \ReflectionException
     * @throws Exception
     */
    private function compileArgsSingleObjNullable(): array
    {
        return $this->compileZep(
            'Stub\Args\Single\ObjNullable',
            'stub/args/single/objnullable.zep',
            'stub/args/single/objnullable'
        );
    }

    /**
     * The generated .c file for Args\Single\ObjNullable must be 100% identical to the reference fixture.
     */
    public function testArgsSingleObjNullableGeneratedCFileIsIdenticalToFixture(): void
    {
        [$cOutput,] = $this->compileArgsSingleObjNullable();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/obj_nullable.zep.c');

        $this->assertSame(
            $fixture,
            $cOutput,
            'Generated obj_nullable.zep.c does not match the reference fixture.'
        );
    }

    /**
     * The generated .h file for Args\Single\ObjNullable must be 100% identical to the reference fixture.
     *
     * Checks that a parameter with a class type and a `null` default uses
     * ZEND_ARG_OBJ_TYPE_MASK, which keeps a default value, instead of
     * ZEND_ARG_OBJ_INFO, which does not.
     */
    public function testArgsSingleObjNullableGeneratedHFileIsIdenticalToFixture(): void
    {
        [, $hOutput] = $this->compileArgsSingleObjNullable();

        $fixture = file_get_contents($this->argsSingleFixturesDir . '/obj_nullable.zep.h');

        $this->assertSame(
            $fixture,
            $hOutput,
            'Generated obj_nullable.zep.h does not match the reference fixture.'
        );
    }
}
