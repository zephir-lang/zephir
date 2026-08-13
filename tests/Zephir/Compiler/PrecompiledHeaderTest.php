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

namespace Zephir\Test\Compiler;

use PHPUnit\Framework\TestCase;
use Psr\Log\Test\TestLogger;
use ReflectionMethod;
use Zephir\Backend\Backend;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * Covers the pre-compiled header prelude: every generated .zep.c opens with
 * php.h plus ext.h, and ext.h pulls in the header of every class in the
 * project, so that prelude is parsed once per translation unit.
 */
final class PrecompiledHeaderTest extends TestCase
{
    private string $originalCwd;
    private string $projectDir;

    protected function setUp(): void
    {
        $this->originalCwd = getcwd();
        $this->projectDir  = sys_get_temp_dir() . '/zephir-pch-' . uniqid('', true);
        mkdir($this->projectDir . '/ext/kernel', 0755, true);
        chdir($this->projectDir);
    }

    protected function tearDown(): void
    {
        putenv('ZEPHIR_NO_PCH');
        chdir($this->originalCwd);
        $this->removeRecursively($this->projectDir);
    }

    /**
     * A GCC pre-compiled header is only used when it is the first token of the
     * translation unit, so the prelude must start where a generated file
     * starts, in the same order.
     */
    public function testPreludeStartsWithTheHeaderBlockOfAGeneratedFile(): void
    {
        $source = $this->createCompiler()->precompiledHeaderSource([]);

        $this->assertStringContainsString('#include <php.h>', $source);
        $this->assertStringContainsString('#include "php_ext.h"', $source);
        $this->assertStringContainsString('#include "ext.h"', $source);
        $this->assertLessThan(
            strpos($source, '#include "php_ext.h"'),
            strpos($source, '#include <php.h>'),
            'php.h has to come before the extension headers, as it does in every generated file.'
        );
        $this->assertStringContainsString('#ifdef HAVE_CONFIG_H', $source);
    }

    public function testPreludeIncludesTheGivenKernelHeadersAfterPhp(): void
    {
        $source = $this->createCompiler()->precompiledHeaderSource(['main.h', 'memory.h']);

        $this->assertStringContainsString('#include "kernel/main.h"', $source);
        $this->assertStringContainsString('#include "kernel/memory.h"', $source);
        $this->assertLessThan(
            strpos($source, '#include "kernel/main.h"'),
            strpos($source, '#include <php.h>'),
            'Kernel headers are not self-contained: php.h has to be parsed first.'
        );
    }

    /**
     * The list of kernel headers is fixed, but a project only ships the ones
     * its Zephir version knows about; the others must not end up in an
     * #include that does not resolve.
     */
    public function testOnlyKernelHeadersPresentInTheProjectArePreCompiled(): void
    {
        touch($this->projectDir . '/ext/kernel/main.h');
        touch($this->projectDir . '/ext/kernel/memory.h');

        $headers = $this->invokeKernelHeaders($this->createCompiler());

        $this->assertSame(['main.h', 'memory.h'], $headers);
    }

    public function testNoKernelHeadersWhenTheProjectHasNotBeenGeneratedYet(): void
    {
        $this->assertSame([], $this->invokeKernelHeaders($this->createCompiler()));
    }

    public function testPreCompilingIsSkippedWhenDisabledByEnvironment(): void
    {
        putenv('ZEPHIR_NO_PCH=1');

        $this->assertNull($this->createCompiler()->preCompileHeaders());
    }

    /**
     * Nothing may be left behind that a later build would trust blindly.
     */
    public function testFailedPreCompilationLeavesNoStaleObject(): void
    {
        // ext/ holds no php_ext.h or ext.h, so gcc cannot build the prelude.
        $compiler = $this->createCompiler();

        $this->assertNull($compiler->preCompileHeaders());
        $this->assertFileDoesNotExist($this->projectDir . '/.zephir/' . Compiler::PCH_HEADER . '.gch');
    }

    /**
     * The path ends up in a Makefile recipe, which make expands and hands to a
     * shell unquoted. `Zephir::VERSION` carries an unexpanded `$Id$`, so the
     * per-version cache container is exactly the wrong place for it.
     *
     * The paths are POSIX literals rather than this test's own project dir: a
     * prelude only ever reaches a `make` recipe where it is pre-compiled, and
     * on Windows `sys_get_temp_dir()` hands back a backslashed path that the
     * rule under test rejects, correctly, for being unquotable.
     *
     * @see \Zephir\Test\FileSystem\HardDiskTest::testShouldReturnUnversionedPathBesideTheContainer
     *      Covers where the prelude lands.
     */
    public function testPreludeLivesOutsideThePerVersionContainer(): void
    {
        $this->assertTrue(
            Compiler::isShellSafePath('/home/dev/project/.zephir/' . Compiler::PCH_HEADER),
            'The prelude path has to survive a Makefile recipe.'
        );
        $this->assertFalse(
            Compiler::isShellSafePath('/home/dev/project/.zephir/1.2.0-$Id$/zephir_pch.h'),
            'A path with a make variable reference in it must be rejected.'
        );
    }

