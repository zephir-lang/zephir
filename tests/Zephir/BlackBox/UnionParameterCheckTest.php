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

/**
 * A union-typed parameter is fetched as a plain zval, so the generated method
 * checks it against its arg-info, by position, right after parsing.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2761
 */
final class UnionParameterCheckTest extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testEachUnionParameterIsCheckedAtItsPosition(): void
    {
        $generated = $this->generate(<<<'ZEP'
            namespace Unioncheck;

            class Checks
            {
                public function several(string label, int|float amount, var extra, string|bool flag = true)
                {
                    return amount;
                }

                public function plain(int amount, string label)
                {
                    return amount;
                }
            }
            ZEP);

        $this->assertStringContainsString(
            "if (amount && UNEXPECTED(!zephir_verify_union_arg(amount, 2))) {\n\t\tRETURN_THROWS();",
            $generated,
        );
        $this->assertStringContainsString('zephir_verify_union_arg(flag, 4)', $generated);
        $this->assertSame(2, substr_count($generated, 'zephir_verify_union_arg('));
    }

    private function generate(string $source): string
    {
        $projectDir = $this->outputDir() . '/unioncheck';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init unioncheck', $this->outputDir())['exitCode']);

        file_put_contents($projectDir . '/unioncheck/checks.zep', $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        return str_replace("\r\n", "\n", (string) file_get_contents($projectDir . '/ext/unioncheck/checks.zep.c'));
    }
}
