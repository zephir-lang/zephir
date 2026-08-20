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
 * A definition that `extends`/`implements` a bundled interface living in a PHP
 * extension header must get that header `#include`d next to the
 * `zend_class_implements()` call that uses its class entry, otherwise the
 * generated C does not compile.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2427
 */
final class InterfaceHeadersTest extends TestCase
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

    public function testInterfaceExtendingBundledInterfacesIncludesTheirHeaders(): void
    {
        $generated = $this->generate(
            'ifacehdrs',
            'aninterface.zep',
            <<<'ZEP'
                namespace Ifacehdrs;

                interface AnInterface extends \JsonSerializable, \SplObserver
                {
                }
                ZEP,
        );

        $this->assertStringContainsString('php_json_serializable_ce', $generated);
        $this->assertStringContainsString('spl_ce_SplObserver', $generated);

        $this->assertStringContainsString('#include "ext/json/php_json.h"', $generated);
        $this->assertStringContainsString('#include "ext/spl/spl_observer.h"', $generated);
    }

    public function testClassImplementingBundledInterfacesIncludesTheirHeaders(): void
    {
        $generated = $this->generate(
            'clshdrs',
            'aclass.zep',
            <<<'ZEP'
                namespace Clshdrs;

                class AClass implements \JsonSerializable, \SplObserver
                {
                    public function jsonSerialize() -> array
                    {
                        return [];
                    }

                    public function update(<\SplSubject> subject) -> void
                    {
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('php_json_serializable_ce', $generated);
        $this->assertStringContainsString('spl_ce_SplObserver', $generated);

        $this->assertStringContainsString('#include "ext/json/php_json.h"', $generated);
        $this->assertStringContainsString('#include "ext/spl/spl_observer.h"', $generated);
    }

    /**
     * Scaffolds a throwaway project, drops a single `.zep` in it, runs
     * `zephir generate` and returns the generated C source.
     */
    private function generate(string $project, string $fileName, string $source): string
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/' . $fileName, $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf(
            '%s/ext/%s/%s.c',
            $projectDir,
            $project,
            basename($fileName, '.zep') . '.zep',
        );
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }
}
