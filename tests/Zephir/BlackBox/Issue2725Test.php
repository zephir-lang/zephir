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
use function preg_match;

/**
 * globals_set() on a `string` global stored `ZSTR_VAL(zval_get_string(v))`:
 * the zend_string was owned by nobody, so every call leaked it, and because it
 * was request memory a `module` global pointed at freed memory in the next
 * request.
 *
 * The member is a `char *` that php.ini's OnUpdateString points into the ini
 * entry, which must never be freed. So globals_set() now keeps its own
 * persistent copy in a companion member, replaced on the next set and freed
 * when the globals are destroyed.
 *
 * These assertions read the generated C, so they hold on every platform and
 * for the multi-request half the CLI SAPI cannot exercise.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2725
 */
final class Issue2725Test extends TestCase
{
    use RunsZephirCommands;

    private const PROJECT = 'issue2725';

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldDeclareAnOwnedCopyBesideAStringGlobalInAStruct(): void
    {
        $this->assertMatchesRegularExpression(
            '/\tchar \* a_string;\s+char \* zephir_owned_a_string;/',
            $this->block($this->generate()['header'], '/typedef struct _zephir_struct_db \{(.*?)\}/s'),
        );
    }

    public function testShouldDeclareAnOwnedCopyBesideAPlainStringGlobal(): void
    {
        $globals = $this->block(
            $this->generate()['header'],
            '/ZEND_BEGIN_MODULE_GLOBALS\(issue2725\)(.*?)ZEND_END_MODULE_GLOBALS/s',
        );

        $this->assertMatchesRegularExpression('/\tchar \* a_string;\s+char \* zephir_owned_a_string;/', $globals);
        $this->assertMatchesRegularExpression('/\tchar \* per_process;\s+char \* zephir_owned_per_process;/', $globals);
    }

    public function testShouldDeclareNoOwnedCopyForAnyOtherType(): void
    {
        $this->assertStringNotContainsString('zephir_owned_an_int', $this->generate()['header']);
    }

    public function testShouldStartEveryOwnedCopyEmptyOncePerThread(): void
    {
        $this->assertMatchesRegularExpression(
            '/php_zephir_init_module_globals\([^)]*\)\s*\{'
            . '(?=[^}]*\tissue2725_globals->zephir_owned_a_string = NULL;)'
            . '(?=[^}]*\tissue2725_globals->db\.zephir_owned_a_string = NULL;)'
            . '(?=[^}]*\tissue2725_globals->zephir_owned_per_process = NULL;)/',
            $this->generate()['project'],
        );
    }

    public function testShouldFreeEveryOwnedCopyAfterTheUserGlobalsDestructors(): void
    {
        $this->assertMatchesRegularExpression(
            '/PHP_GSHUTDOWN_FUNCTION\(issue2725\)\s*\{\s*'
            . 'user_takedown_globals\(\);\s*'
            . 'zephir_globals_release_string\(&issue2725_globals->db\.zephir_owned_a_string\);\s*'
            . 'zephir_globals_release_string\(&issue2725_globals->zephir_owned_a_string\);\s*'
            . 'zephir_globals_release_string\(&issue2725_globals->zephir_owned_per_process\);\s*\}/',
            $this->generate()['project'],
        );
    }

    public function testShouldWriteAStringGlobalThroughItsOwnedCopy(): void
    {
        $method = $this->generate()['method'];

        $this->assertMatchesRegularExpression(
            '/zephir_globals_set_string\(&ZEPHIR_GLOBAL\(a_string\), &ZEPHIR_GLOBAL\(zephir_owned_a_string\), value\);'
            . '\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $method,
        );
        $this->assertMatchesRegularExpression(
            '/zephir_globals_set_string\(&ZEPHIR_GLOBAL\(db\)\.a_string, &ZEPHIR_GLOBAL\(db\)\.zephir_owned_a_string, value\);'
            . '\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $method,
        );
    }

    public function testShouldNeverKeepABareConvertedString(): void
    {
        $this->assertStringNotContainsString('ZSTR_VAL(zval_get_string(', $this->generate()['method']);
    }

    /**
     * A `string` compiled expression holds literal text everywhere else in the
     * compiler, so globals_get() used to hand over the name of the C member as
     * the value of a `let` or a concatenation.
     */
    public function testShouldReadAStringGlobalIntoAZval(): void
    {
        $method = $this->generate()['method'];

        $this->assertStringContainsString('ZVAL_STRING(&v, ZEPHIR_GLOBAL(a_string));', $method);
        $this->assertMatchesRegularExpression('/ZVAL_STRING\(&_\d+, ZEPHIR_GLOBAL\(db\)\.a_string\);/', $method);
        $this->assertStringNotContainsString('"ZEPHIR_GLOBAL(', $method);
    }

    /**
     * An error handler can turn the "could not be converted" warning into an
     * exception. PHP's cast then produces no value, so the global must not be
     * written either.
     */
    public function testShouldWriteANumericGlobalOnlyOnceTheConversionSucceeded(): void
    {
        $method = $this->generate()['method'];

        $this->assertMatchesRegularExpression(
            '/(_\d+) = zval_get_long\(value\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{[^}]*\}\s*ZEPHIR_GLOBAL\(an_int\) = \1;/',
            $method,
        );
        $this->assertMatchesRegularExpression(
            '/(_\d+) = zval_get_long\(value\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{[^}]*\}\s*ZEPHIR_GLOBAL\(db\)\.an_int = \1;/',
            $method,
        );
    }

    private function block(string $code, string $pattern): string
    {
        $this->assertSame(1, preg_match($pattern, $code, $matches), 'Block not found: ' . $pattern);

        return $matches[1];
    }

    /**
     * @return array{header: string, project: string, method: string}
     */
    private function generate(): array
    {
        $projectDir = $this->outputDir() . '/' . self::PROJECT;
        $this->removePath($projectDir);
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . self::PROJECT, $this->outputDir())['exitCode']);

        $config                = json_decode((string) file_get_contents($projectDir . '/config.json'), true);
        $config['globals']     = [
            'a_string'    => ['type' => 'string', 'default' => 'plain'],
            'db.a_string' => ['type' => 'string', 'default' => 'dotted'],
            'per_process' => ['type' => 'string', 'default' => 'module', 'module' => true],
            'an_int'      => ['type' => 'int', 'default' => 10],
            'db.an_int'   => ['type' => 'int', 'default' => 10],
        ];
        $config['destructors'] = [
            'globals' => [['include' => '', 'code' => 'user_takedown_globals()']],
        ];
        file_put_contents($projectDir . '/config.json', json_encode($config));

        file_put_contents(
            $projectDir . '/' . self::PROJECT . '/settings.zep',
            <<<'ZEP'
                namespace Issue2725;

                class Settings
                {
                    public function setString(value) -> void
                    {
                        globals_set("a_string", value);
                        globals_set("db.a_string", value);
                    }

                    public function setInt(value) -> void
                    {
                        globals_set("an_int", value);
                        globals_set("db.an_int", value);
                    }

                    public function getString()
                    {
                        var v;

                        let v = globals_get("a_string");

                        return v . globals_get("db.a_string");
                    }
                }

                ZEP,
        );

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        return [
            'header'  => (string) file_get_contents($projectDir . '/ext/php_' . self::PROJECT . '.h'),
            'project' => (string) file_get_contents($projectDir . '/ext/' . self::PROJECT . '.c'),
            'method'  => (string) file_get_contents($projectDir . '/ext/' . self::PROJECT . '/settings.zep.c'),
        ];
    }
}
