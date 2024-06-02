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

namespace Extension\Oo;

use PHPUnit\Framework\TestCase;
use Stub\Oo\OoParams;

final class OoParamsTest extends TestCase
{
    private OoParams $test;

    protected function setUp(): void
    {
        $this->test = new OoParams();
    }

    public function testOoParams(): void
    {
        $this->assertInstanceOf('Stub\Oo\OoParams', $this->test->createThisClassWithoutWriteCurrentNamespace());
        $this->assertinstanceOf('Stub\Oo\OoDynamicA', $this->test->createOtherClassWithoutWriteCurrentNamespace());

        $this->assertSame($this->test->setAge(17), 17);

        $this->assertSame($this->test->setAverage(17.1), 17.1);
        $this->assertSame($this->test->setAverage(17), 17.0);
        $this->assertSame($this->test->setAverage('17.5'), 17.5);

        $this->assertSame($this->test->setName('peter'), 'peter');

        $this->assertSame($this->test->setEnabled(true), true);
        $this->assertSame($this->test->setEnabled(false), false);

        $this->assertSame($this->test->setList([1, 2, 3, 4, 5]), [1, 2, 3, 4, 5]);
        $this->assertSame($this->test->setList([]), []);

        $this->assertSame(0, $this->test->getDefaultParamValue());
        $this->assertSame(1, $this->test->getDefaultParamValue1());
        $this->assertSame('default', $this->test->getDefaultParamValueStr());
    }

    public function setObjectClassCast(): void
    {
        $result = $this->test->setStrictName(new \Stub\Oo\Param());

        $this->assertInstanceOf('Stub\Oo\Param', $result);
    }

    public function testTypError(): void
    {
        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setAge('17'), 17);

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setAge(17.0), 17);

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setName(12.5), '12.5');

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setName(17), '17');

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setEnabled(1), true);

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setEnabled(0), false);

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setEnabled('1'), true);

        $this->expectException(\TypeError::class);
        $this->assertSame($this->test->setEnabled('0'), false);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/938
     */
    public function testWrongNumberParamsIssue938(): void
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\ArgumentCountError::class);
        } else {
            $this->expectException(\BadMethodCallException::class);
        }

        $this->test->setAge(17, 'invalid');
    }
}
