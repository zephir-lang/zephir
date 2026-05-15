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
 * Equivalent of sharness `t0002-generate.sh`.
 */
final class GenerateCommandTest extends TestCase
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

    public function testLifeCycleGenerate(): void
    {
        $cwd = $this->fixturesDir() . '/lifecycle';
        $this->cleanupPath($cwd . '/.zephir');
        $this->cleanupPath($cwd . '/ext');

        $this->assertSame(0, $this->runZephir('fullclean', $cwd)['exitCode']);
        $this->assertSame(0, $this->runZephir('generate', $cwd)['exitCode']);

        $this->assertFileEquals(
            $cwd . '/expected3.c',
            $cwd . '/ext/lifecycle.c',
        );
    }
}
