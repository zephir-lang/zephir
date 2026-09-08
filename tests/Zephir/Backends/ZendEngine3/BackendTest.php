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
use Zephir\Variable\Variable;

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
        $backend = new Backend(new Config(), '', '');
        $variable = new Variable('variable', 'name');
        $context = new CompilationContext();

        $actual = $backend->assignString($variable, $testString, $context, false);
        $expected = 'ZVAL_STRING(&name, "'.$expectedString.'");';

        $this->assertSame($expected, $actual);
    }

    /**
     * Zephir's four integer types all describe a PHP `int`, which the engine
     * stores in a `zend_long` (`int64_t` on every 64-bit target). Mapping any
     * of them to a C `long` makes the width follow the data model: 64-bit on
     * LP64 (Linux, macOS), 32-bit on LLP64 (Windows x64).
     *
     * @see https://github.com/zephir-lang/zephir/issues/2666
     */
    public function integerTypeDefinitionProvider(): array
    {
        return [
            'int'   => ['int', 'zend_long'],
            'uint'  => ['uint', 'zend_ulong'],
            'long'  => ['long', 'zend_long'],
            'ulong' => ['ulong', 'zend_ulong'],
        ];
    }

    /**
     * @dataProvider integerTypeDefinitionProvider
     */
    public function testIntegerTypesMapToAFixedWidthCType(string $type, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');

        $this->assertSame([null, $expected], $backend->getTypeDefinition($type));
    }
}
