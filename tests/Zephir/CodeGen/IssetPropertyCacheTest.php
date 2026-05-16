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
 * Verifies that isset(obj->staticProp) emits a per-call-site interned
 * zend_string cache rather than a fresh zend_string_init per call.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2385
 */
final class IssetPropertyCacheTest extends TestCase
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

        $this->tempDir = sys_get_temp_dir() . '/zephir_isset_cache_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2385', 0755, true);
        mkdir($this->tempDir . '/stub/issue2385', 0755, true);

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

    public function testStaticPropertyIssetEmitsCachedZendString(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2385;

class Cache
{
    public foo;

    public function check() -> bool
    {
        return isset(this->foo);
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2385/cache.zep', $zep);

        $c = $this->compileZep(
            'Stub\\Issue2385\\Cache',
            'stub/issue2385/cache.zep',
            'stub/issue2385/cache'
        );

        $body = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2385_Cache, check)');
        $this->assertNotSame('', $body, 'Could not locate check() body.');

        $this->assertMatchesRegularExpression(
            '/static\s+zend_string\s*\*\s*\w+\s*=\s*NULL/',
            $body,
            "Expected a per-call-site static zend_string * cache slot.\n$body"
        );

        $this->assertStringContainsString(
            'zephir_isset_property_value_fast(',
            $body,
            "Expected the cache-aware fast helper to be called.\n$body"
        );

        // The old per-call zend_string_init path must NOT appear for the
        // static-name isset — the cached fast helper supersedes it.
        $this->assertStringNotContainsString(
            'zephir_isset_property_value(&this_ptr',
            $body,
            "Expected the cache helper to replace the alloc-per-call helper.\n$body"
        );
    }

    public function testRepeatedAccessReusesSameSlot(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2385;

class CacheReuse
{
    public foo;

    public function checkTwice() -> bool
    {
        if isset(this->foo) {
            if isset(this->foo) {
                return true;
            }
        }
        return false;
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2385/cachereuse.zep', $zep);

        $c = $this->compileZep(
            'Stub\\Issue2385\\CacheReuse',
            'stub/issue2385/cachereuse.zep',
            'stub/issue2385/cachereuse'
        );

        $body = $this->methodBody($c, 'PHP_METHOD(Stub_Issue2385_CacheReuse, checkTwice)');
        $this->assertNotSame('', $body, 'Could not locate checkTwice() body.');

        // Exactly one cache slot should be emitted for the same property
        // checked twice within a single method body.
        $count = preg_match_all('/static\s+zend_string\s*\*/', $body);
        $this->assertSame(
            1,
            $count,
            "Expected exactly one static zend_string * slot for repeated isset of same property; got $count.\n$body"
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

    private function methodBody(string $haystack, string $signature): string
    {
        $startPos = strpos($haystack, $signature);
        if ($startPos === false) {
            return '';
        }
        $braceStart = strpos($haystack, '{', $startPos);
        if ($braceStart === false) {
            return '';
        }
        $depth = 1;
        $i = $braceStart + 1;
        while ($i < strlen($haystack) && $depth > 0) {
            if ($haystack[$i] === '{') {
                $depth++;
            } elseif ($haystack[$i] === '}') {
                $depth--;
            }
            $i++;
        }
        return substr($haystack, $startPos, $i - $startPos);
    }
}
