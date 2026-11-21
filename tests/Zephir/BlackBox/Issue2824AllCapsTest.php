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
use Zephir\Parser\Manager;

use function file_get_contents;
use function phpversion;
use function putenv;
use function str_replace;

/**
 * A class whose name is all capitals, such as `A` or `PDO`, is a class name
 * everywhere one is written: in a cast, a return type, a type hint, `catch`
 * and on the right of `instanceof`, which PHP never reads as a constant.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824AllCapsTest extends TestCase
{
    use GeneratesStubProject;

    private const CLASS_A = <<<'ZEP'
        namespace Stub;

        class A
        {
            public function hello() -> string
            {
                return "a";
            }
        }
        ZEP;

    private const CLASS_B = <<<'ZEP'
        namespace Stub;

        class B
        {
        }
        ZEP;

    private const CLASS_ERR = <<<'ZEP'
        namespace Stub;

        class ERR extends \Exception
        {
        }
        ZEP;

    private const USER = <<<'ZEP'
        namespace Stub;

        use PDO;

        class User
        {
            protected <A> held;

            protected ?<B> maybe = null;

            public function param(<A> a) -> string
            {
                return a->hello();
            }

            public function optional(<A> a = null) -> bool
            {
                return a === null;
            }

            public function union(<A> | <B> value) -> bool
            {
                return value instanceof A;
            }

            public function returns() -> <A>
            {
                return new A();
            }

            public function returnsMany() -> <A[]>
            {
                return [new A()];
            }

            public function returnsUnion() -> <A> | <B>
            {
                return new B();
            }

            public function closure() -> var
            {
                return function (<A> a) {
                    return a->hello();
                };
            }

            public function hinted(var value) -> string
            {
                var a;

                let a = <A> value;

                return a->hello();
            }

            public function caught() -> string
            {
                var e;

                try {
                    throw new ERR("caught");
                } catch ERR, e {
                    return e->getMessage();
                }
            }

            public function caughtUnion() -> string
            {
                var e;

                try {
                    throw new ERR("union");
                } catch \LogicException | ERR, e {
                    return e->getMessage();
                }
            }

            public function isA(var value) -> bool
            {
                return value instanceof A;
            }

            public function isPdo(var value) -> bool
            {
                return value instanceof PDO;
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
        putenv('ZEPHIR_FORCE_PHP_PARSER');
    }

    public function testAllCapsClassNameCompilesEverywhere(): void
    {
        [$output, $generated] = $this->generateUser();

        $this->assertStringContainsString('ZEND_ARG_OBJ_INFO(0, a, Stub\\\\A, 0)', $generated);
        $this->assertStringNotContainsString('ZEPHIR_GET_CONSTANT', $generated);
        $this->assertStringNotContainsString('nonexistent-constant', $output);
    }

    public function testInstanceofAllCapsNameIsAClassCheck(): void
    {
        $this->generateUser();
        $allCaps = (string) file_get_contents($this->projectDir . '/ext/stub/user.zep.c');

        $this->generate([
            'user.zep' => str_replace(
                ['value instanceof A;', 'value instanceof PDO;'],
                ['value instanceof \Stub\A;', 'value instanceof \PDO;'],
                self::USER
            ),
        ]);
        $qualified = (string) file_get_contents($this->projectDir . '/ext/stub/user.zep.c');

        $this->assertSame($qualified, $allCaps);
    }

    public function testExtensionParserAgrees(): void
    {
        $version = phpversion('zephir_parser');
        if (!Manager::isExtensionUsable($version === false ? null : $version)) {
            $this->markTestSkipped('Needs ext-zephir_parser >= ' . Manager::MINIMUM_PARSER_VERSION . '.');
        }

        [, $fromExtension] = $this->generateUser();

        putenv('ZEPHIR_FORCE_PHP_PARSER=1');
        [, $fromPurePhp] = $this->generateUser();

        $this->assertSame($fromPurePhp, $fromExtension);
    }

    /**
     * @return array{0: string, 1: string} the console output and the generated C
     */
    private function generateUser(): array
    {
        $result = $this->generate([
            'a.zep'    => self::CLASS_A,
            'b.zep'    => self::CLASS_B,
            'err.zep'  => self::CLASS_ERR,
            'user.zep' => self::USER,
        ]);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertSame(0, $result['exitCode'], $output);

        return [
            $output,
            file_get_contents($this->projectDir . '/ext/stub/user.zep.c')
                . file_get_contents($this->projectDir . '/ext/stub/user.zep.h'),
        ];
    }
}
