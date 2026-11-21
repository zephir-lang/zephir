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
use Stub\Issue2805;

use function array_merge;
use function bin2hex;
use function fclose;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function fsockopen;
use function is_resource;
use function memory_get_usage;
use function mkdir;
use function mt_rand;
use function proc_close;
use function proc_get_status;
use function proc_open;
use function proc_terminate;
use function random_bytes;
use function rmdir;
use function stream_context_create;
use function stream_get_contents;
use function sys_get_temp_dir;
use function trim;
use function unlink;
use function usleep;

use const PHP_BINARY;
use const PHP_SHLIB_SUFFIX;

/**
 * A static property access keeps the zval it found in a cache record. The
 * static members of a class are allocated again in every request, so a
 * record filled in one request must not be used by the next.
 *
 * Request `b` allocates strings of the size the static members table has
 * before it reads, so the table of `b` does not land where the one of `a`
 * was, and a stale record would point into one of those strings.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
final class Issue2805Test extends TestCase
{
    private string $dir;

    /** @var resource|null */
    private $server = null;

    protected function setUp(): void
    {
        $this->dir = sys_get_temp_dir() . '/zephir-2805-' . bin2hex(random_bytes(6));
        mkdir($this->dir);

        file_put_contents(
            $this->dir . '/router.php',
            '<?php require __DIR__ . "/" . trim($_SERVER["REQUEST_URI"], "/") . ".php";'
            . ' echo \\Stub\\Issue2805Request::bump();'
        );
        file_put_contents($this->dir . '/a.php', '<?php');
        file_put_contents(
            $this->dir . '/b.php',
            '<?php $pad = []; for ($i = 0; $i < 512; $i++) { $pad[] = str_repeat("p", 7); }'
        );
    }

    protected function tearDown(): void
    {
        // Waits for the server to exit: Windows cannot delete its log while
        // the process still holds it open.
        if (is_resource($this->server)) {
            proc_terminate($this->server);
            proc_close($this->server);
        }

        foreach (['router.php', 'a.php', 'b.php', 'server.log'] as $file) {
            if (file_exists($this->dir . '/' . $file)) {
                unlink($this->dir . '/' . $file);
            }
        }

        rmdir($this->dir);
    }

    public function testShouldKeepEveryAccessCorrectOnACachedSite(): void
    {
        $this->assertSame([1000, 500500, 1002, 998, true, false, false, 1000], Issue2805::hits(1000));
        $this->assertSame([3, 6, 5, 2, true, false, false, 3], Issue2805::hits(3));
    }

    /**
     * A value __get() returns is the reader's, by value or by reference, and
     * nothing of it is left behind once the method returns.
     */
    public function testShouldNotLeakAValueReturnedByMagicGet(): void
    {
        $byValue = new class () {
            public function __get(string $name): array
            {
                return [1, 2, mt_rand()];
            }
        };
        $byReference = new class () {
            private array $store;

            public function __construct()
            {
                $this->store = ['bag' => [1, 2, mt_rand()]];
            }

            public function &__get(string $name): array
            {
                return $this->store[$name];
            }
        };

        foreach ([$byValue, $byReference] as $object) {
            Issue2805::readMagic($object, 10);

            $before = memory_get_usage();
            $count  = Issue2805::readMagic($object, 1000);
            $after  = memory_get_usage();

            $this->assertSame(4003, $count);
            $this->assertSame(0, $after - $before);
        }
    }

    public function testShouldNotReuseACacheFilledByAnEarlierRequest(): void
    {
        $port = $this->startServer();

        $responses = [];
        foreach (['a', 'b', 'a', 'b'] as $script) {
            $responses[] = $this->fetch($port, $script);
        }

        $this->assertSame(['x', 'x', 'x', 'x'], $responses);
    }

    private function startServer(): int
    {
        for ($attempt = 0; $attempt < 5; $attempt++) {
            $port    = mt_rand(20000, 60000);
            $command = array_merge(
                [PHP_BINARY],
                $this->extensionArguments(),
                ['-d', 'opcache.enable=0', '-d', 'opcache.enable_cli=0', '-S', '127.0.0.1:' . $port, $this->dir . '/router.php'],
            );

            $this->server = proc_open(
                $command,
                [0 => ['pipe', 'r'], 1 => ['file', $this->dir . '/server.log', 'a'], 2 => ['file', $this->dir . '/server.log', 'a']],
                $pipes,
                $this->dir,
            );

            for ($wait = 0; $wait < 100; $wait++) {
                $socket = @fsockopen('127.0.0.1', $port, $errno, $errstr, 0.1);
                if (false !== $socket) {
                    fclose($socket);

                    return $port;
                }

                if (!proc_get_status($this->server)['running']) {
                    break;
                }

                usleep(50000);
            }

            proc_terminate($this->server);
        }

        $this->fail('The built-in web server did not start: ' . file_get_contents($this->dir . '/server.log'));
    }

    private function fetch(int $port, string $script): string
    {
        $context = stream_context_create(['http' => ['timeout' => 10, 'ignore_errors' => true]]);
        $body    = @file_get_contents('http://127.0.0.1:' . $port . '/' . $script, false, $context);

        return false === $body ? 'no response: ' . file_get_contents($this->dir . '/server.log') : trim($body);
    }

    /**
     * CI enables the stub through an ini file, so the server already has
     * it; locally it is passed on the command line, which a subprocess
     * does not inherit. `-n` is not usable: the stub needs psr and mysqli.
     */
    private function extensionArguments(): array
    {
        if ($this->stubLoaded()) {
            return [];
        }

        $prefix    = 'dll' === PHP_SHLIB_SUFFIX ? 'php_' : '';
        $extension = __DIR__ . '/../../ext/modules/' . $prefix . 'stub.' . PHP_SHLIB_SUFFIX;

        if (!file_exists($extension)) {
            $this->markTestSkipped('The built stub extension is needed to start a server with it.');
        }

        return ['-d', 'extension=' . $extension];
    }

    /**
     * Asks a PHP started without a shell: cmd.exe cannot redirect to
     * `/dev/null`, so a shell probe never ran on Windows.
     */
    private function stubLoaded(): bool
    {
        $process = proc_open(
            [PHP_BINARY, '-r', 'echo (int) extension_loaded("stub");'],
            [1 => ['pipe', 'w'], 2 => ['pipe', 'w']],
            $pipes,
        );

        $stdout = stream_get_contents($pipes[1]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        proc_close($process);

        return '1' === trim((string) $stdout);
    }
}
