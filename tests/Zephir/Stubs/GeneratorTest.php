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
use Zephir\Class\Constant;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Class\Property;
use Zephir\Os;
use Zephir\Stubs\Generator;

class GeneratorTest extends TestCase
{
    /**
     * @var mixed
     */
    private $generatorClass;
    private Generator $testClass;
    private Definition $classDefinition;

    protected function setUp(): void
    {
        $this->generatorClass = new \ReflectionClass(Generator::class);
        $this->testClass = new Generator([]);
        $this->classDefinition = new Definition('Stub\Stubs', 'StubsBuildClass');
    }

    /**
     * Modify method visibility to call protected.
     *
     * @param string $name - method name
     *
     * @return mixed
     *
     * @throws \ReflectionException
     */
    private function getMethod(string $name)
    {
        $method = $this->generatorClass->getMethod($name);
        $method->setAccessible(true);

        return $method;
    }

    public function testShouldBuildClass(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $expected = <<<DOC
            <?php

            namespace Stub\Stubs;

            use Stub\Extendable\BaseTestClass;
            use Stub\Events\EventInterface as EventsManagerInterface;

            /**
             * Class description example
             */
            final class StubsBuildClass extends BaseTestClass implements \Iterator, EventsManagerInterface
            {
                /**
                 * Default path delimiter
                 */
                const DEFAULT_PATH_DELIMITER = '.';

                /**
                 * Default path delimiter class property
                 */
                static public \$defaultPathDelimiter = null;

                /**
                 * @param string \$key
                 * @param int \$priority
                 */
                public static function init(string \$key, int \$priority = 1)
                {
                }
            }

            DOC;

        // Test requirements initialization

        $buildClass = $this->getMethod('buildClass');

        $extendsClassDefinition = new Definition('Stub\Extendable', 'BaseTestClass');
        $implementClassDefinition = new Definition('Stub\Events', 'EventsManagerInterface');
        $aliasManager = new AliasManager();

        // Definitions

        $methodParamsDefinition = [
            [
                'type' => 'parameter',
                'name' => 'key',
                'const' => 0,
                'data-type' => 'string',
                'mandatory' => 0,
            ],
            [
                'type' => 'parameter',
                'name' => 'priority',
                'const' => 0,
                'data-type' => 'int',
                'mandatory' => 0,
                'default' => [
                    'type' => 'int',
                    'value' => 1,
                ],
            ],
        ];

        $classMethod = new Method(
            $this->classDefinition,
            ['public', 'static'],
            'init',
            new Parameters($methodParamsDefinition)
        );

        $constantsDefinition = new Constant(
            'DEFAULT_PATH_DELIMITER',
            [
                'type' => 'string',
                'value' => '.',
            ],
            'Default path delimiter'
        );

        $propertyDefinition = new Property(
            $this->classDefinition,
            ['public', 'static'],
            'defaultPathDelimiter',
            [
                'type' => 'null',
                'value' => null,
            ],
            'Default path delimiter class property',
            [
                'default' => [
                    'type' => 'null',
                    'value' => null,
                ],
            ]
        );

        // Inject definitions and construct test Class

        $aliasManager->add([
            'aliases' => [
                [
                    'name' => 'Stub\\Extendable\\BaseTestClass',
                ],
                [
                    'name' => 'Stub\\Events\\EventInterface',
                    'alias' => 'EventsManagerInterface',
                ],
            ],
        ]);

        $implementClassDefinition->setAliasManager($aliasManager);
        $this->classDefinition->setAliasManager($aliasManager);
        $this->classDefinition->setDocBlock('Class description example');
        $this->classDefinition->setIsFinal(true);
        $this->classDefinition->setExtendsClassDefinition($extendsClassDefinition);
        $this->classDefinition->setExtendsClass('BaseTestClass');
        $this->classDefinition->setImplementedInterfaceDefinitions([
            $implementClassDefinition,
        ]);
        $this->classDefinition->setImplementsInterfaces([
            [
                'value' => '\Iterator',
            ],
            [
                'value' => 'Stub\\Events\\EventInterface',
            ],
        ]);
        $this->classDefinition->setMethod('init', $classMethod);
        $this->classDefinition->addConstant($constantsDefinition);
        $this->classDefinition->addProperty($propertyDefinition);

        // Generate test Class

        // protected function buildClass(ClassDefinition $class, string $indent, string $banner): string
        $actual = $buildClass->invokeArgs(
            $this->testClass,
            [
                $this->classDefinition,
                '    ',
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }

    /**
     * Regression coverage for issue #2426: a method parameter whose
     * `default.type` is `null` must emit a nullable type hint in the stub
     * (`?Type` in the signature, `Type|null` in the `@param` docblock).
     * Before the fix this produced an *implicit-nullable* parameter that
     * PHP 8.4 deprecates.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2426
     */
    public function testShouldBuildNullableParameters(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $methodParamsDefinition = [
            [
                'type'      => 'parameter',
                'name'      => 'obj',
                'const'     => 0,
                'data-type' => 'variable',
                'mandatory' => 0,
                'cast'      => [
                    'type'  => 'variable',
                    'value' => 'Stub\\Stubs\\Foo',
                ],
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
            [
                'type'      => 'parameter',
                'name'      => 'name',
                'const'     => 0,
                'data-type' => 'string',
                'mandatory' => 0,
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
            [
                'type'      => 'parameter',
                'name'      => 'count',
                'const'     => 0,
                'data-type' => 'int',
                'mandatory' => 0,
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
            [
                'type'      => 'parameter',
                'name'      => 'flag',
                'const'     => 0,
                'data-type' => 'bool',
                'mandatory' => 0,
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
            [
                'type'      => 'parameter',
                'name'      => 'ratio',
                'const'     => 0,
                'data-type' => 'double',
                'mandatory' => 0,
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
            [
                'type'      => 'parameter',
                'name'      => 'items',
                'const'     => 0,
                'data-type' => 'array',
                'mandatory' => 0,
                'default'   => [
                    'type'  => 'null',
                    'value' => null,
                ],
            ],
        ];

        $aliasManager = new AliasManager();
        $this->classDefinition->setAliasManager($aliasManager);

        $method = new Method(
            $this->classDefinition,
            ['public'],
            'nullable',
            new Parameters($methodParamsDefinition)
        );

        $buildMethod = $this->getMethod('buildMethod');
        $actual      = $buildMethod->invokeArgs($this->testClass, [$method, false, '    ']);

        // Signature: every typed param with default null must carry a leading `?`.
        $this->assertStringContainsString('?Stub\\Stubs\\Foo $obj = null', $actual);
        $this->assertStringContainsString('?string $name = null', $actual);
        $this->assertStringContainsString('?int $count = null', $actual);
        $this->assertStringContainsString('?bool $flag = null', $actual);
        $this->assertStringContainsString('?float $ratio = null', $actual);
        $this->assertStringContainsString('?array $items = null', $actual);

        // Docblock: the same nullability must surface as `Type|null` in @param.
        $this->assertStringContainsString('@param Stub\\Stubs\\Foo|null $obj', $actual);
        $this->assertStringContainsString('@param string|null $name', $actual);
        $this->assertStringContainsString('@param int|null $count', $actual);
        $this->assertStringContainsString('@param bool|null $flag', $actual);
        $this->assertStringContainsString('@param double|null $ratio', $actual);
        $this->assertStringContainsString('@param array|null $items', $actual);

        // Must NOT emit the legacy implicit-nullable form anywhere in the output.
        $this->assertStringNotContainsString(' Stub\\Stubs\\Foo $obj = null', $actual);
        $this->assertStringNotContainsString(' string $name = null', $actual);
        $this->assertStringNotContainsString(' int $count = null', $actual);
        $this->assertStringNotContainsString(' bool $flag = null', $actual);
        $this->assertStringNotContainsString(' float $ratio = null', $actual);
        $this->assertStringNotContainsString(' array $items = null', $actual);
    }

    /**
     * A variadic parameter (`string ...many`) must be emitted with the `...`
     * spread in the generated signature, and a source docblock `@param string
     * ...$many` must be preserved without an auto-generated duplicate `@param
     * string $many` being appended.
     */
    public function testShouldBuildVariadicParameter(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $methodParamsDefinition = [
            [
                'type'      => 'parameter',
                'name'      => 'delimiter',
                'const'     => 0,
                'data-type' => 'string',
                'mandatory' => 1,
            ],
            [
                'type'      => 'parameter',
                'name'      => 'many',
                'const'     => 0,
                'data-type' => 'string',
                'mandatory' => 0,
                'variadic'  => 1,
            ],
        ];

        $aliasManager = new AliasManager();
        $this->classDefinition->setAliasManager($aliasManager);

        $method = new Method(
            $this->classDefinition,
            ['public'],
            'concat',
            new Parameters($methodParamsDefinition),
            null,
            "/**\n * @param string \$delimiter\n * @param string ...\$many\n *\n * @return string\n */"
        );

        $buildMethod = $this->getMethod('buildMethod');
        $actual      = $buildMethod->invokeArgs($this->testClass, [$method, false, '    ']);

        // Signature: the variadic spread must be present.
        $this->assertStringContainsString('string $delimiter', $actual);
        $this->assertStringContainsString('string ...$many', $actual);

        // Docblock: the source variadic @param is preserved and NOT duplicated
        // by an auto-generated `@param string $many`.
        $this->assertStringContainsString('@param string ...$many', $actual);
        $this->assertStringNotContainsString('@param string $many', $actual);
    }

    /**
     * Regression coverage for issue #2428: when a method declares more than one
     * class in its return type union (e.g. `-> <Model> | <Row> | null`), every
     * class must appear in the generated signature. Previously only the first
     * class was emitted (`key()` of the class-type list), so `Row` was silently
     * dropped from `Model|Row|null`, leaving an invalid `Model|null` hint.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2428
     */
    public function testShouldBuildMethodWithMultipleClassReturnTypes(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $returnType = [
            'type' => 'return-type',
            'list' => [
                [
                    'type'       => 'return-type-parameter',
                    'cast'       => ['type' => 'variable', 'value' => 'Model'],
                    'collection' => 0,
                    'mandatory'  => 0,
                ],
                [
                    'type'       => 'return-type-parameter',
                    'cast'       => ['type' => 'variable', 'value' => 'Row'],
                    'collection' => 0,
                    'mandatory'  => 0,
                ],
                [
                    'type'      => 'return-type-parameter',
                    'data-type' => 'null',
                    'mandatory' => 0,
                ],
            ],
            'void' => 0,
        ];

        $this->classDefinition->setAliasManager(new AliasManager());

        $method = new Method(
            $this->classDefinition,
            ['public'],
            'find',
            new Parameters([]),
            null,
            null,
            $returnType
        );

        $buildMethod = $this->getMethod('buildMethod');
        $actual      = $buildMethod->invokeArgs($this->testClass, [$method, false, '    ']);

        // Both classes plus null must survive into the signature.
        $this->assertStringContainsString('public function find(): Model|Row|null', $actual);
        // And the docblock keeps listing the full union.
        $this->assertStringContainsString('@return Model|Row|null', $actual);
    }

    /**
     * Provide test case data for buildProperty method test.
     */
    public function propertyProvider(): array
    {
        return [
            // [ visibility ], type, value, expected
            [
                ['public'], 'int', 1, 'public $testProperty = 1;',
            ],
            [
                ['protected'], 'bool', 0, 'protected $testProperty = 0;',
            ],
            [
                ['static'], 'string', 'A', 'static private $testProperty = \'A\';',
            ],
            [
                ['static', 'error'], 'empty-array', null, 'static private $testProperty = [];',
            ],
            [
                [], 'null', null, 'private $testProperty = null;',
            ],
        ];
    }

    /**
     * @dataProvider propertyProvider
     *
     * @covers       \Zephir\Stubs\Generator::buildProperty
     *
     * @param array  $visibility
     * @param string $type
     * @param        $value
     * @param string $expected
     *
     * @throws \ReflectionException
     */
    public function testShouldBuildProperty(array $visibility, string $type, $value, string $expected): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $original = [
            'default' => [
                'type' => $type,
                'value' => $value,
            ],
        ];

        // Test requirements initialization

        $buildClass = $this->getMethod('buildProperty');
        $classProperty = new Property(
            $this->classDefinition,
            $visibility,
            'testProperty',
            null,
            '',
            $original
        );

        // protected function buildProperty(ClassProperty $property, string $indent): string
        $actual = $buildClass->invokeArgs(
            $this->testClass,
            [
                $classProperty,
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }

    public function constantProvider(): array
    {
        return [
            // constant type, value, expected
            [
                'null', null, 'const TEST = null;',
            ],
            [
                'string', 'Foo', 'const TEST = \'Foo\';',
            ],
            [
                'char', 'A', 'const TEST = \'A\';',
            ],
            [
                'empty-array', null, 'const TEST = [];',
            ],
            [
                'static-constant-access', ['left' => '\Pdo', 'right' => 'FETCH_LAZY'], 'const TEST = \\Pdo::FETCH_LAZY;',
            ],
            [
                'array',
                [
                    'left' => [
                        [
                            'key' => ['type' => 'string', 'value' => 'first'],
                            'value' => ['type' => 'int', 'value' => 1],
                        ],
                        [
                            'key' => ['type' => 'string', 'value' => 'second'],
                            'value' => ['type' => 'double', 'value' => 2],
                        ],
                        [
                            'key' => ['type' => 'int', 'value' => 3],
                            'value' => ['type' => 'bool', 'value' => 0],
                        ],
                    ],
                ],
                'const TEST = [\'first\' => 1, \'second\' => 2, 3 => 0];',
            ],
        ];
    }

    public function typedConstantProvider(): array
    {
        $typed   = PHP_VERSION_ID >= 80300;
        $docPart = "/**\n * @var %s\n */\n";

        return [
            [
                'int', 1, '@var int',
                sprintf($docPart, 'int') . ($typed ? 'const int TEST = 1;' : 'const TEST = 1;'),
            ],
            [
                'string', 'Foo', '@var string',
                sprintf($docPart, 'string') . ($typed ? "const string TEST = 'Foo';" : "const TEST = 'Foo';"),
            ],
            [
                'bool', 1, '@var bool',
                sprintf($docPart, 'bool') . ($typed ? 'const bool TEST = 1;' : 'const TEST = 1;'),
            ],
            [
                'string', 'bar', null, "const TEST = 'bar';",
            ],
        ];
    }

    /**
     * @dataProvider typedConstantProvider
     *
     * @throws \ReflectionException
     */
    public function testShouldBuildTypedConstant(string $type, mixed $value, ?string $docBlock, string $expected): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $buildClass = $this->getMethod('buildConstant');

        $classConstant = new Constant(
            'TEST',
            [
                'type'  => $type,
                'value' => $value,
            ],
            $docBlock
        );

        $actual = $buildClass->invokeArgs(
            $this->testClass,
            [
                $classConstant,
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }

    /**
     * @dataProvider constantProvider
     *
     * @param string $type
     * @param        $value
     * @param string $expected
     *
     * @throws \ReflectionException
     */
    public function testShouldBuildConstant(string $type, $value, string $expected): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $buildClass = $this->getMethod('buildConstant');

        $extended = [];
        if ('static-constant-access' === $type) {
            $extended = [
                'left' => [
                    'value' => $value['left'],
                ],
                'right' => [
                    'value' => $value['right'],
                ],
            ];
        }

        if ('array' === $type) {
            $extended = $value;
        }

        $classConstant = new Constant(
            'TEST',
            [
                'type' => $type,
                'value' => $value,
            ] + $extended,
            ''
        );

        // protected function buildConstant(ClassConstant $constant, string $indent): string
        $actual = $buildClass->invokeArgs(
            $this->testClass,
            [
                $classConstant,
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }

    public function testShouldBuildMethod(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $buildClass = $this->getMethod('buildMethod');

        $methodParamsDefinition = [
            [
                'type' => 'parameter',
                'name' => 'key',
                'const' => 0,
                'data-type' => 'string',
                'mandatory' => 0,
            ],
            [
                'type' => 'parameter',
                'name' => 'priority',
                'const' => 0,
                'data-type' => 'int',
                'mandatory' => 0,
                'default' => [
                    'type' => 'int',
                    'value' => 1,
                ],
            ],
        ];
        $methodParams = new Parameters($methodParamsDefinition);

        $returnType = [
            'type' => 'return-type',
            'list' => [
                [
                    'type' => 'return-type-parameter',
                    'data-type' => 'bool',
                    'mandatory' => 0,
                ],
            ],
            'void' => 0,
        ];

        $this->classDefinition->setAliasManager(new AliasManager());

        $classMethod = new Method(
            $this->classDefinition,
            ['public', 'static'],
            'testName',
            $methodParams,
            null,
            'Example description for testName method.',
            $returnType
        );

        $expected = <<<DOC
            /**
             * Example description for testName method.
             *
             * @param string \$key
             * @param int \$priority
             * @return bool
             */
            public static function testName(string \$key, int \$priority = 1): bool
            {
            }
            DOC;

        // protected function buildMethod(ClassMethod $method, bool $isInterface, string $indent): string
        $actual = $buildClass->invokeArgs(
            $this->testClass,
            [
                $classMethod,
                false,
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }
}
