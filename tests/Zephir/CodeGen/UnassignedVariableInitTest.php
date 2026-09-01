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
 * A local the user declared without a value and never assigned has to start at
 * IS_NULL, because PHP evaluates an unset variable as null while an IS_UNDEF
 * zval reaches userland as `UNKNOWN:0`.
 *
 * The guard tests matter as much as the positive ones: IS_UNDEF at declaration
 * is what makes ZEPHIR_INIT_NVAR / ZEPHIR_CPY_WRT / ZEPHIR_OBS_NVAR register a
 * zval with the memory frame, so anything that is written must keep it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2654
 */
final class UnassignedVariableInitTest extends TestCase
{
    private string $originalCwd;
    private string $tempDir;
    private Compiler $compiler;
    private CompilerFileFactory $compilerFileFactory;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();

        $this->tempDir = sys_get_temp_dir() . '/zephir_issue2654_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2654', 0755, true);
        mkdir($this->tempDir . '/stub/issue2654', 0755, true);

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

        $this->compilerFileFactory = new CompilerFileFactory($config, $disk, $logger);

        $this->compiler = new Compiler(
            $config,
            $backend,
            new Manager(new Parser()),
            $disk,
            $this->compilerFileFactory
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

    /**
     * @return array<string, array{0: string, 1: string}>
     */
    public static function unassignedProvider(): array
    {
        return [
            'var'    => ['readVar', 'x'],
            'string' => ['readString', 's'],
            'array'  => ['readArray', 'a'],
        ];
    }

    /**
     * @dataProvider unassignedProvider
     */
    public function testUnassignedDeclarationStartsAsNull(string $method, string $name): void
    {
        $body = $this->methodBody($this->compileFixture(), $method);

        $this->assertStringContainsString(
            'ZVAL_NULL(&' . $name . ');',
            $body,
            sprintf('%s() must start "%s" at IS_NULL, not IS_UNDEF.', $method, $name)
        );
        $this->assertStringNotContainsString(
            'ZVAL_UNDEF(&' . $name . ');',
            $body,
            sprintf('%s() still leaves "%s" uninitialized.', $method, $name)
        );
    }

    public function testAssignedVariableKeepsUndef(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'writeVar');

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&y);',
            $body,
            'An assigned local must keep IS_UNDEF so its first write observes it.'
        );
    }

    public function testTemporalVariableKeepsUndef(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'temporal');

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&_0);',
            $body,
            'Compiler temporaries must keep IS_UNDEF: zephir_cast_to_string() and '
            . 'friends observe them only while they are undefined.'
        );
    }

    public function testOptionalParamSubstituteKeepsUndef(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'optionalParam');

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&b_sub);',
            $body,
            'The <param>_sub companion must keep IS_UNDEF: ZEPHIR_CPY_WRT observes '
            . 'through it only while it is undefined, so IS_NULL would leak.'
        );
    }

    public function testNativeStringParamCompanionKeepsUndef(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'stringParam');

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&t_zv);',
            $body,
            'The <param>_zv companion must keep IS_UNDEF.'
        );
    }

    /**
     * zephir_preg_match() does `zval_dtor(matches); ZVAL_COPY(matches, ...)`
     * without observing, so `matches` has to stay undefined.
     */
    public function testPregMatchOutParameterKeepsUndef(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'pregMatch');

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&matches);',
            $body,
            'An out-parameter is written by the callee, so it must keep IS_UNDEF.'
        );
    }

    public function testNativeIntDeclarationKeepsZeroDefault(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'readInt');

        $this->assertStringContainsString(
            'zend_long i = 0;',
            $body,
            'Native locals keep their existing type-zero default.'
        );
    }

    private function compileFixture(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2654;

class Plain
{
    public function readVar()
    {
        var x;

        return x;
    }

    public function readString()
    {
        string s;

        return s;
    }

    public function readArray()
    {
        array a;

        return a;
    }

    public function readInt() -> int
    {
        int i;

        return i;
    }

    public function writeVar()
    {
        var y;

        let y = "assigned";

        return y;
    }

    public function identity(var value)
    {
        return value;
    }

    public function temporal(var value) -> array
    {
        var out;

        let out = [];
        let out[] = this->identity(value);

        return out;
    }

    public function optionalParam(var b = null)
    {
        let b = "prefix" . b;

        return b;
    }

    public function stringParam(string t) -> string
    {
        return t;
    }

    public function pregMatch(string subject) -> array
    {
        var matches;

        preg_match("/[a-z]+/", subject, matches);

        return matches;
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2654/plain.zep', $zep);

        $compilerFile = $this->compilerFileFactory->create(
            'Stub\\Issue2654\\Plain',
            'stub/issue2654/plain.zep'
        );
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        return file_get_contents($this->tempDir . '/ext/stub/issue2654/plain.zep.c');
    }

    /**
     * Returns the C body of a single generated method.
     */
    private function methodBody(string $generated, string $method): string
    {
        $marker   = 'PHP_METHOD(Stub_Issue2654_Plain, ' . $method . ')';
        $startPos = strpos($generated, $marker);

        $this->assertNotFalse($startPos, 'Could not locate ' . $marker . ' in the generated C.');

        $endPos = strpos($generated, 'PHP_METHOD(', $startPos + strlen($marker));

        return false === $endPos
            ? substr($generated, $startPos)
            : substr($generated, $startPos, $endPos - $startPos);
    }
}
