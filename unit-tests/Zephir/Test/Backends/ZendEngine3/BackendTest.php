<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\Backends\ZendEngine3;

use PHPUnit\Framework\TestCase;
use Zephir\Backends\ZendEngine3\Backend;
use Zephir\CompilationContext;
use Zephir\Config;
use Zephir\Variable;

class BackendTest extends TestCase
{
    public function stringDataProvider()
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
     * @test
     * @dataProvider stringDataProvider
     */
    public function shouldEscapeStringWithRegexp(string $testString, string $expectedString)
    {
        $backend = new Backend(new Config(), null, null);
        $variable = new Variable('variable', 'name');
        $context = new CompilationContext();

        $actual = $backend->assignString($variable, $testString, $context, false);
        $expected = 'ZVAL_STRING(&name, "'.$expectedString.'");';

        $this->assertSame($expected, $actual);
    }
}
