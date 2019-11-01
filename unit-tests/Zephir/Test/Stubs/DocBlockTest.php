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
use Zephir\Stubs\DocBlock;

class DocBlockTest extends TestCase
{
    public function testInitialState()
    {
        $docBlock = new DocBlock('');

        $this->assertSame('', (string) $docBlock);
    }

    public function testDescription()
    {
        $doc = <<<DOC
/**
 * Hello world
 * I am test.
 */
DOC;
        $expected = <<<DOC
    /**
     * Hello world
     * I am test.
     */
DOC;

        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocs()
    {
        $doc = <<<DOC
/**
 * @param string \$foo
 * @return string
 */
DOC;
        $expected = <<<DOC
    /**
     * @param string \$foo
     * @return string
     */
DOC;
        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocWithDescription()
    {
        $doc = <<<DOC
/**
 * Hello world
 * @return string
 */
DOC;
        $expected = <<<DOC
    /**
     * Hello world
     *
     * @return string
     */
DOC;

        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocWithCodeBloc()
    {
        $doc = <<<DOC
/**
 * Some method.
 *
 * <code>
 * if (\$foo !== \$bar) {
 *     // Some comment
 *
 *     // Another comment
 *
 *     while (true) {
 *         // Some staff
 *     }
 * }
 * </code>
 *
 * @return string
 */
DOC;

        $expected = <<<DOC
    /**
     * Some method.
     *
     * <code>
     * if (\$foo !== \$bar) {
     *     // Some comment
     *
     *     // Another comment
     *
     *     while (true) {
     *         // Some staff
     *     }
     * }
     * </code>
     *
     * @return string
     */
DOC;

        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocWithScalarParams()
    {
        $doc = <<<DOC
    /**
     * Method with various input params
     *
     * @param int \$valueInteger
     * @param double \$valueDouble
     * @param float \$valueFloat
     * @param string \$valueString
     * @param bool \$valueBoolean
     * @param array \$valueArray
     * @param object \$valueObject
     * @param resource \$valueResource
     * @param null \$valueNull
     *
     * @param int|double \$valueIntOrDouble
     * @param int|string \$intOrString
     * @param array|object \$arrayOrObject
     *
     * @throws \Zephir\Compiler\CompilerException
     * @return int|null - Return Some Result
     */
DOC;

        $expected = <<<DOC
    /**
     * Method with various input params
     *
     * @param int \$valueInteger
     * @param double \$valueDouble
     * @param float \$valueFloat
     * @param string \$valueString
     * @param bool \$valueBoolean
     * @param array \$valueArray
     * @param object \$valueObject
     * @param resource \$valueResource
     * @param null \$valueNull
     *
     * @param int|double \$valueIntOrDouble
     * @param int|string \$intOrString
     * @param array|object \$arrayOrObject
     *
     * @throws \Zephir\Compiler\CompilerException
     * @return int|null - Return Some Result
     */
DOC;
        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocWithVariousDocBlockTags()
    {
        $doc = <<<DOC
    /**
     * Method with various tags
     * @author Zephir Team <noreply@zephir-lang.com>
     * @copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)
     * @license MIT http://zephir-lang.com/license.html
     * @link https://github.com/phalcon/zephir
     * @since 1.0.0
     * @todo Something
     * @uses MyClass::function doSmthName
     * @version 1.0.0
     * @api Some Api Description
     * @wrongTag - \$somethingStrange
     */
DOC;

        $expected = <<<DOC
    /**
     * Method with various tags
     *
     * @author Zephir Team <noreply@zephir-lang.com>
     * @copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)
     * @license MIT http://zephir-lang.com/license.html
     * @link https://github.com/phalcon/zephir
     * @since 1.0.0
     * @todo Something
     * @uses MyClass::function doSmthName
     * @version 1.0.0
     * @api Some Api Description
     * @wrongTag - \$somethingStrange
     */
DOC;
        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }

    public function testPhpDocWithDoubleDollarSymbols()
    {
        $doc = <<<DOC
    /**
     * @param integer \$\$dollars - var description
     */
DOC;

        $expected = <<<DOC
    /**
     * @param integer \$dollars - var description
     */
DOC;
        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string) $docBlock);
    }
}
