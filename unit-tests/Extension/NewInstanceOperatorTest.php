<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class NewInstanceOperatorTest extends TestCase
{
    protected $autoloadMap = [
        'Fixture\ThrowException' => ZEPHIRPATH.'/unit-tests/fixtures/throw-exception.php',
        'Fixture\EmptyClass' => ZEPHIRPATH.'/unit-tests/fixtures/class-empty.php',
    ];

    public function setUp()
    {
        spl_autoload_register([$this, 'autoload']);
    }

    public function tearDown()
    {
        spl_autoload_unregister([$this, 'autoload']);
    }

    public function autoload($className)
    {
        if (isset($this->autoloadMap[$className])) {
            include $this->autoloadMap[$className];
        }
    }

    public function testException()
    {
        $this->expectException(\Exception::class);
        $t = new \Test\Operator();
        $obj = $t->testNewInstanceOperator('Fixture\ThrowException');
    }

    public function testNewInstance()
    {
        $t = new \Test\Operator();
        $object = $t->testNewInstanceOperator('Fixture\EmptyClass');

        $this->assertInstanceOf('Fixture\EmptyClass', $object);
    }
}
