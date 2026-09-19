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
use Stub\Globals;

use const PHP_BINARY;
use const PHP_SHLIB_SUFFIX;

/**
 * An extension global declared in config.json is a php.ini directive.
 *
 * Only `bool` and `string` used to register one, and the value a directive
 * carried was thrown away again at the start of every request, so nothing set
 * in php.ini ever took effect.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2449
 */
final class GlobalsIniTest extends TestCase
{
    use AssertsPhpParity;

    public function startupProvider(): array
    {
        return [
            // directive, value, getter, expected
            'bool'                  => ['ini-entry.my_setting_1', '0', 'getDefaultGlobals4', false],
            'bool without ini-entry' => ['stub.test_setting_1', '0', 'getTestSetting1', false],
            'int'                   => ['stub.my_setting_2', '42', 'getDefaultGlobals5', 42],
            'double'                => ['stub.my_setting_3', '2.5', 'getDefaultGlobals6', 2.5],
            // A char global reads back as its character code: 90 is 'Z'.
            'char'                  => ['stub.my_setting_4', 'Z', 'getDefaultGlobals7', 90],
            'string'                => ['stub.my_setting_5', 'from-ini', 'getDefaultGlobals8', 'from-ini'],
            'uint'                  => ['stub.my_setting_6', '7', 'getUIntValue', 7],
            'bool with a renamed directive' => ['extension.test_ini_variable', '0', 'getTestIniVariable', false],
            'bool in a struct'      => ['stub.db.my_setting_1', '1', 'getDefaultGlobals1', true],
            'bool in a struct, again' => ['stub.orm.cache_enable', '0', 'getDefaultGlobalsOrmCacheEnable', false],
            'int in a struct'       => ['stub.db.my_setting_2', '11', 'getDefaultGlobals2', 11],
            'double in a struct'    => ['stub.db.my_setting_3', '0.25', 'getDefaultGlobals3', 0.25],
            'int in a struct, again' => ['stub.orm.cache_level', '99', 'getDefaultGlobalsOrmCacheLevel', 99],
            'string in a struct'    => ['stub.orm.cache_prefix', 'PFX', 'getDefaultGlobalsOrmCachePrefix', 'PFX'],
            // Set up once per process rather than per request, and still
            // seeded from php.ini because GINIT runs before MINIT.
            'module scoped'         => ['stub.module_setting', '0', 'getModuleSetting', false],
        ];
    }

    /**
     * @dataProvider startupProvider
     */
    public function testShouldTakeTheValueGivenAtStartup(
        string $directive,
        string $value,
        string $getter,
        $expected
    ): void {
        $output = $this->runWithDirective(
            $directive,
            $value,
            sprintf('echo json_encode((new \Stub\Globals())->%s());', $getter),
        );

        $this->assertSame($expected, json_decode($output, true), 'stdout was: ' . $output);
    }

    /**
     * @dataProvider startupProvider
     */
    public function testShouldExposeTheValueThroughIniGet(string $directive, string $value): void
    {
        $output = $this->runWithDirective(
            $directive,
            $value,
            sprintf('echo ini_get(%s);', var_export($directive, true)),
        );

        $this->assertSame($value, $output);
    }

    /**
     * A directive belonging to the extension has to behave the way a directive
     * belonging to PHP itself does. `precision` is the closest core analogue
     * of an int global: PHP_INI_ALL, numeric, with no displayer of its own.
     */
    public function testShouldBehaveLikeACoreDirective(): void
    {
        $this->assertMatchesPhp(
            fn() => $this->exercise('stub.my_setting_2', '77'),
            fn() => $this->exercise('precision', '7'),
        );
    }

    /**
     * ini_set() has to reach the global, which is what a NULL on_modify
     * handler prevented for every string global.
     */
    public function testShouldPropagateARuntimeChangeToTheGlobal(): void
    {
        $globals = new Globals();

        ini_set('stub.my_setting_5', 'runtime');
        $this->assertSame('runtime', $globals->getDefaultGlobals8());

        ini_set('stub.my_setting_2', '123');
        $this->assertSame(123, $globals->getDefaultGlobals5());

        ini_restore('stub.my_setting_5');
        ini_restore('stub.my_setting_2');

        $this->assertSame('custom_value', $globals->getDefaultGlobals8());
        $this->assertSame(10, $globals->getDefaultGlobals5());
    }

