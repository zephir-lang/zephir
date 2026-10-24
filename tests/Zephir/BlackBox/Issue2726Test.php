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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

use function file_get_contents;
use function file_put_contents;
use function json_decode;
use function json_encode;

/**
 * A `hash` global was declared as a `HashTable *` and reset to NULL, but
 * neither accessor accepted it: globals_get() handed back a `hash` compiled
 * expression nothing could box, and globals_set() rejected the type.
 *
 * The member now holds a shared reference to a PHP array, or NULL. The table
 * a request global holds is released at the end of the request, while a
 * `module` hash cannot hold request memory at all and stays C-only.
 *
 * These assertions read the generated C, so they hold on every platform and
 * for the request lifecycle the CLI cannot exercise.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2726
 */
final class Issue2726Test extends TestCase
{
    use RunsZephirCommands;

    private const PROJECT = 'issue2726';

    private const GLOBALS = [
        'cache'         => ['type' => 'hash', 'default' => 'NULL'],
        'orm.ast_cache' => ['type' => 'hash', 'default' => null],
        'per_process'   => ['type' => 'hash', 'default' => 'NULL', 'module' => true],
        'an_int'        => ['type' => 'int', 'default' => 10],
    ];

    private const SETTINGS = <<<'ZEP'
        namespace Issue2726;

        class Settings
        {
            public function setCache(value) -> void
            {
                globals_set("cache", value);
                globals_set("orm.ast_cache", value);
            }

            public function getCache()
            {
                return globals_get("cache");
            }

            public function getDotted()
            {
                var v;

                let v = globals_get("orm.ast_cache");

                return v;
            }
        }

        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldReleaseEveryRequestHashAfterTheUserRequestDestructors(): void
    {
        $this->assertMatchesRegularExpression(
            '/PHP_RSHUTDOWN_FUNCTION\(issue2726\)\s*\{\s*'
            . 'user_takedown_request\(\);\s*'
            . 'zephir_globals_release_hash\(&ZEPHIR_GLOBAL\(orm\)\.ast_cache\);\s*'
            . 'zephir_globals_release_hash\(&ZEPHIR_GLOBAL\(cache\)\);\s*'
            . 'zephir_deinitialize_memory\(\);/',
            $this->generateOk()['project'],
        );
    }

    public function testShouldNotReleaseAModuleHashAtRequestEnd(): void
    {
        $this->assertStringNotContainsString(
            'zephir_globals_release_hash(&ZEPHIR_GLOBAL(per_process))',
            $this->generateOk()['project'],
        );
    }

    public function testShouldStartAHashWithAJsonNullDefaultAsNull(): void
    {
        $this->assertStringContainsString('issue2726_globals->orm.ast_cache = NULL;', $this->generateOk()['project']);
    }

    public function testShouldWriteAHashGlobalThroughTheKernel(): void
    {
        $method = $this->generateOk()['method'];

        $this->assertMatchesRegularExpression(
            '/zephir_globals_set_hash\(&ZEPHIR_GLOBAL\(cache\), value\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $method,
        );
        $this->assertMatchesRegularExpression(
            '/zephir_globals_set_hash\(&ZEPHIR_GLOBAL\(orm\)\.ast_cache, value\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $method,
        );
    }

    public function testShouldReadAHashGlobalIntoAZval(): void
    {
        $method = $this->generateOk()['method'];

        $this->assertMatchesRegularExpression(
            '/zephir_globals_get_hash\((return_value|&_\d+), ZEPHIR_GLOBAL\(cache\)\);/',
            $method,
        );
        $this->assertStringContainsString('zephir_globals_get_hash(&v, ZEPHIR_GLOBAL(orm).ast_cache);', $method);
    }

    public function testShouldRefuseToWriteAModuleHash(): void
    {
        $result = $this->generate($this->method('globals_set("per_process", value);'));

        $this->assertCompileError(
            "Global 'per_process' is a module hash: it lives across requests, so it can only be accessed from C",
            $result,
        );
    }

