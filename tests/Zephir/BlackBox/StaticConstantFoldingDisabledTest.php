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
use Zephir\Os;

/**
 * `static-constant-class-folding` is on by default, and with it on a class
 * constant is inlined as a literal, so `StaticConstantAccess`'s dynamic branch
 * never runs. Turning the option off is the only way to reach it, and that
 * branch emitted `zephir_get_class_constant()`, which no kernel header
 * declared, into a bare local rather than its address. Every project that
 * turned the option off therefore failed to build, and no test in the suite
 * turned it off.
 *
 * The compile step is the assertion that matters: a declaration alone would
 * not catch a missing definition, and the previous emission would not have
 * compiled even if the function had existed.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2711
 *
 * @group slow
 */
final class StaticConstantFoldingDisabledTest extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('compile invokes phpize/configure/make, which are not available on Windows.');
        }

        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testAProjectWithConstantFoldingDisabledBuildsAndReadsTheConstant(): void
    {
        $project    = 'constfold';
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        $this->disableConstantFolding($projectDir . '/config.json');

        file_put_contents(
            $projectDir . '/' . $project . '/marker.zep',
            <<<'ZEP'
                namespace Constfold;

                interface Marker
                {
                    const FROM_INTERFACE = "interface";
                }

                ZEP,
        );

        file_put_contents(
            $projectDir . '/' . $project . '/base.zep',
            <<<'ZEP'
                namespace Constfold;

                class Base
                {
                    const FROM_PARENT = "parent";
                }

                ZEP,
        );

        file_put_contents(
            $projectDir . '/' . $project . '/reader.zep',
            <<<'ZEP'
                namespace Constfold;

                class Reader extends Base implements Marker
                {
                    const OWN = "own";

                    public function own()
                    {
                        return self::OWN;
                    }

                    public function fromParent()
                    {
                        return self::FROM_PARENT;
                    }

                    public function fromNamedClass()
                    {
                        return Base::FROM_PARENT;
                    }

                    public function fromInterface()
                    {
                        return self::FROM_INTERFACE;
                    }

                    public function intoLocal()
                    {
                        var value;

                        let value = self::OWN;

                        return value;
                    }
                }

                ZEP,
        );

        $generate = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $generate['exitCode'], $generate['stderr']);

        $generated = (string) file_get_contents($projectDir . '/ext/' . $project . '/reader.zep.c');
        $this->assertStringContainsString(
            'zephir_get_class_constant(return_value,',
            $generated,
            'With folding off the constant has to be read at run time rather than inlined.',
        );

        /**
         * A returned constant lands in `return_value`, which is already a
         * `zval *`, so the bare name happened to be right there and the defect
         * only shows on a local destination.
         */
        $this->assertStringContainsString(
            'zephir_get_class_constant(&value,',
            $generated,
            'A local destination is a zval, so the emitter has to pass its address.',
        );

        $compile = $this->runZephir('compile --no-dev', $projectDir);
        $this->assertSame(
            0,
            $compile['exitCode'],
            "compile failed with constant folding disabled:\n" . $compile['stdout'] . $compile['stderr'],
        );

        $this->assertSame(
            '["own","parent","parent","interface","own"]',
            $this->readConstantsThroughTheExtension($projectDir),
            'The run-time read must produce the same values the folded read inlines.',
        );
    }

    /**
     * `zephir init` writes the default optimization set, which has the option
     * on. Rewriting the file keeps the rest of the generated config intact.
     */
    private function disableConstantFolding(string $configFile): void
    {
        $config = json_decode((string) file_get_contents($configFile), true);
        $this->assertIsArray($config);

        $config['optimizations']['static-constant-class-folding'] = false;

        file_put_contents($configFile, (string) json_encode($config, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES));
    }

    private function readConstantsThroughTheExtension(string $projectDir): string
    {
        $modules = glob($projectDir . '/ext/modules/*.so');
        $this->assertNotEmpty($modules, 'compile reported success but produced no extension.');

        $code = '$r = new \\Constfold\\Reader();'
            . ' echo json_encode([$r->own(), $r->fromParent(), $r->fromNamedClass(), $r->fromInterface(), $r->intoLocal()]);';

        $command = sprintf(
            '%s -n -d extension=%s -r %s 2>&1',
            escapeshellarg($this->phpBin()),
            escapeshellarg((string) $modules[0]),
            escapeshellarg($code),
        );

        return trim((string) shell_exec($command));
    }
}
