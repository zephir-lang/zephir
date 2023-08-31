<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\Backends\ZendEngine3;

use PHPUnit\Framework\TestCase;
use Zephir\Backend\Backend;
use Zephir\CompilationContext;
use Zephir\Config;
use Zephir\Variable;

final class BackendTest extends TestCase
{
    public function stringDataProvider(): array
    {
        return [
            'regexp1' => ['/(\w+)\s*=\s*(\[[^\]]*\]|\"[^\"]*\"|[^,)]*)\s*(?:,|$)/', '/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/'],
            'regexp2' => ['/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/', '/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/'],
            'simple string' => ['zephir', 'zephir'],
            'string with slashed' => ['\\Zephir', '\Zephir'],
            'string with \n' => ['\\Zephir\n', '\Zephir\\n'],
            'string with \null' => ['\\null', '\null'],
        ];
    }

    /**
     * @dataProvider stringDataProvider
     *
     * @param string $testString
     * @param string $expectedString
     */
    public function testShouldEscapeStringWithRegexp(string $testString, string $expectedString): void
    {
        $backend = new Backend(new Config(), null, null);
        $variable = new Variable('variable', 'name');
        $context = new CompilationContext();

        $actual = $backend->assignString($variable, $testString, $context, false);
        $expected = 'ZVAL_STRING(&name, "'.$expectedString.'");';

        $this->assertSame($expected, $actual);
    }
}
