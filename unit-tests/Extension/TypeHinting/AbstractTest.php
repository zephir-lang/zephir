<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\TypeHinting;

use PHPUnit\Framework\SkippedTestError;
use TestConcreteClass;
use Zephir\Support\TestCase;

class AbstractTest extends TestCase
{
    /**
     * {@inheritdoc}
     *
     * @return void
     */
    public static function setUpBeforeClass()
    {
        if (PHP_VERSION_ID < 70000) {
            throw new SkippedTestError("Scalar type hint are avalibale in PHP 7 only");
        }
    }

    /**
     * @test
     * @link https://github.com/phalcon/zephir/issues/1667
     */
    public function shouldCreateCompatibleChildClassUsingTypeHintedParams()
    {
        $childClass = new TestConcreteClass();

        $this->assertEquals(
            [[1667], '/', true, null],
            $childClass->testFunc([1667], '/', true, null)
        );

        $this->assertEquals(
            [['issue'], '\\', true, 1667],
            $childClass->testFunc(['issue'], '\\', true, 1667)
        );
    }
}
