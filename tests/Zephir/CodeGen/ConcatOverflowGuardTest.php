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
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use Zephir\Backend\StringsManager;
use Zephir\Os;

/**
 * The generated `zephir_concat_*()` helpers must not sum operand lengths without
 * checking for a `size_t` wrap, the way `concat_function()` does in
 * `Zend/zend_operators.c`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2657
 */
final class ConcatOverflowGuardTest extends TestCase
{
    /**
     * Operand count per key, including the three keys `StringsManager` always emits.
     */
    private const KEYS = [
        'sv'   => 2,
        'vs'   => 2,
        'vv'   => 2,
        'svvs' => 4,
    ];

    private string $originalCwd;
    private string $tempDir;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();
        $this->tempDir     = sys_get_temp_dir() . '/zephir_concat_guard_' . uniqid('', true);

        mkdir($this->tempDir . '/ext/kernel', 0755, true);
        chdir($this->tempDir);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);

        if (!is_dir($this->tempDir)) {
            return;
        }

        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($this->tempDir, FilesystemIterator::SKIP_DOTS),
            RecursiveIteratorIterator::CHILD_FIRST
        );
        foreach ($iterator as $file) {
            $file->isDir() ? rmdir($file->getRealPath()) : unlink($file->getRealPath());
        }
        rmdir($this->tempDir);
    }

    public function testNoHelperSumsOperandLengthsUnchecked(): void
    {
        $code = $this->generate();

        $this->assertSame(
            0,
            preg_match_all('/^\tlength = .+ \+ /m', $code),
            'The result length must be accumulated one guarded operand at a time, '
            . 'not summed in a single unchecked expression.'
        );
    }

    public function testEveryOperandAndTheSelfOffsetIsGuarded(): void
    {
        $code = $this->generate();

        foreach (self::KEYS as $key => $operands) {
            $body = $this->helperBody($code, $key);

            // The first operand seeds `length`, so it needs no guard: one guard per
            // remaining operand, plus one for the `self_var` offset.
            $this->assertSame(
                $operands,
                substr_count($body, 'goto zephir_concat_overflow;'),
                "zephir_concat_{$key}() must guard each accumulation and the offset."
            );
        }
    }

    public function testGuardsUseTheOperandItIsAboutToAdd(): void
    {
        $body = $this->helperBody($this->generate(), 'svvs');

        $this->assertStringContainsString(
            'if (UNEXPECTED(Z_STRLEN_P(op2) > ZSTR_MAX_LEN - length)) {',
            $body,
            'A zval operand must be checked with its own Z_STRLEN_P().'
        );
        $this->assertStringContainsString(
            'if (UNEXPECTED(op4_len > ZSTR_MAX_LEN - length)) {',
            $body,
            'A string operand must be checked with its own length argument.'
        );
    }

    public function testOffsetIsGuardedBeforeTheReallocation(): void
    {
        $body = $this->helperBody($this->generate(), 'vv');

        $guard   = strpos($body, 'if (UNEXPECTED(offset > ZSTR_MAX_LEN - length)) {');
        $realloc = strpos($body, 'zend_string_realloc(');

        $this->assertNotFalse($guard, 'The self_var offset must be guarded.');
        $this->assertLessThan(
            $realloc,
            $guard,
            'The offset guard must run before the reallocation it protects.'
        );
    }

    public function testOverflowThrowsTheSameErrorAsPhp(): void
    {
        $body = $this->helperBody($this->generate(), 'vv');

        $this->assertSame(
            1,
            substr_count($body, 'zend_throw_error(NULL, "String size overflow");'),
            'Overflow must raise the same catchable Error as userland concatenation.'
        );
        $this->assertStringContainsString(
            'ZVAL_UNDEF(result);',
            $body,
            'A non self_var result must be left undefined, as concat_function() does.'
        );
    }

    public function testOverflowRunsTheExistingOperandCleanup(): void
    {
        $body = $this->helperBody($this->generate(), 'vv');

        $overflow = strpos($body, PHP_EOL . 'zephir_concat_overflow:' . PHP_EOL);
        $cleanup  = strpos($body, PHP_EOL . 'zephir_concat_cleanup:' . PHP_EOL);
        $dtor     = strpos($body, 'zval_dtor(op1);');

        $this->assertNotFalse($overflow, 'The overflow tail needs its own label.');
        $this->assertNotFalse($cleanup, 'The shared cleanup tail needs its own label.');
        $this->assertLessThan($cleanup, $overflow, 'The throw must fall through to the cleanup.');
        $this->assertLessThan(
            $dtor,
            $cleanup,
            'The printable-zval copies must still be released on the overflow path.'
        );
    }

    public function testGuardsAreBoundedByZstrMaxLen(): void
    {
        $code = $this->generate();

        $this->assertStringNotContainsString(
            'SIZE_MAX',
            $code,
            'ZSTR_MAX_LEN, not SIZE_MAX, accounts for the zend_string header overhead.'
        );
    }

    private function generate(): string
    {
        $manager = new StringsManager();
        foreach (self::KEYS as $key => $operands) {
            $manager->addConcatKey($key);
        }
        $manager->genConcatCode();

        return file_get_contents($this->tempDir . '/ext/kernel/concat.c');
    }

    private function helperBody(string $code, string $key): string
    {
        $start = strpos($code, "void zephir_concat_{$key}(");
        $this->assertNotFalse($start, "zephir_concat_{$key}() was not generated.");

        $end = strpos($code, PHP_EOL . '}' . PHP_EOL, $start);
        $this->assertNotFalse($end, "zephir_concat_{$key}() has no closing brace.");

        return substr($code, $start, $end - $start);
    }
}
