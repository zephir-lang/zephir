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
 * A method declared anywhere in the interface inheritance chain must resolve,
 * not only when the class implements the declaring interface directly.
 *
 * A Zephir interface stores its parents in the same slot as a class stores its
 * `implements` list, while method lookup walked the `extends` chain only — a
 * chain that is always empty for an interface. So `class C implements Outer`
 * with `interface Outer extends Inner` could not see `Inner::go()`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2635
 */
final class Issue2635Test extends TestCase
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
     * The case reported in the issue: one level of interface inheritance.
     */
    public function testMethodFromParentInterfaceResolves(): void
    {
        [$exitCode, $stderr] = $this->generateProject('ifaceparent', [
            'inner.zep' => <<<'ZEP'
                namespace Ifaceparent;

                interface Inner
                {
                    public function go();
                }
                ZEP,
            'outer.zep' => <<<'ZEP'
                namespace Ifaceparent;

                interface Outer extends Inner
                {
                }
                ZEP,
            'base.zep' => <<<'ZEP'
                namespace Ifaceparent;

                abstract class Base implements Outer
                {
                    public function run()
                    {
                        return this->go();
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * Two levels up, so the lookup has to recurse rather than just take one
     * extra hop.
     */
    public function testMethodFromGrandparentInterfaceResolves(): void
    {
        [$exitCode, $stderr] = $this->generateProject('ifacegrand', [
            'top.zep' => <<<'ZEP'
                namespace Ifacegrand;

                interface Top
                {
                    public function go();
                }
                ZEP,
            'middle.zep' => <<<'ZEP'
                namespace Ifacegrand;

                interface Middle extends Top
                {
                }
                ZEP,
            'bottom.zep' => <<<'ZEP'
                namespace Ifacegrand;

                interface Bottom extends Middle
                {
                }
                ZEP,
            'base.zep' => <<<'ZEP'
                namespace Ifacegrand;

                abstract class Base implements Bottom
                {
                    public function run()
                    {
                        return this->go();
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * An interface may extend several interfaces at once; every branch has to
     * be searched.
     */
    public function testMethodFromMultipleParentInterfacesResolves(): void
    {
        [$exitCode, $stderr] = $this->generateProject('ifacemulti', [
            'firstinner.zep' => <<<'ZEP'
                namespace Ifacemulti;

                interface FirstInner
                {
                    public function first();
                }
                ZEP,
            'secondinner.zep' => <<<'ZEP'
                namespace Ifacemulti;

                interface SecondInner
                {
                    public function second();
                }
                ZEP,
            'outer.zep' => <<<'ZEP'
                namespace Ifacemulti;

                interface Outer extends FirstInner, SecondInner
                {
                }
                ZEP,
            'base.zep' => <<<'ZEP'
                namespace Ifacemulti;

                abstract class Base implements Outer
                {
                    public function run()
                    {
                        return this->first() . this->second();
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $exitCode, $stderr);
    }

    /**
     * Control case. A concrete class must satisfy the whole chain, otherwise
     * the engine raises a fatal error when the module registers the class.
     */
    public function testConcreteClassMustImplementInheritedInterfaceMethod(): void
    {
        [$exitCode, $stderr] = $this->generateProject('ifaceconcrete', [
            'inner.zep' => <<<'ZEP'
                namespace Ifaceconcrete;

                interface Inner
                {
                    public function go();
                }
                ZEP,
            'outer.zep' => <<<'ZEP'
                namespace Ifaceconcrete;

                interface Outer extends Inner
                {
                }
                ZEP,
            'impl.zep' => <<<'ZEP'
                namespace Ifaceconcrete;

                class Impl implements Outer
                {
                }
                ZEP,
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('must implement a method called', $stderr);
        $this->assertStringContainsString('go', $stderr);
    }

    /**
     * Control case. Widening the search must not make every method resolvable:
     * a method that exists nowhere in the chain is still an error.
     */
    public function testUnknownMethodStillErrors(): void
    {
        [$exitCode, $stderr] = $this->generateProject('ifaceunknown', [
            'inner.zep' => <<<'ZEP'
                namespace Ifaceunknown;

                interface Inner
                {
                    public function go();
                }
                ZEP,
            'outer.zep' => <<<'ZEP'
                namespace Ifaceunknown;

                interface Outer extends Inner
                {
                }
                ZEP,
            'base.zep' => <<<'ZEP'
                namespace Ifaceunknown;

                abstract class Base implements Outer
                {
                    public function run()
                    {
                        return this->nowhere();
                    }
                }
                ZEP,
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('does not implement method', $stderr);
        $this->assertStringContainsString('nowhere', $stderr);
    }

    /**
     * @param array<string, string> $files
     *
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, array $files): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        foreach ($files as $fileName => $source) {
            file_put_contents($projectDir . '/' . $name . '/' . $fileName, $source . "\n");
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }
}
