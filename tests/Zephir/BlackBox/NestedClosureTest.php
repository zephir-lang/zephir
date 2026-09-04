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
 * A closure declared inside another closure is registered while the compiler is
 * already walking the anonymous classes, so its source file used to be skipped
 * while its `ZEPHIR_INIT()` call still went out: the extension did not link.
 *
 * The assertions below check the invariant rather than a file count: every
 * closure class the module init registers must have a generated source, an
 * include in the project header and an entry in the build files.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2655
 */
final class NestedClosureTest extends TestCase
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

    public function testNestedClosureIsGenerated(): void
    {
        $this->assertEveryRegisteredClosureIsGenerated(
            'nestone',
            <<<'ZEP'
                namespace Nestone;

                class Nest
                {
                    public function plainNested() -> <\Closure>
                    {
                        return function () {
                            return function () {
                                return 1;
                            };
                        };
                    }
                }
                ZEP,
            2,
        );
    }

    /**
     * One extra pass over the queue is not enough: the third level is appended
     * while the second one is being compiled.
     */
    public function testThreeLevelsDeepClosureIsGenerated(): void
    {
        $this->assertEveryRegisteredClosureIsGenerated(
            'nestthree',
            <<<'ZEP'
                namespace Nestthree;

                class Nest
                {
                    public function depth3() -> <\Closure>
                    {
                        return function () {
                            return function () {
                                return function () {
                                    return 3;
                                };
                            };
                        };
                    }
                }
                ZEP,
            3,
        );
    }

    /**
     * `this` inside a nested closure used to resolve against the *outer
     * closure's* synthetic class, which owns no user properties, so the
     * property check threw at generate time.
     */
    public function testNestedClosureResolvesThisAgainstTheEnclosingUserClass(): void
    {
        $this->assertEveryRegisteredClosureIsGenerated(
            'nestthis',
            <<<'ZEP'
                namespace Nestthis;

                class Nest
                {
                    protected name = "probe";

                    public function label() -> string
                    {
                        return "label";
                    }

                    public function nestedThis() -> <\Closure>
                    {
                        return function () {
                            return function () {
                                return this->name . this->label();
                            };
                        };
                    }

                    public function nestedWrite() -> <\Closure>
                    {
                        return function () {
                            return function () {
                                let this->name = "written";

                                return this->name;
                            };
                        };
                    }
                }
                ZEP,
            4,
        );
    }

    /**
     * Scaffolds a throwaway project, drops a single `.zep` in it, runs
     * `zephir generate` and asserts that every closure class registered in the
     * module init has a source file, a header include and a build entry.
     */
    private function assertEveryRegisteredClosureIsGenerated(
        string $project,
        string $source,
        int $expectedClosures,
    ): void {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/nest.zep', $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr'] . $result['stdout']);

        $projectSource = (string) file_get_contents($projectDir . '/ext/' . $project . '.c');
        $projectHeader = (string) file_get_contents($projectDir . '/ext/' . $project . '.h');
        $configM4      = (string) file_get_contents($projectDir . '/ext/config.m4');
        $configW32     = (string) file_get_contents($projectDir . '/ext/config.w32');

        preg_match_all(
            '/ZEPHIR_INIT\(' . preg_quote($project, '/') . '_(\d+__closure)\);/',
            $projectSource,
            $matches,
        );
        $registered = $matches[1];

        $this->assertCount(
            $expectedClosures,
            $registered,
            'Unexpected number of closure classes registered in ' . $project . '.c',
        );

        foreach ($registered as $closure) {
            $this->assertFileExists($projectDir . '/ext/' . $project . '/' . $closure . '.zep.c');
            $this->assertFileExists($projectDir . '/ext/' . $project . '/' . $closure . '.zep.h');
            $this->assertStringContainsString(
                '#include "' . $project . '/' . $closure . '.zep.h"',
                $projectHeader,
            );
            $this->assertStringContainsString($project . '/' . $closure . '.zep.c', $configM4);
            $this->assertStringContainsString($closure . '.zep.c', $configW32);
        }
    }
}
