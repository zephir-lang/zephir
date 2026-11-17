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

use function putenv;

/**
 * The compile time rules PHP applies to named arguments, plus the one Zephir
 * adds: a built-in function that only exists in Zephir has no parameter names
 * to bind them to.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824NamedArgumentsTest extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();

        // A keyword as an argument name needs a parser newer than ext-zephir_parser 2.11.0.
        putenv('ZEPHIR_FORCE_PHP_PARSER=1');
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
        putenv('ZEPHIR_FORCE_PHP_PARSER');
    }

    /**
     * @dataProvider invalidCallProvider
     */
    public function testInvalidCallIsACompileError(string $call, string $message): void
    {
        $result = $this->generate(['named.zep' => $this->source($call)]);

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString($message, $result['stdout'] . $result['stderr']);
    }

    public static function invalidCallProvider(): array
    {
        return [
            'positional after named' => [
                'this->pair(a: 1, 2)',
                'Cannot use positional argument after named argument',
            ],
            'duplicate name' => [
                'this->pair(a: 1, a: 2)',
                'Named parameter $a overwrites previous argument',
            ],
            'unknown name of a known method' => [
                'this->pair(zz: 1)',
                'Named parameter "zz" is not a valid parameter name, available: a, b',
            ],
            'zephir-only built-in function' => [
                'camelize(text: "a_b")',
                'Named arguments cannot be passed to the built-in function camelize()',
            ],
        ];
    }

    /**
     * @dataProvider validCallProvider
     */
    public function testValidCallCompiles(string $call): void
    {
        $this->generateOk('named.zep', $this->source($call));
    }

    public static function validCallProvider(): array
    {
        return [
            'positional then named'       => ['this->pair(1, b: 2)'],
            'unknown name of a variadic'  => ['this->variadic(1, zz: 2)'],
            'any name of an unknown call' => ['other->anything(zz: 1)'],
            'optimized php function'      => ['strlen(string: "ab")'],
        ];
    }

    public function testNamedArgumentsGoToTheEngineByName(): void
    {
        $generated = $this->generateOk('named.zep', $this->source('strlen(string: "ab") + strlen("ab")'));

        $this->assertStringContainsString('SL("string"), ', $generated);
        $this->assertStringContainsString('ZEPHIR_CALL_FUNCTION_NAMED(', $generated);
        $this->assertStringContainsString('zephir_fast_strlen_ev(', $generated);
    }

    private function source(string $call): string
    {
        return <<<ZEP
            namespace Stub;

            class Named
            {
                public function pair(var a, var b = null)
                {
                    return a;
                }

                public function variadic(var a, var ...rest)
                {
                    return rest;
                }

                public function call(var other)
                {
                    return {$call};
                }
            }
            ZEP;
    }
}
