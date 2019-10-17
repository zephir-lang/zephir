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
     * @test
     * @dataProvider getDocBlock()
     *
     * @param array  $parameters
     * @param string $expected
     */
    public function methodsWithDataSet(array $parameters, $expected)
    {
        $baseDefinition = require ZEPHIRPATH.'/unit-tests/fixtures/base-definition.php';
        $baseDefinition['method']['parameters'][] = $parameters;

        $this->assertSame($expected, $this->prepareMethod($baseDefinition)->processMethodDocBlock());
    }

    public function docBlockProvider(): array
    {
        return [
            'with @var' => [
                '@var mixed',
                '@param mixed',
            ],
            'with @return' => [
                '@return int',
                '@return int',
            ],
            'with all types' => [
                "@var string\n * @param array data\n * @throws \Exception\n * @return boolean",
                "@param string\n * @param array \$data\n * @throws \Exception\n * @return boolean",
            ],
            'one Integer param' => [
                '@param int magicNumber',
                '@param int $magicNumber',
            ],
            'with Mixed params' => [
                '@param int|double magicNumber',
                '@param int|double $magicNumber',
            ],
            'with Mixed params and spaces' => [
                '@param int | double magicNumber',
                '@param int | double $magicNumber',
            ],
            'with Description in DocBlock' => [
                "Test description\n * @param int | double magicNumber",
                "Test description\n *\n * @param int | double \$magicNumber",
            ],
            'with short variable name' => [
                '@param string a',
                '@param string $a',
            ],
            'without any params' => [
                'Zephir',
                'Zephir',
            ],
            'with @var in @return' => [
                '@return var|array - some description of var',
                '@return mixed|array - some description of var',
            ],
            'with many params and Exception' => [
                // Zep
                "Test for full filled Method\n".
                "* with non-ordered params\n".
                "*\n".
                "* @param int \$val1\n".
                "* @param callback callback\n".
                "* @param array \$val3\n".
                "* @param string \$val2 - with additional descrription\n".
                "* @throws \Zephir\Compiler\CompilerException",
                // Php
                "Test for full filled Method\n".
                " * with non-ordered params\n".
                " *\n".
                " * @param int \$val1\n".
                " * @param callback \$callback\n".
                " * @param array \$val3\n".
                " * @param string \$val2 - with additional descrription\n".
                " * @throws \Zephir\Compiler\CompilerException",
            ],
            'with function scalar input args' => [
                // Zep
                "Test function scalarInputArgs(int val1, bool val2, string val3, double val4)\n".
                "* with return Integer\n".
                "*\n".
                "* @return int\n".
                "* @param int \$val1\n".
                "* @param bool \$val2\n".
                "* @param string \$val3\n".
                '* @param double $val4',
                // Php
                "Test function scalarInputArgs(int val1, bool val2, string val3, double val4)\n".
                " * with return Integer\n".
                " *\n".
                " * @return int\n".
                " * @param int \$val1\n".
                " * @param bool \$val2\n".
                " * @param string \$val3\n".
                ' * @param double $val4',
            ],
            'with multiline array' => [
                // Zep
                "@param array | object config_zzz = [\n".
                "*    \"test\" => \"xyz\"\n".
                '* ]',
                // Php
                "@param array | object \$config_zzz = [\n".
                " *    \"test\" => \"xyz\"\n".
                ' * ]',
            ],
        ];
    }

    /**
     * @test
     * @dataProvider docBlockProvider()
     */
    public function shouldParseDocBlock(string $zephirDocBlock, string $phpDocBlock)
    {
        $classMethod = new ClassMethod(
            new ClassDefinition('Zephir', 'testMethod'),
            ['public'],
            'exampleMethodName',
            null,
            null,
            "/**\n * {$zephirDocBlock}\n */"
        );

        $docblock = new MethodDocBlock($classMethod, new AliasManager(), '');
        $expected = "/**\n * {$phpDocBlock}\n */";

        $this->assertSame($expected, $docblock->processMethodDocBlock());
    }
}
