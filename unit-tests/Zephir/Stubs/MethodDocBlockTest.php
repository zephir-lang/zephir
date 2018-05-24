<?php

namespace Zephir\Stubs;

use Zephir\ClassMethod;
use Zephir\ClassDefinition;
use Zephir\StatementsBlock;
use Zephir\AliasManager;

class MethodDocBlockTest extends \PHPUnit_Framework_TestCase
{
    public function prepareMethod($params)
    {
        /**
         * ClassDefinition for Class creation
         * @var string $classNamespace - namespace ex: \Zephir\Stubs
         * @var string $className - Class name, ex: Stubs
         * @var string $classShortName - {get, set}
         */

        $classNamespace = $params['class']['namespace'];
        $className      = $params['class']['className'];
        $classShortName = $params['class']['classShortName'];

        $classDefinition = new ClassDefinition(
            $classNamespace,
            $className,
            $classShortName
        );

        /**
         * ClassMethod constructor
         *
         * @param ClassDefinition $classDefinition
         * @param array $visibility
         * @param $name
         * @param $parameters
         * @param StatementsBlock $statements
         * @param null $docblock
         * @param null $returnType
         * @param array $original
         */
        $methodVisibility = $params['method']['visibility'];
        $methodName       = $params['method']['name'];
        $methodParams     = $params['method']['parameters'];
        $methodStBlock    = $params['method']['statements'];
        $methodDocBlock   = $params['method']['docblock'];
        $methodReturnType = $params['method']['returnType'];
        $methodExpression = $params['method']['expression'];

        $classMethod = new ClassMethod(
            $classDefinition,
            $methodVisibility,
            $methodName,
            $methodParams,
            new StatementsBlock($methodStBlock),
            $methodDocBlock,
            $methodReturnType,
            $methodExpression
        );

        $aliasManager = new AliasManager();

        return new MethodDocBlock($classMethod, $aliasManager);
    }

    public function testMethodWithoutInputArgs()
    {
        $classDefinition = [
            'class' => [
                'namespace' => 'Zephir\Stubs',
                'className' => 'Stubs',
                'classShortName' => 'get'
            ],
            'method' => [
                'visibility' => ['public', 'final'],
                'name'       => 'fooTest',
                'parameters' => null,
                'statements' => [],
                'docblock'   => 'Zephir',
                'returnType' => null,
                'expression' => null
            ]
        ];
        $testDocBlock = $this->prepareMethod($classDefinition);
        $expected = <<<DOC
    /**
     * Zephir
     */
DOC;

        $this->assertSame($expected, (string)$testDocBlock);
    }

    public function testMethodWithInputArgs()
    {
        $classDefinition = [
            'class' => [
                'namespace' => 'Zephir\Stubs',
                'className' => 'Stubs',
                'classShortName' => 'get'
            ],
            'method' => [
                'visibility' => ['public', 'final'],
                'name'       => 'scalarInputAgrs',
                'parameters' => [
                    ['data-type' => 'int', 'name' => 'val1'],
                    ['data-type' => 'bool', 'name' => 'val2'],
                    ['data-type' => 'string', 'name' => 'val3'],
                    ['data-type' => 'double', 'name' => 'val4'],
                ],
                'statements' => [],
                'docblock'   => "Test function scalarInputArgs(int val1, bool val2, string val3, double val4)\n with return Integer\n @return int",
                'returnType' => null,
                'expression' => null
            ]
        ];
        $testDocBlock = $this->prepareMethod($classDefinition);
        $expected = <<<DOC
    /**
     * Test function scalarInputArgs(int val1, bool val2, string val3, double val4)
     * with return Integer
     *
     * @param int \$val1
     * @param bool \$val2
     * @param string \$val3
     * @param double \$val4
     * @return int
     */
DOC;

        $this->assertSame($expected, (string)$testDocBlock);
    }

    public function getDocBlock()
    {
        return [
            [
                ['data-type' => 'int', 'name' => 'value'],
                "    /**\n     * @param int \$value\n     */"
            ],
            [
                ['data-type' => 'double', 'name' => 'value'],
                "    /**\n     * @param double \$value\n     */"
            ],
            [
                ['data-type' => 'float', 'name' => 'value'],
                "    /**\n     * @param float \$value\n     */"
            ],
            [
                ['data-type' => 'string', 'name' => 'value'],
                "    /**\n     * @param string \$value\n     */"
            ],
            [
                ['data-type' => 'array', 'name' => 'value'],
                "    /**\n     * @param array \$value\n     */"
            ],
            [
                ['data-type' => 'object', 'name' => 'value'],
                "    /**\n     * @param object \$value\n     */"
            ],
            [
                ['data-type' => 'int|string', 'name' => 'value'],
                "    /**\n     * @param int|string \$value\n     */"
            ],
            [
                ['data-type' => 'int|null', 'name' => 'value'],
                "    /**\n     * @param int|null \$value\n     */"
            ],
            [
                ['data-type' => 'array|object', 'name' => '$value'],
                "    /**\n     * @param array|object \$value\n     */"
            ],
            [
                ['data-type' => '\FrontendInterface', 'name' => '$value'],
                "    /**\n     * @param \FrontendInterface \$value\n     */"
            ],
        ];
    }

    /**
     * @dataProvider getDocBlock()
     */
    public function testMethodsWithDataSet($classDefinition, $expected)
    {
        $baseDefinition = [
            'class' => [
                'namespace' => 'Zephir\Stubs',
                'className' => 'Stubs',
                'classShortName' => 'get'
            ],
            'method' => [
                'visibility' => ['public'],
                'name'       => 'testMethodDatSet',
                'parameters' => [],
                'statements' => [],
                'docblock'   => null,
                'returnType' => null,
                'expression' => null
            ]
        ];
        $baseDefinition['method']['parameters'][] = $classDefinition;

        $this->assertSame($expected, (string)$this->prepareMethod($baseDefinition));
    }

    public function testMethodWithFullDocBlock()
    {
        $docblock = "Test for full filled Method\n" .
                    "with non-ordered params\n" .
                    "@param int \$val1\n" .
                    "@param array \$val3 \n" .
                    "@param string \$val2 - with additional descrription\n" .
                    "@throws \Zephir\Compiler\CompilerException \n";

        $testDefinition = [
            'class' => [
                'namespace' => 'Zephir\Stubs',
                'className' => 'Stubs',
                'classShortName' => 'get'
            ],
            'method' => [
                'visibility' => ['public'],
                'name'       => 'testMethodWithFullDocBlock',
                'parameters' => [
                    ['data-type' => 'int', 'name' => 'val1'],
                    ['data-type' => 'string', 'name' => 'val2'],
                    ['data-type' => 'array', 'name' => '$val3'],
                ],
                'statements' => [],
                'docblock'   => $docblock,
                'returnType' => null,
                'expression' => null
            ]
        ];

        $expected = <<<DOC
    /**
     * Test for full filled Method
     * with non-ordered params
     *
     * @throws \Zephir\Compiler\CompilerException
     * @param int \$val1
     * @param string \$val2 - with additional descrription
     * @param array \$val3
     */
DOC;

        $this->assertSame($expected, (string)$this->prepareMethod($testDefinition));
    }
}
