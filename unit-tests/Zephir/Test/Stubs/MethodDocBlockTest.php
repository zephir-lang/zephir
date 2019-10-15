<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Stubs;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\ClassMethodParameters;
use Zephir\StatementsBlock;
use Zephir\Stubs\MethodDocBlock;

class MethodDocBlockTest extends TestCase
{
    public function prepareMethod($params)
    {
        /**
         * ClassDefinition for Class creation.
         *
         * @param string      $namespace Namespace ex: \Zephir\Stubs
         * @param string      $name      Class name, ex: Stubs
         * @param string|null $shortName {get, set}
         */
        $classNamespace = $params['class']['namespace'];
        $className = $params['class']['className'];
        $classShortName = $params['class']['classShortName'];

        $classDefinition = new ClassDefinition(
            $classNamespace,
            $className,
            $classShortName
        );

        /**
         * ClassMethod constructor.
         *
         * @param ClassDefinition            $classDefinition
         * @param array                      $visibility
         * @param string                     $name
         * @param ClassMethodParameters|null $parameters
         * @param StatementsBlock|null       $statements
         * @param string|null                $docblock
         * @param array|null                 $returnType
         * @param array|null                 $original
         */
        $methodVisibility = $params['method']['visibility'];
        $methodName = $params['method']['name'];
        $methodParams = $params['method']['parameters'];
        $methodStBlock = $params['method']['statements'];
        $methodDocBlock = $params['method']['docblock'];
        $methodReturnType = $params['method']['returnType'];
        $methodExpression = $params['method']['expression'];

        $classMethod = new ClassMethod(
            $classDefinition,
            $methodVisibility,
            $methodName,
            $methodParams ? new ClassMethodParameters($methodParams) : null,
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
        $classDefinition = require_once ZEPHIRPATH.'/unit-tests/fixtures/class-definition-2.php';
        $testDocBlock = $this->prepareMethod($classDefinition);
        $expected = <<<DOC
    /**
     * Zephir
     */
DOC;

        $this->assertSame($expected, $testDocBlock->processMethodDocBlock());
    }

    public function testMethodWithInputArgs()
    {
        $classDefinition = require_once ZEPHIRPATH.'/unit-tests/fixtures/class-definition-1.php';

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

        $this->assertSame($expected, $testDocBlock->processMethodDocBlock());
    }

    public function getDocBlock()
    {
        return [
            [
                ['data-type' => 'int', 'name' => 'value'],
                "    /**\n     * @param int \$value\n     */",
            ],
            [
                ['data-type' => 'double', 'name' => 'value'],
                "    /**\n     * @param double \$value\n     */",
            ],
            [
                ['data-type' => 'float', 'name' => 'value'],
                "    /**\n     * @param float \$value\n     */",
            ],
            [
                ['data-type' => 'string', 'name' => 'value'],
                "    /**\n     * @param string \$value\n     */",
            ],
            [
                ['data-type' => 'array', 'name' => 'value'],
                "    /**\n     * @param array \$value\n     */",
            ],
            [
                ['data-type' => 'object', 'name' => 'value'],
                "    /**\n     * @param object \$value\n     */",
            ],
            [
                ['data-type' => 'int|string', 'name' => 'value'],
                "    /**\n     * @param int|string \$value\n     */",
            ],
            [
                ['data-type' => 'int|null', 'name' => 'value'],
                "    /**\n     * @param int|null \$value\n     */",
            ],
            [
                ['data-type' => 'array|object', 'name' => '$value'],
                "    /**\n     * @param array|object \$value\n     */",
            ],
            [
                ['data-type' => '\FrontendInterface', 'name' => '$value'],
                "    /**\n     * @param \FrontendInterface \$value\n     */",
            ],
        ];
    }

    /**
     * @dataProvider getDocBlock()
     *
     * @param array  $parameters
     * @param string $expected
     */
    public function testMethodsWithDataSet(array $parameters, $expected)
    {
        $baseDefinition = require ZEPHIRPATH.'/unit-tests/fixtures/base-definition.php';
        $baseDefinition['method']['parameters'][] = $parameters;

        $this->assertSame($expected, $this->prepareMethod($baseDefinition)->processMethodDocBlock());
    }

    public function testMethodWithFullDocBlock()
    {
        $docblock = "Test for full filled Method\n".
                    "with non-ordered params\n".
                    "@param int \$val1\n".
                    "@param array \$val3 \n".
                    "@param string \$val2 - with additional descrription\n".
                    "@throws \Zephir\Compiler\CompilerException \n";

        $testDefinition = require_once ZEPHIRPATH.'/unit-tests/fixtures/class-definition-3.php';
        $testDefinition['method']['docblock'] = $docblock;

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

        $this->assertSame($expected, $this->prepareMethod($testDefinition)->processMethodDocBlock());
    }
}
