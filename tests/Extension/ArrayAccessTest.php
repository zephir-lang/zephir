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

namespace Extension;

use PHPUnit\Framework\TestCase;

final class ArrayAccessTest extends TestCase
{
    private array $defaultUnsetData = [
        'key_a' => 'marcin',
        'key_b' => 'paula',
        3 => 'long value',
        //3.14 => 'double value', // Not supported yet
        //false => 'bool value', // Not supported yet
        //null => 'null value', // Not supported yet
    ];

    public function testTest(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertTrue($class->exits('one'));
        $this->assertSame(2, $class->get());
    }

    public function testUnsetByKeyFromArray(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame([], $class->unsetByKeyFromArray('not-exist', []));
    }

    public function testUnsetByKeyFromProperty(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame([], $class->unsetByKeyFromProperty('ok', ['ok' => true]));
        $this->assertSame(['another' => 'value'], $class->unsetByKeyFromProperty('ok', ['ok' => true, 'another' => 'value']));
        $this->assertSame([], $class->unsetByKeyFromProperty('not-exist', []));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/645
     */
    public function testIssue645(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame([], $class->issue645());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1155
     */
    public function testIssue1155(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertFalse($class->issue1155());
    }

    public function testIssue1094(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertFalse($class->issue1094Test1());
        $this->assertFalse($class->issue1094Test1([]));
        $this->assertFalse($class->issue1094Test1(['test' => 'ok']));

        $this->assertTrue($class->issue1094Test2());
        $this->assertFalse($class->issue1094Test2([]));
        $this->assertFalse($class->issue1094Test2(['test' => 'ok']));

        $this->assertTrue($class->issue1094Test3());
        $this->assertFalse($class->issue1094Test3([]));
        $this->assertFalse($class->issue1094Test3(['test' => 'ok']));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public function testIssue1086StaticallyCalledFunctionWithArrayAsArgMustReturnArray(): void
    {
        if (version_compare(PHP_VERSION, '8.2.0', '>=')) {
            $this->markTestSkipped('Deprecated Callable Patterns');
        }

        $class = new \Stub\ArrayAccessTest();

        $actual = $class->issue1086WontNullArrayAfterPassViaStaticWithStrictParams();
        $this->assertSame(['test' => 123], $actual);

        $actual = $class->issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams();
        $this->assertSame(['test' => 123], $actual);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1259
     */
    public function testIssue1259CheckUnsetKeyFromArray(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $expected = [
            ['key_a' => 'marcin', 'key_b' => 'paula'],
            ['key_b' => 'paula'],
        ];

        $this->assertSame($expected, $class->issue1259UnsetKeyFromArrayInternalVariable());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1259
     */
    public function testIssue1259CheckUnsetStringKeyFromArrayProperty(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame(
            [
                $this->defaultUnsetData,
                [
                    'key_b' => 'paula',
                    3 => 'long value',
                ],
            ],
            $class->issue1259UnsetStringKeyFromArrayProperty()
        );
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1259
     */
    public function testIssue1259CheckUnsetLongKeyFromArrayProperty(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame(
            [
                $this->defaultUnsetData,
                [
                    'key_a' => 'marcin',
                    'key_b' => 'paula',
                ],
            ],
            $class->issue1259UnsetLongKeyFromArrayProperty()
        );
    }
}