    public function testShouldRefuseToReadAModuleHash(): void
    {
        $result = $this->generate($this->method('return globals_get("per_process");'));

        $this->assertCompileError(
            "Global 'per_process' is a module hash: it lives across requests, so it can only be accessed from C",
            $result,
        );
    }

    /**
     * The global reads back as null while it holds no table.
     */
    public function testShouldRefuseAnArrayTypedReceiver(): void
    {
        $result = $this->generate($this->method('array v; let v = globals_get("cache"); return v;'));

        $this->assertCompileError('Returned values by functions can only be assigned to variant variables', $result);
    }

    public function testShouldReportAnInvalidHashDefaultAsAConfigError(): void
    {
        $globals          = self::GLOBALS;
        $globals['cache'] = ['type' => 'hash', 'default' => []];

        $this->assertConfigError(
            'Extension global "cache" is a hash, so its default must be null',
            $this->generate(self::SETTINGS, $globals),
        );
    }

    /**
     * The classes are compiled before the globals are emitted, so the first to
     * see a bad definition is whichever accessor reads it.
     */
    public function testShouldReportAnInvalidHashDefaultReadByGlobalsGetAsAConfigError(): void
    {
        $globals          = self::GLOBALS;
        $globals['cache'] = ['type' => 'hash', 'default' => []];

        $this->assertConfigError(
            'Extension global "cache" is a hash, so its default must be null',
            $this->generate($this->method('return globals_get("cache");'), $globals),
        );
    }

    public function testShouldReportAnIniEntryOnAHashAsAConfigError(): void
    {
        $globals          = self::GLOBALS;
        $globals['cache'] = ['type' => 'hash', 'default' => 'NULL', 'ini-entry' => ['name' => 'issue2726.cache']];

        $this->assertConfigError(
            'Extension global "cache" is a hash, which cannot have an ini-entry: an INI value is always a string',
            $this->generate(self::SETTINGS, $globals),
        );
    }

    /**
     * @param array{exitCode: int, stderr: string} $result
     */
    private function assertCompileError(string $message, array $result): void
    {
        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString($message, $result['stderr']);
    }

    /**
     * @param array{exitCode: int, stderr: string} $result
     */
    private function assertConfigError(string $message, array $result): void
    {
        $this->assertCompileError($message, $result);
        $this->assertStringNotContainsString('Internal error', $result['stderr']);
    }

    private function method(string $body): string
    {
        return "namespace Issue2726;\n\nclass Settings\n{\n    public function run(value)\n    {\n        "
            . $body . "\n    }\n}\n";
    }

    /**
     * @return array{project: string, method: string}
     */
    private function generateOk(): array
    {
        $result = $this->generate(self::SETTINGS);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        return $result;
    }

    /**
     * @return array{exitCode: int, stderr: string, project: string, method: string}
     */
    private function generate(string $zep, array $globals = self::GLOBALS): array
    {
        $projectDir = $this->outputDir() . '/' . self::PROJECT;
        $this->removePath($projectDir);
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . self::PROJECT, $this->outputDir())['exitCode']);

        $config                = json_decode((string) file_get_contents($projectDir . '/config.json'), true);
        $config['globals']     = $globals;
        $config['destructors'] = [
            'request' => [['include' => '', 'code' => 'user_takedown_request()']],
        ];
        file_put_contents($projectDir . '/config.json', json_encode($config));

        file_put_contents($projectDir . '/' . self::PROJECT . '/settings.zep', $zep);

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $ok     = 0 === $result['exitCode'];

        return [
            'exitCode' => $result['exitCode'],
            'stderr'   => $result['stderr'],
            'project'  => $ok ? (string) file_get_contents($projectDir . '/ext/' . self::PROJECT . '.c') : '',
            'method'   => $ok
                ? (string) file_get_contents($projectDir . '/ext/' . self::PROJECT . '/settings.zep.c')
                : '',
        ];
    }
}
