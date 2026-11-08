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

use PHPUnit\Framework\TestCase;
use Stub\Issue2826;

use function array_map;
use function array_merge;
use function array_slice;
use function debug_zval_dump;
use function fclose;
use function file_exists;
use function file_put_contents;
use function implode;
use function ob_get_clean;
use function ob_start;
use function proc_close;
use function proc_open;
use function sprintf;
use function stream_get_contents;
use function sys_get_temp_dir;
use function tempnam;
use function trim;
use function unlink;
use function var_export;

use const PHP_BINARY;
use const PHP_SHLIB_SUFFIX;

/**
 * `fetch` and `empty` on an element holding an empty array that is not
 * reference counted wrote a new array into that element of the source. PHP's
 * reads never write, so the source must stay exactly as it was.
 *
 * `debug_zval_dump()` tells the two arrays apart on every supported PHP version
 * only when it is given the elements as arguments: nested in their array, PHP
 * 8.0 prints `refcount(1)` for both.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2826
 */
final class Issue2826Test extends TestCase
{
    private Issue2826 $test;

    /** @var string|null */
    private $script = null;

    protected function setUp(): void
    {
        $this->test = new Issue2826();
    }

    protected function tearDown(): void
    {
        if (null !== $this->script && file_exists($this->script)) {
            unlink($this->script);
        }
    }

    /**
     * @return array<string, array{0: string, 1: array<int, int|string>}>
     */
    public static function readProvider(): array
    {
        return [
            'fetch a[k]'                     => ['fetchDynamic', ['key']],
            'fetch a[k] with an integer key' => ['fetchDynamic', [0]],
            'fetch a[0]'                     => ['fetchLong', []],
            'fetch a["key"]'                 => ['fetchString', []],
            'empty a[k]'                     => ['emptyDynamic', ['key']],
            'empty a[0]'                     => ['emptyLong', []],
            'empty a["key"]'                 => ['emptyString', []],
        ];
    }

    /**
     * @dataProvider readProvider
     */
    public function testReadLeavesTheSourceUnchanged(string $method, array $arguments): void
    {
        $params = $this->emptyElements();
        $before = $this->dumpElements($params);

        $this->test->$method($params, ...$arguments);

        $this->assertSame($before, $this->dumpElements($params));
    }

    /**
     * `fetch` finds the element and `empty` finds it empty.
     *
     * @dataProvider readProvider
     */
    public function testReadFindsTheEmptyArray(string $method, array $arguments): void
    {
        $this->assertTrue($this->test->$method($this->emptyElements(), ...$arguments));
    }

    /**
     * A Zephir array constant lives as long as the process, so a request
     * allocated array written into it is left dangling when the request ends.
     */
    public function testFetchLeavesAZephirConstantUnchanged(): void
    {
        $before = $this->dumpElements(Issue2826::PARAMS);

        $this->test->fetchString(Issue2826::PARAMS);

        $this->assertSame($before, $this->dumpElements(Issue2826::PARAMS));
    }

    /**
     * @return array<string, array{0: string, 1: array<int|string, int>, 2: array<int|string, mixed>}>
     */
    public static function writeProvider(): array
    {
        return [
            'assign to an empty array'        => ['assignFetched', [], ['y' => 1]],
            'append to an empty array'        => ['appendFetched', [], [1]],
            'assign a nested offset'          => ['assignNestedFetched', [], ['a' => ['b' => 1]]],
            'assign to an array'              => ['assignFetched', ['y' => 0, 'z' => 2], ['y' => 1, 'z' => 2]],
            'append to an array'              => ['appendFetched', ['y' => 0], ['y' => 0, 0 => 1]],
            'unset in an array'               => ['unsetFetched', ['y' => 1, 'z' => 2], ['z' => 2]],
            'assign a nested offset in array' => ['assignNestedFetched', ['z' => 2], ['z' => 2, 'a' => ['b' => 1]]],
        ];
    }

    /**
     * @dataProvider writeProvider
     */
    public function testWriteChangesTheFetchedValue(string $method, array $element, array $expected): void
    {
        $this->assertSame($expected, $this->test->$method(['key' => $element]));
    }

