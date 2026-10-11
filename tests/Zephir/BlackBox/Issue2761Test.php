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
 * A `{ get, set }` shortcut types its accessors from the property's declared
 * type, falling back to a `@var` docblock, and a class named by either
 * becomes a class type exactly as a hand-written `-> <Class>` would.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2761
 */
final class Issue2761Test extends TestCase
{
    use RunsZephirCommands;

    private const COLLABORATOR = <<<'ZEP'
        namespace Probe;

        class Collaborator
        {
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testDeclaredScalarTypeTypesBothAccessors(): void
    {
        $stub = $this->stubFor('ScalarHint', <<<'ZEP'
            namespace Probe;

            class ScalarHint
            {
                protected string name = "" { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getName(): string', $stub);
        $this->assertStringContainsString('public function setName(string $name)', $stub);
    }

    public function testDeclaredClassTypeTypesBothAccessors(): void
    {
        $stub = $this->stubFor('ProposedSyntax', <<<'ZEP'
            namespace Probe;

            class ProposedSyntax
            {
                protected <Collaborator> thing { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getThing(): Collaborator', $stub);
        $this->assertStringContainsString('public function setThing(Collaborator $thing)', $stub);
    }

    public function testDeclaredNullableClassTypeTypesBothAccessors(): void
    {
        $stub = $this->stubFor('NullableSyntax', <<<'ZEP'
            namespace Probe;

            class NullableSyntax
            {
                protected ?<Collaborator> thing = null { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getThing(): Collaborator|null', $stub);
        $this->assertStringContainsString('public function setThing(Collaborator|null $thing)', $stub);
    }

    public function testDeclaredUnionTypeTypesBothAccessors(): void
    {
        $stub = $this->stubFor('UnionSyntax', <<<'ZEP'
            namespace Probe;

            class UnionSyntax
            {
                protected string|int key = "" { get, set };
            }
            ZEP);

        $this->assertMatchesRegularExpression('/public function getKey\(\): (string\|int|int\|string)/', $stub);
        $this->assertMatchesRegularExpression('/public function setKey\((string\|int|int\|string) \$key\)/', $stub);
    }

    public function testScalarDocblockStillTypesBothAccessors(): void
    {
        $stub = $this->stubFor('ScalarDoc', <<<'ZEP'
            namespace Probe;

            class ScalarDoc
            {
                /** @var string */
                protected name { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getName(): string', $stub);
        $this->assertStringContainsString('public function setName(string $name)', $stub);
    }

    public function testQualifiedClassDocblockTypesTheGetter(): void
    {
        $stub = $this->stubFor('DocblockHint', <<<'ZEP'
            namespace Probe;

            class DocblockHint
            {
                /** @var \Probe\Collaborator */
                protected thing { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getThing(): \Probe\Collaborator', $stub);
        $this->assertStringContainsString('public function setThing(\Probe\Collaborator $thing)', $stub);
    }

    public function testRelativeClassDocblockTypesTheGetter(): void
    {
        $stub = $this->stubFor('RelativeDocblockHint', <<<'ZEP'
            namespace Probe;

            class RelativeDocblockHint
            {
                /** @var Collaborator */
                protected thing { get };
            }
            ZEP);

        $this->assertStringContainsString('public function getThing(): Collaborator', $stub);
    }

    public function testDeclaredTypeWinsOverDocblock(): void
    {
        $stub = $this->stubFor('DeclaredWins', <<<'ZEP'
            namespace Probe;

            class DeclaredWins
            {
                /** @var Collaborator */
                protected int count = 0 { get, set };
            }
            ZEP);

        $this->assertStringContainsString('public function getCount(): int', $stub);
        $this->assertStringContainsString('public function setCount(int $count)', $stub);
    }

    public function testClassTypedGetterEmitsObjectReturnArgInfo(): void
    {
        $this->generateProject('ProposedSyntax', <<<'ZEP'
            namespace Probe;

            class ProposedSyntax
            {
                protected <Collaborator> thing { get };
            }
            ZEP);

        $header = (string) file_get_contents(
            $this->outputDir() . '/probe/ext/probe/proposedsyntax.zep.h'
        );

        $this->assertStringContainsString(
            'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_probe_proposedsyntax_getthing, 0, 0, Probe\\\\Collaborator, 0)',
            $header,
        );
    }

    /**
     * Scaffolds a `probe` project holding `Collaborator` and the given class,
     * runs `zephir generate` + `zephir stubs`, and returns the class's stub.
     */
    private function stubFor(string $className, string $source): string
    {
        $projectDir = $this->generateProject($className, $source);

        $result = $this->runZephir('stubs --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $stubFile = sprintf('%s/ide/0.0.1/Probe/%s.php', $projectDir, $className);
        $this->assertFileExists($stubFile);

        return (string) file_get_contents($stubFile);
    }

    private function generateProject(string $className, string $source): string
    {
        $projectDir = $this->outputDir() . '/probe';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init probe', $this->outputDir())['exitCode']);

        file_put_contents($projectDir . '/probe/collaborator.zep', self::COLLABORATOR . "\n");
        file_put_contents($projectDir . '/probe/' . strtolower($className) . '.zep', $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        return $projectDir;
    }
}
