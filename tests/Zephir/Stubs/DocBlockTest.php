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
use Zephir\Os;
use Zephir\Stubs\DocBlock;

final class DocBlockTest extends TestCase
{
    public function testInitialState(): void
    {
        $docBlock = new DocBlock('');

        $this->assertSame('', (string) $docBlock);
    }

    public function testDescription(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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

    public function testPhpDocs(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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

    public function testPhpDocWithDescription(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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

    public function testPhpDocWithCodeBloc(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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

    public function testPhpDocWithScalarParams(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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
                 * @param string[] \$stringArray
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
                 * @param string[] \$stringArray
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

    public function testPhpDocWithVariousDocBlockTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
                /**
                 * Method with various tags
                 * @author Phalcon Team <noreply@zephir-lang.com>
                 * @copyright (c) 2013-present Phalcon Team (https://zephir-lang.com/)
                 * @license MIT https://docs.zephir-lang.com/0.12/en/license
                 * @link https://github.com/zephir-lang/zephir
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
                 * @author Phalcon Team <noreply@zephir-lang.com>
                 * @copyright (c) 2013-present Phalcon Team (https://zephir-lang.com/)
                 * @license MIT https://docs.zephir-lang.com/0.12/en/license
                 * @link https://github.com/zephir-lang/zephir
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

    public function testPhpDocWithDoubleDollarSymbols(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

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
