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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;

use function dirname;
use function file_get_contents;
use function json_decode;
use function preg_match;
use function str_contains;

/**
 * The per-request reset of an INI-backed global cannot be observed from the
 * CLI SAPI, which serves exactly one request per process. What can be checked
 * is that the generated C still wires it up: the list of directives to
 * re-apply, and the RINIT call that walks it.
 *
 * Without that call a value written by globals_set() outlives the request that
 * wrote it, which is what dotted globals did before #2449 - cphalcon's
 * `warning.enable` is toggled off and back on around a block, and an exception
 * in between used to leak the "off" into every later request on that worker.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2449
 */
final class RequestIniGlobalsTest extends TestCase
{
    private string $generated;

    protected function setUp(): void
    {
        $this->generated = (string) file_get_contents($this->projectRoot() . '/ext/stub.c');
    }

    public function testShouldReApplyTheDirectivesOnEveryRequest(): void
    {
        $this->assertTrue(
            (bool) preg_match(
                '/PHP_RINIT_FUNCTION\(stub\).*?zephir_ini_activate_globals\(zephir_request_ini_entries\);/s',
                $this->generated,
            ),
            'RINIT must re-apply the request-scoped ini directives',
        );
    }

    public function testShouldListEveryRequestScopedDirectiveAndNoOther(): void
    {
        $expected = [];
        foreach ($this->configuredGlobals() as $name => $global) {
            if ('hash' === $global['type'] || !empty($global['module'])) {
                continue;
            }

            $expected[] = $global['ini-entry']['name'] ?? 'stub.' . $name;
        }

        $this->assertNotEmpty($expected);

        preg_match(
            '/zephir_request_ini_entries\[\] = \{(.*?)\};/s',
            $this->generated,
            $matches,
        );

        $this->assertArrayHasKey(1, $matches, 'The generated list of request-scoped directives is missing');

        foreach ($expected as $directive) {
            $this->assertStringContainsString('"' . $directive . '"', $matches[1]);
        }
    }

    /**
     * A module-scoped global is set up once per process, so putting it back to
     * its php.ini value on every request would defeat the point of declaring
     * it that way.
     */
    public function testShouldLeaveOutAModuleScopedDirective(): void
    {
        preg_match(
            '/zephir_request_ini_entries\[\] = \{(.*?)\};/s',
            $this->generated,
            $matches,
        );

        $this->assertArrayHasKey(1, $matches);
        $this->assertStringNotContainsString('"stub.module_setting"', $matches[1]);
        $this->assertTrue(
            str_contains($this->generated, 'STD_PHP_INI_BOOLEAN("stub.module_setting"'),
            'A module-scoped global still gets its directive',
        );
    }

    private function configuredGlobals(): array
    {
        $config = json_decode(
            (string) file_get_contents($this->projectRoot() . '/config.json'),
            true,
        );

        return $config['globals'];
    }

    private function projectRoot(): string
    {
        return dirname(__DIR__, 2);
    }
}
