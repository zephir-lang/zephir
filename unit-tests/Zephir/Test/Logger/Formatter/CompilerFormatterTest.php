<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Logger\Formatter;

use PHPUnit\Framework\TestCase;
use Zephir\Config;
use Zephir\Logger\Formatter\CompilerFormatter;

class CompilerFormatterTest extends TestCase
{
    /** @var Config $config */
    private $config;

    public function setUp()
    {
        $this->config = new Config();
    }

    /** @test */
    public function shouldBeQuietWhenConfigSilent()
    {
        $this->config->set('silent', true);
        $compilerFormatter = new CompilerFormatter($this->config);

        $this->assertSame('', $compilerFormatter->format([]), 'Should return empty string when Config have: silent=true');
    }

    public function getWarningContext(): array
    {
        return [
            'message' => 'Variable "param1" declared but not used in test\3__closure::__invoke',
            'context' => [
                'unused-variable-external',
                [
                    'type' => 'parameter',
                    'name' => 'param1',
                    'const' => 0,
                    'data-type' => 'variable',
                    'mandatory' => 0,
                    'reference' => 0,
                    'file' => realpath(__DIR__.'/../../../../../test/closures.zep'),
                    'line' => 20,
                    'char' => 25,
                ],
            ],
            'level' => 300,
            'level_name' => 'WARNING',
            'channel' => 'zephir',
            'datetime' => new \DateTime(),
            'extra' => [],
        ];
    }

    /** @test */
    public function shouldBeQuietForDisabledWarning()
    {
        // deny to print warnings for this category
        $this->config->offsetSet(['warnings' => 'unused-variable-external'], false);
        $compilerFormatter = new CompilerFormatter($this->config);

        $this->assertSame('', $compilerFormatter->format($this->getWarningContext()));
    }

    /** @test */
    public function shouldFormatWarningForCategory()
    {
        // allow to print warnings for this category
        $this->config->offsetSet(['warnings' => 'unused-variable-external'], true);
        $compilerFormatter = new CompilerFormatter($this->config);
        $testContext = $this->getWarningContext();
        $filePath = $testContext['context'][1]['file'];

        $expected = " Warning: Variable \"param1\" declared but not used in test\\3__closure::__invoke in {$filePath} on line 20 [unused-variable-external]\n\n".
                    "\t**return function(param1) {\n\n".
                    "\t------------------------^\n";

        $this->assertSame($expected, $compilerFormatter->format($testContext));

        unset($testContext['context'][1]['file']);
        $expected = ' Warning: Variable "param1" declared but not used in test\3__closure::__invoke in unknown on line 0 [unused-variable-external]'.PHP_EOL;

        $this->assertSame($expected, $compilerFormatter->format($testContext));
    }
}
