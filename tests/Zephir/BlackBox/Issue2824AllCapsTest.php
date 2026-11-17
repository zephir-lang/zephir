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

use function file_get_contents;
use function function_exists;
use function is_array;
use function putenv;
use function str_replace;
use function zephir_parse_file;

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
        putenv('ZEPHIR_FORCE_PHP_PARSER=1');

        [$output, $generated] = $this->generateUser();

        $this->assertStringContainsString('ZEND_ARG_OBJ_INFO(0, a, Stub\\\\A, 0)', $generated);
        $this->assertStringNotContainsString('ZEPHIR_GET_CONSTANT', $generated);
        $this->assertStringNotContainsString('nonexistent-constant', $output);
    }

    public function testInstanceofAllCapsNameIsAClassCheck(): void
    {
        putenv('ZEPHIR_FORCE_PHP_PARSER=1');
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
        if (!$this->extensionParsesAllCapsCasts()) {
            $this->markTestSkipped('Needs an ext-zephir_parser that parses all-caps class names in casts.');
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

    private function extensionParsesAllCapsCasts(): bool
    {
        if (!function_exists('zephir_parse_file')) {
            return false;
        }

        $ir = zephir_parse_file('class X { public function f(<A> a) -> <A> { } }', '(eval code)');

        return is_array($ir) && 'error' !== ($ir['type'] ?? $ir[0]['type'] ?? null);
    }
}