    /**
     * @dataProvider writeProvider
     */
    public function testWriteLeavesTheSourceUnchanged(string $method, array $element, array $expected): void
    {
        $params = ['key' => $element];

        $this->test->$method($params);

        $this->assertSame(['key' => $element], $params);
    }

    /**
     * The reported crash. OPcache keeps a constant array in shared memory, and
     * with `opcache.protect_memory` a write to it is a segmentation fault.
     *
     * @dataProvider readProvider
     */
    public function testReadDoesNotWriteIntoOpcacheSharedMemory(string $method, array $arguments): void
    {
        $extension = $this->extensionArguments();
        $opcache   = $this->opcacheArguments($extension);

        $this->script = tempnam(sys_get_temp_dir(), 'zephir-2826-');
        file_put_contents($this->script, sprintf(
            '<?php echo opcache_is_script_cached(__FILE__) ? "[cached]" : "[not cached]";'
            . ' (new \Stub\Issue2826())->%s(["", "key" => [], 0 => []]%s); echo " done";',
            $method,
            implode('', array_map(static fn ($argument): string => ', ' . var_export($argument, true), $arguments))
        ));

        $result = $this->runPhp(array_merge(
            [PHP_BINARY],
            $extension,
            $opcache,
            [
                '-d', 'opcache.enable_cli=1',
                '-d', 'opcache.protect_memory=1',
                '-d', 'opcache.file_update_protection=0',
                $this->script,
            ],
        ));

        $this->assertSame(['stdout' => '[cached] done', 'exit' => 0], [
            'stdout' => $result['stdout'],
            'exit'   => $result['exit'],
        ], $result['stderr']);
    }

    /**
     * Built at run time, so every case starts from elements nothing has read.
     * A literal would be one array shared by every run of this method.
     */
    private function emptyElements(): array
    {
        $params        = [];
        $params['key'] = [];
        $params[0]     = [];

        return $params;
    }

    private function dumpElements(array $params): string
    {
        ob_start();
        debug_zval_dump($params['key'], $params[0]);

        return (string) ob_get_clean();
    }

    /**
     * OPcache is not always loaded, and loading it a second time is an error.
     */
    private function opcacheArguments(array $extension): array
    {
        foreach ([[], ['-d', 'zend_extension=opcache']] as $load) {
            $probe = $this->runPhp(array_merge(
                [PHP_BINARY],
                $extension,
                $load,
                ['-r', 'echo (int) function_exists("opcache_is_script_cached");'],
            ));

            if ('1' === $probe['stdout']) {
                return $load;
            }
        }

        $this->markTestSkipped('OPcache cannot be loaded.');
    }

    /**
     * Runs PHP without a shell: cmd.exe cannot redirect to `/dev/null`.
     *
     * @return array{stdout: string, stderr: string, exit: int}
     */
    private function runPhp(array $command): array
    {
        $command = array_merge([$command[0], '-d', 'display_errors=stderr'], array_slice($command, 1));
        $process = proc_open($command, [1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes);

        $stdout = stream_get_contents($pipes[1]);
        $stderr = stream_get_contents($pipes[2]);
        fclose($pipes[1]);
        fclose($pipes[2]);

        return ['stdout' => trim((string) $stdout), 'stderr' => (string) $stderr, 'exit' => proc_close($process)];
    }

    /**
     * CI enables the stub through an ini file, so a subprocess already has
     * it; locally it is passed on the command line, which a subprocess
     * does not inherit. `-n` is not usable: the stub needs psr and mysqli.
     */
    private function extensionArguments(): array
    {
        if ('1' === $this->runPhp([PHP_BINARY, '-r', 'echo (int) extension_loaded("stub");'])['stdout']) {
            return [];
        }

        $prefix    = 'dll' === PHP_SHLIB_SUFFIX ? 'php_' : '';
        $extension = __DIR__ . '/../../ext/modules/' . $prefix . 'stub.' . PHP_SHLIB_SUFFIX;

        if (!file_exists($extension)) {
            $this->markTestSkipped('The built stub extension is needed to start PHP with it.');
        }

        return ['-d', 'extension=' . $extension];
    }
}
