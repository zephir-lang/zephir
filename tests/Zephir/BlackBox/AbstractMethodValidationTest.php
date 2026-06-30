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
 * A concrete class extending an abstract class must implement every inherited
 * abstract method, otherwise `zephir generate` has to fail at build time
 * instead of letting the engine raise "Cannot instantiate abstract class".
 *
 * @see https://github.com/zephir-lang/zephir/issues/1628
 */
final class AbstractMethodValidationTest extends TestCase
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

    public function testFailsWhenAbstractMethodIsNotImplemented(): void
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/abstractmethod';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init abstractmethod', $cwd)['exitCode']);

        file_put_contents(
            $projectDir . '/abstractmethod/base.zep',
            "namespace Abstractmethod;\n\nabstract class Base\n{\n    abstract public function mustImplement() -> string;\n}\n",
        );
        file_put_contents(
            $projectDir . '/abstractmethod/child.zep',
            "namespace Abstractmethod;\n\nclass Child extends Base\n{\n}\n",
        );

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('mustImplement', $result['stderr']);
        $this->assertStringContainsString('abstract', $result['stderr']);
    }

    public function testSucceedsWhenAbstractMethodIsImplemented(): void
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/abstractmethodok';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init abstractmethodok', $cwd)['exitCode']);

        file_put_contents(
            $projectDir . '/abstractmethodok/base.zep',
            "namespace Abstractmethodok;\n\nabstract class Base\n{\n    abstract public function mustImplement() -> string;\n}\n",
        );
        file_put_contents(
            $projectDir . '/abstractmethodok/child.zep',
            "namespace Abstractmethodok;\n\nclass Child extends Base\n{\n    public function mustImplement() -> string\n    {\n        return \"ok\";\n    }\n}\n",
        );

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
    }
}
