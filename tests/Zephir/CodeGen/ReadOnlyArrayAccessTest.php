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
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * The read-only flag on a subscript read means "borrowed, the container owns
 * the value". An ArrayAccess object owns nothing after offsetGet() returns, so
 * the flag may only be emitted for a container that provably holds a native
 * array.
 *
 * These assertions run on the generated C, so they hold without building the
 * extension, and the last one is what keeps the fix from being "turn the
 * optimization off everywhere".
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class ReadOnlyArrayAccessTest extends TestCase
{
    private Compiler $compiler;
    private string   $originalCwd;
    private string   $tempDir;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();

        $this->tempDir = sys_get_temp_dir() . '/zephir_readonly_dim_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2682', 0755, true);
        mkdir($this->tempDir . '/stub/issue2682', 0755, true);

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

    /**
     * `fetch v, c["k"]` on an untyped container. The throw is what moves the
     * last call line ahead of the read, which is what used to promote the
     * target to read-only.
     */
    public function testFetchFromAnUntypedContainerIsNotReadOnly(): void
    {
        $body = $this->compileMethod('FetchDim', 'fetchdim', <<<'ZEP'
    public function read(container)
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        if fetch value, container["namespace"] {
            return value;
        }

        return null;
    }
ZEP);

        $this->assertStringContainsString('zephir_array_isset_string_fetch(', $body);
        $this->assertMatchesRegularExpression(
            '/zephir_array_isset_string_fetch\([^;]*,\s*0\)/',
            $body,
            "The fetch must ask for an owned value from a container that may be an object.\n$body"
        );
    }

    /**
     * `let x = c["k"]` on an untyped container, kernel/array.c
     * zephir_array_fetch_string().
     */
    public function testReadFromAnUntypedContainerIsNotReadOnly(): void
    {
        $body = $this->compileMethod('Read', 'read', <<<'ZEP'
    public function read(container)
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let value = container["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString('zephir_array_fetch_string(', $body);
        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "A container that may be an ArrayAccess object must not borrow.\n$body"
        );
    }

    /**
     * The optimization has to survive where it is valid. `table` is a `var`,
     * so a gate on the declared type would lose this; it is provable because
     * every assignment to it is an array literal.
     */
    public function testReadFromAProvenNativeArrayStaysReadOnly(): void
    {
        $body = $this->compileMethod('Native', 'native', <<<'ZEP'
    public function read(int mode)
    {
        var table, value;

        if mode < 0 {
            throw new \RuntimeException("Not reachable, it only moves the last call line");
        }

        let table = ["namespace": "kept"];
        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString(
            'PH_READONLY',
            $body,
            "A container assigned only from an array literal must keep the shortcut.\n$body"
        );
    }

    /**
     * An array parameter is enforced by PHP itself, so it is provable too.
     */
    public function testReadFromAnArrayParameterStaysReadOnly(): void
    {
        $body = $this->compileMethod('Param', 'param', <<<'ZEP'
    public function read(array table, int mode)
    {
        var value;

        if mode < 0 {
            throw new \RuntimeException("Not reachable, it only moves the last call line");
        }

        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringContainsString(
            'PH_READONLY',
            $body,
            "An array parameter cannot hold an object, so the shortcut is valid.\n$body"
        );
    }

    /**
     * The hole a declared-type gate would leave: `array` is a compile-time
     * label, and `let table = container` copies whatever the source holds.
     */
    public function testReadFromAnArrayLocalAliasedFromAVarIsNotReadOnly(): void
    {
        $body = $this->compileMethod('Aliased', 'aliased', <<<'ZEP'
    public function read(container)
    {
        array table;
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let table = container;
        let value = table["namespace"];

        return value;
    }
ZEP);

        $this->assertStringNotContainsString(
            'PH_READONLY',
            $body,
            "An array local assigned from a var is not proof of anything.\n$body"
        );
    }

    private function compileMethod(string $className, string $fileName, string $method): string
    {
        $zep = "namespace Stub\\Issue2682;\n\nclass $className\n{\n$method\n}\n";

        file_put_contents($this->tempDir . "/stub/issue2682/$fileName.zep", $zep);

        $factory = new ReflectionClass($this->compiler);
        $prop    = $factory->getProperty('compilerFileFactory');
        if (PHP_VERSION_ID < 80100) {
            $prop->setAccessible(true);
        }
        /** @var CompilerFileFactory $compilerFileFactory */
        $compilerFileFactory = $prop->getValue($this->compiler);

        $compilerFile = $compilerFileFactory->create(
            "Stub\\Issue2682\\$className",
            "stub/issue2682/$fileName.zep"
        );
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        $c    = file_get_contents($this->tempDir . "/ext/stub/issue2682/$fileName.zep.c");
        $body = $this->methodBody($c, "PHP_METHOD(Stub_Issue2682_$className, read)");

        $this->assertNotSame('', $body, "Could not locate read() in the generated C.\n$c");

        return $body;
    }

    private function methodBody(string $haystack, string $signature): string
    {
        $startPos = strpos($haystack, $signature);
        if (false === $startPos) {
            return '';
        }

        $braceStart = strpos($haystack, '{', $startPos);
        if (false === $braceStart) {
            return '';
        }

        $depth = 1;
        $i     = $braceStart + 1;
        while ($i < strlen($haystack) && $depth > 0) {
            if ('{' === $haystack[$i]) {
                ++$depth;
            } elseif ('}' === $haystack[$i]) {
                --$depth;
            }
            ++$i;
        }

        return substr($haystack, $startPos, $i - $startPos);
    }
}
