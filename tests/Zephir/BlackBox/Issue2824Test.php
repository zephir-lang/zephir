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
 * A method's return type names a class as written in the file that declares
 * the method, so a call made from another file resolves it there: through
 * that file's namespace and `use` aliases, not the caller's.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824Test extends TestCase
{
    use GeneratesStubProject;

    private const SOME_CLASS = <<<'ZEP'
        namespace Stub\Full\Path;

        class SomeClass
        {
            public function hello() -> string
            {
                return "hi";
            }

            public static function make() -> <SomeClass>
            {
                return new SomeClass();
            }
        }
        ZEP;

    private const HOLDER = <<<'ZEP'
        namespace Stub\Myapp;

        use Stub\Full\Path\SomeClass;

        class Holder
        {
            public function getX() -> <SomeClass>
            {
                return new SomeClass();
            }

            public static function make() -> <SomeClass>
            {
                return new SomeClass();
            }

            public static function create() -> <static>
            {
                return new static();
            }

            public static function createSelf() -> <self>
            {
                return new self();
            }

            public function hello() -> string
            {
                return "holder";
            }
        }
        ZEP;

    private const CALLER = <<<'ZEP'
        namespace Stub\Other;

        use Stub\Myapp\Holder;

        class Caller
        {
            public function viaCollaborator(<Holder> holder) -> string
            {
                return holder->getX()->hello();
            }
        }
        ZEP;

    private const CHILD = <<<'ZEP'
        namespace Stub\Myapp;

        class C extends Holder
        {
            public function chained() -> string
            {
                return this->getX()->hello();
            }
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testCallFromAnotherFileResolvesReturnTypeWhereItIsDeclared(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'myapp/holder.zep'        => self::HOLDER,
            'other/caller.zep'        => self::CALLER,
            'myapp/c.zep'             => self::CHILD,
        ]);
    }

    public function testSameShortNameInCallerNamespaceIsNotPicked(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'myapp/holder.zep'        => self::HOLDER,
            'other/caller.zep'        => self::CALLER,
            'other/someclass.zep'     => <<<'ZEP'
                namespace Stub\Other;

                class SomeClass
                {
                    private function hidden()
                    {
                    }
                }
                ZEP,
        ]);
    }

    public function testStaticCallResolvesReturnTypeWhereItIsDeclared(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'myapp/holder.zep'        => self::HOLDER,
            'other/caller.zep'        => <<<'ZEP'
                namespace Stub\Other;

                use Stub\Myapp\Holder;

                class Caller
                {
                    public function viaStatic() -> string
                    {
                        return Holder::make()->hello();
                    }

                    public function viaStaticReturn() -> string
                    {
                        return Holder::create()->hello();
                    }

                    public function viaSelfReturn() -> string
                    {
                        return Holder::createSelf()->hello();
                    }
                }
                ZEP,
        ]);
    }

    public function testParentStaticCallReturningStaticKeepsTheCallingClass(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'myapp/holder.zep'        => self::HOLDER,
            'myapp/child.zep'         => <<<'ZEP'
                namespace Stub\Myapp;

                class Child extends Holder
                {
                    public static function create() -> <static>
                    {
                        return parent::create();
                    }

                    public function own() -> string
                    {
                        return "child";
                    }

                    public static function chained() -> string
                    {
                        return parent::create()->own();
                    }
                }
                ZEP,
        ]);
    }

    public function testTraitMethodResolvesReturnTypeInTheTraitFile(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'traits/makes.zep'        => <<<'ZEP'
                namespace Stub\Traits;

                use Stub\Full\Path\SomeClass;

                trait Makes
                {
                    public function getX() -> <SomeClass>
                    {
                        return new SomeClass();
                    }
                }
                ZEP,
            'myapp/maker.zep'         => <<<'ZEP'
                namespace Stub\Myapp;

                class Maker
                {
                    use \Stub\Traits\Makes;
                }
                ZEP,
            'other/caller.zep'        => <<<'ZEP'
                namespace Stub\Other;

                use Stub\Myapp\Maker;

                class Caller
                {
                    public function viaTrait(<Maker> maker) -> string
                    {
                        return maker->getX()->hello();
                    }
                }
                ZEP,
        ]);
    }

    public function testInterfaceResolvesReturnTypeInTheInterfaceFile(): void
    {
        $this->assertCompilesWithoutNonexistentClass([
            'full/path/someclass.zep' => self::SOME_CLASS,
            'contracts/holds.zep'     => <<<'ZEP'
                namespace Stub\Contracts;

                use Stub\Full\Path\SomeClass;

                interface Holds
                {
                    public function getX() -> <SomeClass>;
                }
                ZEP,
            'other/caller.zep'        => <<<'ZEP'
                namespace Stub\Other;

                use Stub\Contracts\Holds;

                class Caller
                {
                    public function viaInterface(<Holds> holder) -> string
                    {
                        return holder->getX()->hello();
                    }
                }
                ZEP,
        ]);
    }

    public function testMissingReturnClassIsReportedByItsDeclaredName(): void
    {
        $result = $this->generate([
            'contracts/holds.zep' => <<<'ZEP'
                namespace Stub\Contracts;

                use Stub\Nowhere\Missing;

                interface Holds
                {
                    public function getX() -> <Missing>;
                }
                ZEP,
            'other/caller.zep'    => <<<'ZEP'
                namespace Stub\Other;

                use Stub\Contracts\Holds;

                class Caller
                {
                    public function viaInterface(<Holds> holder) -> string
                    {
                        return holder->getX()->hello();
                    }
                }
                ZEP,
        ]);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('Class "Stub\Nowhere\Missing" does not exist at compile time', $output);
        $this->assertStringNotContainsString('Stub\Other\Missing', $output);
    }

    /**
     * @param array<string, string> $files
     */
    private function assertCompilesWithoutNonexistentClass(array $files): void
    {
        $result = $this->generate($files);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('nonexistent-class', $output);
    }
}