    /**
     * An unsigned global refuses a negative value instead of wrapping around,
     * and leaves both the directive and the global untouched.
     */
    public function testShouldRejectANegativeValueForAnUnsignedGlobal(): void
    {
        $globals = new Globals();

        $this->assertFalse(@ini_set('stub.my_setting_6', '-1'));
        $this->assertSame('64', ini_get('stub.my_setting_6'));
        $this->assertSame(64, $globals->getUIntValue());
    }

    /**
     * globals_set() writes the struct member and bypasses the ini entry, so
     * the engine cannot restore it at the end of the request the way it
     * restores an ini_set(). That is why the generated RINIT re-applies every
     * request-scoped directive; this asserts the half that is observable
     * within a single request.
     */
    public function testGlobalsSetShouldNotMoveTheDirective(): void
    {
        $globals = new Globals();

        $globals->setIntValue(4321);
        $globals->setUIntValue(11);
        $globals->setDoubleValue(0.5);

        $this->assertSame(4321, $globals->getDefaultGlobals5());
        $this->assertSame(11, $globals->getUIntValue());
        $this->assertSame(0.5, $globals->getDefaultGlobals6());

        $this->assertSame('10', ini_get('stub.my_setting_2'));
        $this->assertSame('64', ini_get('stub.my_setting_6'));
        $this->assertSame('15.2', ini_get('stub.my_setting_3'));

        $globals->setIntValue(10);
        $globals->setUIntValue(64);
        $globals->setDoubleValue(15.2);
    }

    /**
     * One ini_set()/ini_get()/ini_restore() round trip, as a transcript the
     * parity assertion can compare across two different directives.
     */
    private function exercise(string $directive, string $value): array
    {
        $before   = ini_get($directive);
        $previous = ini_set($directive, $value);
        $during   = ini_get($directive);
        ini_restore($directive);

        return [
            'is string'   => is_string($before),
            'set returns' => $before === $previous,
            'took'        => $value === $during,
            'restored'    => $before === ini_get($directive),
        ];
    }

    /**
     * Runs a snippet in a fresh process with one directive set at startup.
     *
     * `php -n` is not usable here: the stub extension needs psr and mysqli,
     * which only php.ini loads.
     */
    private function runWithDirective(string $directive, string $value, string $code): string
    {
        $arguments   = $this->extensionArguments();
        $arguments[] = '-d ' . escapeshellarg($directive . '=' . $value);
        $arguments[] = '-r ' . escapeshellarg($code);

        return trim((string) shell_exec(
            escapeshellarg(PHP_BINARY) . ' ' . implode(' ', $arguments) . ' 2>&1'
        ));
    }

    /**
     * The arguments needed to give the subprocess the extension, if any.
     *
     * CI enables the stub through a conf.d ini file, so a subprocess already
     * has it; loading it again prints `Module "stub" is already loaded` into
     * the output this test parses. Locally the extension is passed to PHPUnit
     * on the command line instead, and a subprocess inherits nothing.
     *
     * @return list<string>
     */
    private function extensionArguments(): array
    {
        static $arguments;

        if (isset($arguments)) {
            return $arguments;
        }

        if ($this->subprocessLoadsStub()) {
            return $arguments = [];
        }

        /* Windows names it php_stub.dll, the way tests/ext-bootstrap.php does. */
        $prefix    = 'dll' === PHP_SHLIB_SUFFIX ? 'php_' : '';
        $extension = __DIR__ . '/../../ext/modules/' . $prefix . 'stub.' . PHP_SHLIB_SUFFIX;

        if (!file_exists($extension)) {
            $this->markTestSkipped('The built stub extension is needed to set a directive at startup.');
        }

        return $arguments = ['-d ' . escapeshellarg('extension=' . $extension)];
    }

    private function subprocessLoadsStub(): bool
    {
        $probe = shell_exec(sprintf(
            '%s -r %s 2>/dev/null',
            escapeshellarg(PHP_BINARY),
            escapeshellarg('echo (int) extension_loaded("stub");'),
        ));

        return '1' === trim((string) $probe);
    }
}
