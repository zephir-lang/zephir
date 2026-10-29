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
use Zephir\Parser\Manager;

/**
 * The newest grammar compiles through `ext-zephir_parser` exactly as through
 * the pure-PHP parser. The zept suite always forces the pure-PHP parser, so
 * without this a MINIMUM_PARSER_VERSION that lags the grammar would go
 * unnoticed: the extension would be used and reject the syntax.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2711
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
final class ParserExtensionGrammarTest extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Pegrammar;

        class Sample
        {
            const NAME = "sample";

            public static function lateStaticConstant() -> string
            {
                return static::NAME;
            }

            public static function lateStaticMethod(string method)
            {
                return static::{method}();
            }

            public static function constantClassMethod(string method)
            {
                return SAMPLE::{method}();
            }
        }

        ZEP;

    protected function setUp(): void
    {
        $version = phpversion('zephir_parser');
        if (!Manager::isExtensionUsable($version === false ? null : $version)) {
            $this->markTestSkipped('Needs ext-zephir_parser >= ' . Manager::MINIMUM_PARSER_VERSION . '.');
        }

        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
        putenv('ZEPHIR_FORCE_PHP_PARSER');
    }

    public function testExtensionAndPurePhpParserGenerateTheSameCode(): void
    {
        $fromExtension = $this->generate();

        putenv('ZEPHIR_FORCE_PHP_PARSER=1');
        $fromPurePhp = $this->generate();

        // static::NAME is looked up on the called class at run time.
        $this->assertStringContainsString('zend_get_called_scope(execute_data), SL("NAME")', $fromExtension);
        $this->assertSame($fromPurePhp, $fromExtension);
    }

    private function generate(): string
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/pegrammar';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init pegrammar', $cwd)['exitCode']);
        file_put_contents($projectDir . '/pegrammar/sample.zep', self::SOURCE);

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        return (string) file_get_contents($projectDir . '/ext/pegrammar/sample.zep.c');
    }
}