    /**
     * @dataProvider unsafePathProvider
     */
    public function testPathsAShellWouldChewOnAreRejected(string $path): void
    {
        $this->assertFalse(Compiler::isShellSafePath($path));
    }

    public static function unsafePathProvider(): array
    {
        return [
            'make variable' => ['/home/dev/.zephir/1.2.0-$Id$/zephir_pch.h'],
            'space'         => ['/home/my projects/ext/zephir_pch.h'],
            'quote'         => ['/home/dev\'s/zephir_pch.h'],
            'backtick'      => ['/home/`whoami`/zephir_pch.h'],
            'glob'          => ['/home/dev/*/zephir_pch.h'],
        ];
    }

    /**
     * A PCH is only accepted for a translation unit compiled with the same
     * flags, so they are read off the generated Makefile — whose INCLUDES and
     * CFLAGS are what its own recipes pass, already expanded — rather than
     * derived a second time and hoped to match.
     */
    public function testFlagsComeFromTheGeneratedMakefile(): void
    {
        file_put_contents($this->projectDir . '/ext/Makefile', implode("\n", [
            'CFLAGS = -O2 -fvisibility=hidden -DZEPHIR_RELEASE=1',
            'CFLAGS_CLEAN = $(CFLAGS) -D_GNU_SOURCE',
            'INCLUDES = -I/opt/php83/include/php -I/opt/php83/include/php/main',
        ]) . "\n");

        $flags = $this->invokeFlags($this->createCompiler());

        $this->assertStringContainsString('-I/opt/php83/include/php/main', $flags);
        $this->assertStringContainsString('-O2 -fvisibility=hidden -DZEPHIR_RELEASE=1', $flags);
        // libtool's, and the recipe's own, which are not Makefile variables.
        $this->assertStringContainsString('-fPIC', $flags);
        $this->assertStringContainsString('-DZEND_COMPILE_DL_EXT=1', $flags);
    }

    public function testMakefileVariableIsReadWhenPlain(): void
    {
        $this->assertSame(
            '-I/usr/local/include/php -I/usr/local/include/php/main',
            Compiler::parseMakefileVariable(
                "CC = gcc\nINCLUDES = -I/usr/local/include/php -I/usr/local/include/php/main\n",
                'INCLUDES'
            )
        );
    }

    /**
     * `CFLAGS_CLEAN = $(CFLAGS) -D_GNU_SOURCE` must not be mistaken for CFLAGS,
     * and a value still holding a reference is refused rather than passed on
     * unexpanded.
     */
    public function testMakefileVariableIgnoresLookalikesAndUnexpandedValues(): void
    {
        $makefile = "CFLAGS_CLEAN = \$(CFLAGS) -D_GNU_SOURCE\nCFLAGS = -O2\n";

        $this->assertSame('-O2', Compiler::parseMakefileVariable($makefile, 'CFLAGS'));
        $this->assertNull(Compiler::parseMakefileVariable($makefile, 'CFLAGS_CLEAN'));
        $this->assertNull(Compiler::parseMakefileVariable("INCLUDES =\n", 'INCLUDES'));
        $this->assertNull(Compiler::parseMakefileVariable("CC = gcc\n", 'INCLUDES'));
    }

    private function invokeFlags(Compiler $compiler): string
    {
        $method = new ReflectionMethod(Compiler::class, 'precompiledHeaderFlags');
        $method->setAccessible(true);

        return $method->invoke($compiler, false, $this->projectDir . '/ext', \Zephir\PhpToolchain::default());
    }

    /**
     * @return string[]
     */
    private function invokeKernelHeaders(Compiler $compiler): array
    {
        $method = new ReflectionMethod(Compiler::class, 'precompiledKernelHeaders');
        $method->setAccessible(true);

        return $method->invoke($compiler, $this->projectDir . '/ext');
    }

    private function createCompiler(): Compiler
    {
        $logger = new TestLogger();
        $config = new Config();
        $config->set('namespace', 'test');
        $backend         = new Backend($config, 'kernel', 'templates');
        $disk            = new HardDisk($this->projectDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');
        $compilerFactory = new CompilerFileFactory($config, $disk, $logger);

        $compiler = new Compiler($config, $backend, new Manager(new Parser()), $disk, $compilerFactory);
        $compiler->setLogger($logger);

        return $compiler;
    }

    private function removeRecursively(string $path): void
    {
        if (!is_dir($path)) {
            return;
        }

        foreach (scandir($path) as $entry) {
            if ('.' === $entry || '..' === $entry) {
                continue;
            }
            $full = $path . DIRECTORY_SEPARATOR . $entry;
            is_dir($full) ? $this->removeRecursively($full) : @unlink($full);
        }

        @rmdir($path);
    }
}
