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

    public function testIsPreservedTagLineDelegatesToRegistry(): void
    {
        // DocBlock::isPreservedTagLine is protected; expose via anonymous subclass.
        $probe = new class ('') extends \Zephir\Stubs\DocBlock {
            public function check(string $line): bool
            {
                return $this->isPreservedTagLine($line);
            }
        };

        $this->assertTrue($probe->check('@phpstan-return Foo'));
        $this->assertTrue($probe->check('@template T'));
        $this->assertFalse($probe->check('@param string $x'));
        $this->assertFalse($probe->check('plain text'));
    }

    public function testPreservesExtendsImplementsTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * @extends \Phalcon\Mvc\Model<T>
             * @implements \Foo\BarInterface<int, string>
             */
            DOC;

        $expected = <<<DOC
                /**
                 * @extends \Phalcon\Mvc\Model<T>
                 * @implements \Foo\BarInterface<int, string>
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }

    public function testPreservesMixedTagsWithDescription(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * Find records matching parameters.
             *
             * @phpstan-return \Phalcon\Mvc\Model\Resultset\Simple<array-key, static>
             * @return \Phalcon\Mvc\Model\Resultset\Simple
             * @throws \Phalcon\Mvc\Model\Exception
             */
            DOC;

        $expected = <<<DOC
                /**
                 * Find records matching parameters.
                 *
                 * @phpstan-return \Phalcon\Mvc\Model\Resultset\Simple<array-key, static>
                 * @return \Phalcon\Mvc\Model\Resultset\Simple
                 * @throws \Phalcon\Mvc\Model\Exception
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }

    public function testPreservesMultilinePhpStanType(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * @phpstan-type FindParams array{
             *     conditions?: string,
             *     bind?: array<string, mixed>,
             *     limit?: int,
             * }
             */
            DOC;

        $expected = <<<DOC
                /**
                 * @phpstan-type FindParams array{
                 *     conditions?: string,
                 *     bind?: array<string, mixed>,
                 *     limit?: int,
                 * }
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }

    public function testPreservesPhpStanTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * @phpstan-return static|null
             * @phpstan-param array<string, mixed> \$parameters
             * @phpstan-type FindParams array{conditions?: string, limit?: int}
             * @phpstan-template T of object
             */
            DOC;

        $expected = <<<DOC
                /**
                 * @phpstan-return static|null
                 * @phpstan-param array<string, mixed> \$parameters
                 * @phpstan-type FindParams array{conditions?: string, limit?: int}
                 * @phpstan-template T of object
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }

    public function testPreservesPsalmTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * @psalm-return Foo<int>
             * @psalm-param positive-int \$count
             * @psalm-var non-empty-string
             * @psalm-template T of \Bar
             */
            DOC;

        $expected = <<<DOC
                /**
                 * @psalm-return Foo<int>
                 * @psalm-param positive-int \$count
                 * @psalm-var non-empty-string
                 * @psalm-template T of \Bar
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }

    public function testPreservesTemplateTags(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $doc = <<<DOC
            /**
             * @template T of \Phalcon\Mvc\ModelInterface
             * @template-covariant TItem
             * @template-contravariant TIn of object
             */
            DOC;

        $expected = <<<DOC
                /**
                 * @template T of \Phalcon\Mvc\ModelInterface
                 * @template-covariant TItem
                 * @template-contravariant TIn of object
                 */
            DOC;

        $this->assertSame($expected, (string) new DocBlock($doc));
    }
}
