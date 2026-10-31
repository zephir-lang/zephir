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

use function array_merge;
use function bin2hex;
use function escapeshellarg;
use function fclose;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function fsockopen;
use function is_resource;
use function mkdir;
use function mt_rand;
use function proc_get_status;
use function proc_open;
use function proc_terminate;
use function random_bytes;
use function rmdir;
use function shell_exec;
use function sprintf;
use function stream_context_create;
use function sys_get_temp_dir;
use function trim;
use function unlink;
use function usleep;

use const PHP_BINARY;
use const PHP_SHLIB_SUFFIX;

/**
 * A call site's inline cache holds a class entry and a method, and a class
 * declared by a request is freed when that request ends. The next request
 * can declare another class at the same address, so a cache filled in one
 * request must not be used by the next.
 *
 * Both requests declare a class named `Aba`, compiled from files of the same
 * length, so the second class entry lands where the first one was. Its
 * `who()` comes after a `pad()` method, so it does not.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2790
 */
final class Issue2790Test extends TestCase
{
    private string $dir;

    /** @var resource|null */
    private $server = null;

    protected function setUp(): void
    {
        $this->dir = sys_get_temp_dir() . '/zephir-2790-' . bin2hex(random_bytes(6));
        mkdir($this->dir);

        file_put_contents(
            $this->dir . '/router.php',
            '<?php require __DIR__ . "/" . trim($_SERVER["REQUEST_URI"], "/") . ".php";'
            . ' echo (new \Stub\Issue2790())->callWho(new Aba());'
        );
        file_put_contents(
            $this->dir . '/a.php',
            '<?php class Aba { public function who() { return "first"; } }'
        );
        file_put_contents(
            $this->dir . '/b.php',
            '<?php class Aba { public function pad() { return "padded"; } public function who() { return "second"; } }'
        );
    }

    protected function tearDown(): void
    {
        if (is_resource($this->server)) {
            proc_terminate($this->server);
        }

        foreach (['router.php', 'a.php', 'b.php', 'server.log'] as $file) {
            if (file_exists($this->dir . '/' . $file)) {
                unlink($this->dir . '/' . $file);
            }
        }

        rmdir($this->dir);
    }

    public function testShouldNotReuseACacheFilledByAnEarlierRequest(): void
    {
        $port = $this->startServer();

        $responses = [];
        foreach (['a', 'b', 'a', 'b'] as $script) {
            $responses[] = $this->fetch($port, $script);
        }

        $this->assertSame(['first', 'second', 'first', 'second'], $responses);
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
     * CI enables the stub through a conf.d ini file, so the server already
     * has it; locally it is passed on the command line, which a subprocess
     * does not inherit. `-n` is not usable: the stub needs psr and mysqli.
     */
    private function extensionArguments(): array
    {
        $probe = shell_exec(sprintf(
            '%s -r %s 2>/dev/null',
            escapeshellarg(PHP_BINARY),
            escapeshellarg('echo (int) extension_loaded("stub");'),
        ));

        if ('1' === trim((string) $probe)) {
            return [];
        }

        $prefix    = 'dll' === PHP_SHLIB_SUFFIX ? 'php_' : '';
        $extension = __DIR__ . '/../../ext/modules/' . $prefix . 'stub.' . PHP_SHLIB_SUFFIX;

        if (!file_exists($extension)) {
            $this->markTestSkipped('The built stub extension is needed to start a server with it.');
        }

        return ['-d', 'extension=' . $extension];
    }
}
