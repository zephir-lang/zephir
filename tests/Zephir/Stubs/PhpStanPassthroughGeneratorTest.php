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

namespace Zephir\Test\Stubs;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Os;
use Zephir\Stubs\Generator;

final class PhpStanPassthroughGeneratorTest extends TestCase
{
    private const FIXTURE_DIR = __DIR__ . '/../../fixtures/stubs/phpstan-passthrough/expected';

    public function testShowcaseClassPreservesAllWhitelistedTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $classDocBlock = "Showcase class for PHPStan/Psalm/template tag pass-through.\n"
            . "*\n"
            . "* @template T of \\Phalcon\\Mvc\\ModelInterface\n"
            . "* @extends \\Phalcon\\Mvc\\Model<T>\n"
            . "* @implements \\Foo\\BarInterface<T>\n"
            . "* @phpstan-type FindParams array{\n"
            . "*     conditions?: string,\n"
            . "*     bind?: array<string, mixed>,\n"
            . "*     limit?: int,\n"
            . "* }";

        $classDefinition = new Definition('Stub\\PhpStan', 'PhpStanShowcase');
        $classDefinition->setAliasManager(new AliasManager());
        $classDefinition->setDocBlock($classDocBlock);

        $findParams = new Parameters([
            [
                'type'      => 'parameter',
                'name'      => 'parameters',
                'const'     => 0,
                'data-type' => 'array',
                'mandatory' => 0,
            ],
        ]);

        $findDocBlock = "/**\n"
            . " * Find records matching parameters.\n"
            . " *\n"
            . " * @phpstan-return \\Phalcon\\Mvc\\Model\\Resultset\\Simple<array-key, static>\n"
            . " * @psalm-param array<string, mixed> \$parameters\n"
            . " */";

        $findReturnType = [
            'type' => 'return-type',
            'list' => [
                ['type' => 'return-type-parameter', 'data-type' => 'array', 'mandatory' => 0],
            ],
            'void' => 0,
        ];

        $findMethod = new Method(
            $classDefinition,
            ['public', 'static'],
            'find',
            $findParams,
            null,
            $findDocBlock,
            $findReturnType
        );

        $findFirstDocBlock = "/**\n"
            . " * Find the first record matching parameters.\n"
            . " *\n"
            . " * @phpstan-return static|null\n"
            . " */";

        $findFirstParams = new Parameters([
            [
                'type'      => 'parameter',
                'name'      => 'parameters',
                'const'     => 0,
                'data-type' => 'array',
                'mandatory' => 0,
            ],
        ]);

        $findFirstMethod = new Method(
            $classDefinition,
            ['public', 'static'],
            'findFirst',
            $findFirstParams,
            null,
            $findFirstDocBlock
        );

        $classDefinition->setMethod('find', $findMethod);
        $classDefinition->setMethod('findFirst', $findFirstMethod);

        $generator  = new Generator([]);
        $reflection = new \ReflectionClass(Generator::class);
        $buildClass = $reflection->getMethod('buildClass');
        $buildClass->setAccessible(true);

        $actual = $buildClass->invokeArgs($generator, [$classDefinition, '    ', '']);

        $expected = file_get_contents(self::FIXTURE_DIR . '/PhpStanShowcase.php');

        $this->assertSame($expected, $actual);
    }

    public function testNonWhitelistedTagsStillPassThrough(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $classDocBlock = "Class with non-whitelisted custom tags.\n"
            . "*\n"
            . "* @phan-template T\n"
            . "* @my-custom-tag something";

        $classDefinition = new Definition('Stub\\PhpStan', 'PhpStanNegative');
        $classDefinition->setAliasManager(new AliasManager());
        $classDefinition->setDocBlock($classDocBlock);

        $methodDocBlock = "/**\n"
            . " * Method with non-whitelisted tags.\n"
            . " *\n"
            . " * @phan-return Foo\n"
            . " * @my-internal-marker\n"
            . " */";

        $method = new Method(
            $classDefinition,
            ['public'],
            'doStuff',
            null,
            null,
            $methodDocBlock
        );

        $classDefinition->setMethod('doStuff', $method);

        $generator  = new Generator([]);
        $reflection = new \ReflectionClass(Generator::class);
        $buildClass = $reflection->getMethod('buildClass');
        $buildClass->setAccessible(true);

        $actual   = $buildClass->invokeArgs($generator, [$classDefinition, '    ', '']);
        $expected = file_get_contents(self::FIXTURE_DIR . '/PhpStanNegative.php');

        $this->assertSame($expected, $actual);
    }
}
