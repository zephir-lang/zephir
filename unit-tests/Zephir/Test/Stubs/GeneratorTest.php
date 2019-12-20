<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;
use Zephir\ClassConstant;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\Stubs\Generator;

class GeneratorTest extends TestCase
{
    private $generatorClass;

    public function setUp()
    {
        $this->generatorClass = new \ReflectionClass(Generator::class);
    }

    /**
     * Modify method visibility to call protected.
     *
     * @param string $name - method name
     */
    private function getMethod(string $name)
    {
        $method = $this->generatorClass->getMethod($name);
        $method->setAccessible(true);

        return $method;
    }

    /** @test */
    public function shouldBuildClass()
    {
        $expected = <<<DOC
<?php

namespace Test\Stubs;

use Test\Extendable\BaseTestClass;
use Test\Events\EventInterface as EventsManagerInterface;

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

        $generator = new Generator([]);

        $classDefinition = new ClassDefinition('Test\Stubs', 'StubsBuildClass');
        $extendsClassDefinition = new ClassDefinition('Test\Extendable', 'BaseTestClass');
        $implementClassDefinition = new ClassDefinition('Test\Events', 'EventsManagerInterface');
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
            $classDefinition,
            ['public', 'static'],
            'init',
            new \Zephir\ClassMethodParameters($methodParamsDefinition)
        );

        $constantsDefinition = new ClassConstant(
            'DEFAULT_PATH_DELIMITER',
            [
                'type' => 'string',
                'value' => '.',
            ],
            'Default path delimiter'
        );

        // Inject definitions and construct test Class

        $aliasManager->add([
            'aliases' => [
                [
                    'name' => 'Test\\Extendable\\BaseTestClass',
                ],
                [
                    'name' => 'Test\\Events\\EventInterface',
                    'alias' => 'EventsManagerInterface',
                ],
            ],
        ]);

        $implementClassDefinition->setAliasManager($aliasManager);
        $classDefinition->setAliasManager($aliasManager);
        $classDefinition->setDocBlock('Class description example');
        $classDefinition->setIsFinal(true);
        $classDefinition->setExtendsClassDefinition($extendsClassDefinition);
        $classDefinition->setExtendsClass('BaseTestClass');
        $classDefinition->setImplementedInterfaceDefinitions([
            $implementClassDefinition,
        ]);
        $classDefinition->setImplementsInterfaces([
            [
                'value' => '\Iterator',
            ],
            [
                'value' => 'Test\\Events\\EventInterface',
            ],
        ]);
        $classDefinition->setMethod('init', $classMethod);
        $classDefinition->addConstant($constantsDefinition);

        // Generate test Class

        $actual = $buildClass->invokeArgs(
            $generator,
            [
                $classDefinition,
                '    ',
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }
}
