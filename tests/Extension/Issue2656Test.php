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

namespace Extension;

use Issue2656OffsetRecorder;
use Issue2656RefcountedExists;
use PHPUnit\Framework\TestCase;
use Stub\Issue2656;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2656
 */
final class Issue2656Test extends TestCase
{
    private Issue2656 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2656();
    }

    /**
     * The reported bug. kernel/array.c released the offset string between
     * offsetExists() and offsetGet(), and the method-name string allocated for
     * the offsetGet() call reused the freed slot, so the object was handed
     * "offsetget" as its key.
     */
    public function testFetchWithLiteralKeyHandsOffsetGetTheRealKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $result = $this->subject->fetchLiteral($container);

        $this->assertSame(1, $container->existsCount);
        $this->assertSame(['cache.key'], $container->seen);
        $this->assertSame('hit', $result);
    }

    /**
     * empty() reaches the same helper through zephir_isempty_dim_string().
     */
    public function testEmptyWithLiteralKeyHandsOffsetGetTheRealKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $result = $this->subject->emptyLiteral($container);

        $this->assertSame(['cache.key'], $container->seen);
        $this->assertFalse($result);
    }

    public function testFetchWithLiteralKeyReportsAMissingKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $this->assertFalse($this->subject->fetchLiteralMissing($container));
        $this->assertSame(1, $container->existsCount);
        $this->assertSame([], $container->seen, 'offsetGet() must not run for a key offsetExists() denied.');
    }

    public function testIssetWithLiteralKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $this->assertTrue($this->subject->issetLiteral($container));
        $this->assertSame(1, $container->existsCount);
    }

    public function testFetchWithLongKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $this->assertSame('seven', $this->subject->fetchLong($container));
        $this->assertSame([7], $container->seen);
    }

    public function testIssetWithLongKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $this->assertTrue($this->subject->issetLong($container));
        $this->assertSame(1, $container->existsCount);
    }

    /**
     * The zval-index helper forwards the caller's offset instead of building
     * one, so it was never affected. Kept as the control.
     */
    public function testFetchWithDynamicKey(): void
    {
        $container = new Issue2656OffsetRecorder();

        $this->assertSame('hit', $this->subject->fetchDynamic($container, 'cache.key'));
        $this->assertSame(['cache.key'], $container->seen);
        $this->assertTrue($this->subject->issetDynamic($container, 'cache.key'));
    }

    /**
     * `fetch v, c["k"]` is `if (isset($c['k'])) $v = $c['k'];`, which in PHP is
     * one offsetExists() then one offsetGet() with the same key. Zephir must
     * make exactly the same two calls.
     */
    public function testMatchesPlainPhpSemantics(): void
    {
        $viaPhp = new Issue2656OffsetRecorder();
        $value  = null;
        if (isset($viaPhp['cache.key'])) {
            $value = $viaPhp['cache.key'];
        }

        $viaZephir = new Issue2656OffsetRecorder();
        $result    = $this->subject->fetchLiteral($viaZephir);

        $this->assertSame($viaPhp->existsCount, $viaZephir->existsCount);
        $this->assertSame($viaPhp->seen, $viaZephir->seen);
        $this->assertSame($value, $result);
    }

    /**
     * The positive control: without it a flat probe reading proves nothing,
     * because it would also be flat if the probe could not measure at all.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->assertGreaterThan(
            0,
            $this->subject->leakProbeControl(2000),
            'The leak probe cannot detect retention, so its flat readings are meaningless.'
        );
    }

    /**
     * The isset helpers dropped the offsetExists() return value on every call.
     * A bool costs nothing, so the container answers with a heap string.
     *
     * A leak scales with the iteration count, so compare two runs rather than
     * pinning an absolute figure.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testIssetHelpersDoNotLeakTheOffsetExistsResult(): void
    {
        $container = new Issue2656RefcountedExists();

        // Settle interned strings and call caches.
        $this->subject->leakProbe($container, 'cache.key', 100);

        $small = $this->subject->leakProbe($container, 'cache.key', 1000);
        $large = $this->subject->leakProbe($container, 'cache.key', 50000);

        $this->assertSame(
            $small,
            $large,
            'Retained memory grows with the iteration count, so an isset helper leaks.'
        );
    }

    /**
     * kernel/object.c zephir_unset_property() built a zend_string for the
     * property name and never released it.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testUnsetPropertyDoesNotLeakThePropertyName(): void
    {
        $this->subject->unsetPropertyProbe(100);

        $small = $this->subject->unsetPropertyProbe(1000);
        $large = $this->subject->unsetPropertyProbe(50000);

        $this->assertSame($small, $large, 'unset obj->property leaks the property name string.');
    }

    /**
     * kernel/exception.c zephir_throw_exception_debug() addrefs the instance
     * for zend_throw_exception_object(); when the constructor call fails the
     * throw is skipped and the reference used to leak.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testFailedThrowDoesNotLeakTheExceptionInstance(): void
    {
        $this->subject->throwArrayProbe(100);

        $small = $this->subject->throwArrayProbe(1000);
        $large = $this->subject->throwArrayProbe(20000);

        $this->assertSame($small, $large, 'A throw whose constructor call fails leaks the instance.');
    }

    /**
     * PHP's zend_std_read_dimension() addrefs the container across both calls
     * because zend_call_function() does not take a reference for the frame. A
     * userland offsetExists() that drops the last reference therefore freed
     * the object mid-sequence.
     *
     * Run out of process: before the fix this is a segfault, and one crashed
     * child is a failed assertion rather than a dead test run.
     */
    public function testContainerFreedByOffsetExistsSurvives(): void
    {
        $root   = \dirname(__DIR__, 2);
        $script = (string) tempnam(sys_get_temp_dir(), 'issue2656');

        file_put_contents($script, sprintf(
            '<?php require %s; $h = new Stub\Issue2656();'
            . ' $h->setContainer(new Issue2656SelfDestroying($h));'
            . ' echo "RESULT:", $h->fetchThroughProperty(), "\n";',
            var_export($root . '/vendor/autoload.php', true)
        ));

        $command = sprintf(
            '%s %s %s 2>&1',
            escapeshellarg(PHP_BINARY),
            $this->childExtensionFlag($root),
            escapeshellarg($script)
        );

        exec($command, $output, $status);
        unlink($script);

        $printed = implode("\n", $output);

        $this->assertSame(0, $status, "Child exited with {$status}:\n" . $printed);
        $this->assertSame(
            'RESULT:alive',
            $this->resultLine($output),
            "The container was destroyed before offsetGet() ran. Child said:\n" . $printed
        );
    }

    /**
     * The child needs `-d extension=` only when it does not already inherit
     * the extension from php.ini. CI installs stub.so into extension_dir and
     * enables it from a conf.d file, so passing the flag there loads the
     * module a second time and PHP warns about it on the child's stdout.
     */
    private function childExtensionFlag(string $root): string
    {
        exec(
            sprintf(
                '%s -r %s',
                escapeshellarg(PHP_BINARY),
                escapeshellarg('echo (int) extension_loaded("stub");')
            ),
            $probe
        );

        if ('1' === trim(implode('', $probe))) {
            return '';
        }

        $localSo = $root . '/ext/modules/stub.so';

        return '-d extension=' . escapeshellarg(is_file($localSo) ? $localSo : 'stub');
    }

    /**
     * Startup diagnostics land on stdout ahead of the result, so match the
     * marker line rather than comparing the whole output.
     *
     * @param list<string> $output
     */
    private function resultLine(array $output): string
    {
        foreach ($output as $line) {
            if (str_starts_with($line, 'RESULT:')) {
                return $line;
            }
        }

        return implode("\n", $output);
    }
}
