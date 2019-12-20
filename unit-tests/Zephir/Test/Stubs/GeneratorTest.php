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
use Zephir\ClassDefinition;
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
        $buildClass = $this->getMethod('buildClass');
        $actual = <<<DOC
<?php

namespace TestNamespace\Stubs;

use TestNamespace\Events\ManagerInterface;
use TestNamespace\Events\EventInterface as EventsManagerInterface;

/**
 * Class description example
 */
final class shouldBuildClassTest
{

}

DOC;

        $generator = new Generator([]);
        $classDefinition = new ClassDefinition('TestNamespace\Stubs', 'shouldBuildClassTest');
        $aliasManager = new AliasManager();

        $aliasManager->add([
            'aliases' => [
                [
                    'name' => 'TestNamespace\\Events\\ManagerInterface',
                ],
                [
                    'name' => 'TestNamespace\\Events\\EventInterface',
                    'alias' => 'EventsManagerInterface',
                ],
            ],
        ]);

        $classDefinition->setAliasManager($aliasManager);
        $classDefinition->setDocBlock('Class description example');
        $classDefinition->setIsFinal(true);

        $expected = $buildClass->invokeArgs(
            $generator,
            [
                $classDefinition,
                '  ',
                '',
            ]
        );

        $this->assertSame($expected, $actual);
    }
}
