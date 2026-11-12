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

use function file_get_contents;
use function glob;
use function implode;

/**
 * The newest grammar compiles through `ext-zephir_parser` exactly as through
 * the pure-PHP parser. The zept suite always forces the pure-PHP parser, so
 * without this a MINIMUM_PARSER_VERSION that lags the grammar would go
 * unnoticed: the extension would be used and reject the syntax.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2711
 * @see https://github.com/zephir-lang/zephir/issues/2723
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class ParserExtensionGrammarTest extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Pegrammar;

        use DateTime;

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

            public static function classDefault(string name = DateTime::class) -> string
            {
                return name;
            }

            public static function classKeyDefault(array map = [DateTime::class: [self::class]]) -> array
            {
                return map;
            }

            public function closureName() -> var
            {
                return function () {
                    return __FUNCTION__;
                };
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
        $this->assertStringContainsString('zephir_get_called_scope(execute_data), SL("NAME")', $fromExtension);
        // PHP 8.4 names a closure after the line it starts on, not the line it ends on.
        $this->assertStringContainsString('SL("{closure:Pegrammar\\\\Sample::closureName():36}")', $fromExtension);
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

        // A closure is generated into a file of its own, and parameter
        // defaults reach the arginfo header.
        $sources = [];
        foreach (glob($projectDir . '/ext/pegrammar/*.zep.[ch]') ?: [] as $file) {
            $sources[] = $file . "\n" . file_get_contents($file);
        }

        return implode("\n", $sources);
    }
}
