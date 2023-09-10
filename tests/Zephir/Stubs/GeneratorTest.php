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
use ReflectionException;
use Zephir\AliasManager;
use Zephir\Class\ClassConstant;
use Zephir\Class\ClassDefinition;
use Zephir\Class\ClassMethod;
use Zephir\ClassMethodParameters;
use Zephir\ClassProperty;
use Zephir\Stubs\Generator;
use function Zephir\is_windows;

class GeneratorTest extends TestCase
{
    /**
     * @var mixed
     */
    private $generatorClass;
    private Generator $testClass;
    private ClassDefinition $classDefinition;

    protected function setUp(): void
    {
        $this->generatorClass = new \ReflectionClass(Generator::class);
        $this->testClass = new Generator([]);
        $this->classDefinition = new ClassDefinition('Stub\Stubs', 'StubsBuildClass');
    }

    /**
     * Modify method visibility to call protected.
     *
     * @param string $name - method name
     *
     * @return mixed
     *
     * @throws ReflectionException
     */
    private function getMethod(string $name)
    {
        $method = $this->generatorClass->getMethod($name);
        $method->setAccessible(true);

        return $method;
    }

    public function testShouldBuildClass(): void
    {
        if (is_windows()) {
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

        $extendsClassDefinition = new ClassDefinition('Stub\Extendable', 'BaseTestClass');
        $implementClassDefinition = new ClassDefinition('Stub\Events', 'EventsManagerInterface');
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

        $classMethod = new ClassMethod(
            $this->classDefinition,
            ['public', 'static'],
            'init',
            new ClassMethodParameters($methodParamsDefinition)
        );

        $constantsDefinition = new ClassConstant(
            'DEFAULT_PATH_DELIMITER',
            [
                'type' => 'string',
                'value' => '.',
            ],
            'Default path delimiter'
        );

        $propertyDefinition = new ClassProperty(
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
     * @covers       \Zephir\Stubs\Generator::buildProperty
     *
     * @param array  $visibility
     * @param string $type
     * @param $value
     * @param string $expected
     *
     * @throws ReflectionException
     */
    public function testShouldBuildProperty(array $visibility, string $type, $value, string $expected): void
    {
        if (is_windows()) {
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
        $classProperty = new ClassProperty(
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

    /**
     * @dataProvider constantProvider
     *
     * @param string $type
     * @param $value
     * @param string $expected
     *
     * @throws ReflectionException
     */
    public function testShouldBuildConstant(string $type, $value, string $expected): void
    {
        if (is_windows()) {
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

        $classConstant = new ClassConstant(
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
        if (is_windows()) {
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
        $methodParams = new ClassMethodParameters($methodParamsDefinition);

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

        $classMethod = new ClassMethod(
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
