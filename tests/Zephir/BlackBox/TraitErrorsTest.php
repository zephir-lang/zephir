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
 * Compile-time validation of trait misuse: every case here is a fatal error
 * in PHP, so the Zephir compiler must reject it during `generate` instead of
 * producing a broken extension.
 *
 * @see https://github.com/zephir-lang/zephir/issues/504
 */
final class TraitErrorsTest extends TestCase
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

    /**
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, array $files): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        foreach ($files as $fileName => $source) {
            file_put_contents($projectDir . '/' . $name . '/' . $fileName, $source);
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }

    public function testFailsOnNonexistentTrait(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitmissing', [
            'consumer.zep' => "namespace Traitmissing;\n\nclass Consumer\n{\n    use Ghost;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Cannot locate trait', $stderr);
        $this->assertStringContainsString('Ghost', $stderr);
    }

    public function testFailsWhenClassIsUsedAsTrait(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitclassuse', [
            'notatrait.zep' => "namespace Traitclassuse;\n\nclass NotATrait\n{\n}\n",
            'consumer.zep'  => "namespace Traitclassuse;\n\nclass Consumer\n{\n    use NotATrait;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('cannot be used as a trait', $stderr);
        $this->assertStringContainsString('a class', $stderr);
    }

    public function testFailsWhenInterfaceIsUsedAsTrait(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitifaceuse', [
            'notatrait.zep' => "namespace Traitifaceuse;\n\ninterface NotATrait\n{\n}\n",
            'consumer.zep'  => "namespace Traitifaceuse;\n\nclass Consumer\n{\n    use NotATrait;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('cannot be used as a trait', $stderr);
        $this->assertStringContainsString('an interface', $stderr);
    }

    public function testFailsOnConcreteMethodCollision(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitcollision', [
            'a.zep'        => "namespace Traitcollision;\n\ntrait A\n{\n    public function hello() -> string\n    {\n        return \"a\";\n    }\n}\n",
            'b.zep'        => "namespace Traitcollision;\n\ntrait B\n{\n    public function hello() -> string\n    {\n        return \"b\";\n    }\n}\n",
            'consumer.zep' => "namespace Traitcollision;\n\nclass Consumer\n{\n    use A, B;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Trait method collision', $stderr);
    }

    public function testAbstractPlusConcreteMethodIsNotACollision(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitabstractok', [
            'a.zep'        => "namespace Traitabstractok;\n\ntrait A\n{\n    abstract public function hello() -> string;\n}\n",
            'b.zep'        => "namespace Traitabstractok;\n\ntrait B\n{\n    public function hello() -> string\n    {\n        return \"b\";\n    }\n}\n",
            'consumer.zep' => "namespace Traitabstractok;\n\nclass Consumer\n{\n    use A, B;\n}\n",
        ]);

        $this->assertSame(0, $exitCode, $stderr);
    }

    public function testFailsOnPropertyCollision(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitpropcollision', [
            'a.zep'        => "namespace Traitpropcollision;\n\ntrait A\n{\n    protected counter = 1;\n}\n",
            'b.zep'        => "namespace Traitpropcollision;\n\ntrait B\n{\n    protected counter = 2;\n}\n",
            'consumer.zep' => "namespace Traitpropcollision;\n\nclass Consumer\n{\n    use A, B;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Property collision', $stderr);
    }

    public function testIdenticalPropertyFromTwoTraitsIsAllowed(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitpropidentical', [
            'a.zep'        => "namespace Traitpropidentical;\n\ntrait A\n{\n    protected counter = 1;\n}\n",
            'b.zep'        => "namespace Traitpropidentical;\n\ntrait B\n{\n    protected counter = 1;\n}\n",
            'consumer.zep' => "namespace Traitpropidentical;\n\nclass Consumer\n{\n    use A, B;\n}\n",
        ]);

        $this->assertSame(0, $exitCode, $stderr);
    }

    public function testFailsOnConstantCollision(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitconstcollision', [
            'a.zep'        => "namespace Traitconstcollision;\n\ntrait A\n{\n    const LIMIT = 1;\n}\n",
            'b.zep'        => "namespace Traitconstcollision;\n\ntrait B\n{\n    const LIMIT = 2;\n}\n",
            'consumer.zep' => "namespace Traitconstcollision;\n\nclass Consumer\n{\n    use A, B;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Constant collision', $stderr);
    }

    public function testFailsOnCyclicTraitUse(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitcycle', [
            'a.zep' => "namespace Traitcycle;\n\ntrait A\n{\n    use B;\n}\n",
            'b.zep' => "namespace Traitcycle;\n\ntrait B\n{\n    use A;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Cyclic trait use', $stderr);
    }

    public function testFailsWhenTraitAndClassShareOneFile(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitsamefile', [
            'combined.zep' => "namespace Traitsamefile;\n\ntrait Combined\n{\n}\n\nclass Combined2\n{\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('More than one class/interface/trait', $stderr);
    }

    public function testFailsWhenAbstractTraitMethodIsNotImplemented(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitabstractmissing', [
            'a.zep'        => "namespace Traitabstractmissing;\n\ntrait A\n{\n    abstract public function mustImplement() -> string;\n}\n",
            'consumer.zep' => "namespace Traitabstractmissing;\n\nclass Consumer\n{\n    use A;\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('mustImplement', $stderr);
        $this->assertStringContainsString('abstract', $stderr);
    }

    public function testFailsWhenInstantiatingATrait(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitnew', [
            'a.zep'       => "namespace Traitnew;\n\ntrait A\n{\n    public function hello() -> string\n    {\n        return \"a\";\n    }\n}\n",
            'factory.zep' => "namespace Traitnew;\n\nclass Factory\n{\n    public function make()\n    {\n        return new A();\n    }\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Cannot instantiate trait', $stderr);
    }

    public function testFailsOnComplexPropertyDefaultInTrait(): void
    {
        [$exitCode, $stderr] = $this->generateProject('traitarrayprop', [
            'a.zep' => "namespace Traitarrayprop;\n\ntrait A\n{\n    protected items = [];\n}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('array', $stderr);
        $this->assertStringContainsString('trait', $stderr);
    }
}
