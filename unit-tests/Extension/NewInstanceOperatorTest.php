<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class NewInstanceOperatorTest extends TestCase
{

    protected $autoloadMap = [
        'Fixture\ParseErrorClass' => ZEPHIRPATH.'/unit-tests/fixtures/class-parse-error.php',
        'Fixture\EmptyClass' => ZEPHIRPATH.'/unit-tests/fixtures/class-empty.php'
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
        if(isset($this->autoloadMap[$className])) {
            include $this->autoloadMap[$className];
        }
    }

    public function testThrowableException()
    {
        $this->expectException(\ParseError::class);

        $t = new \Test\Operator();
        $obj = $t->testNewInstanceOperator('Fixture\ParseErrorClass');
    }

    public function testNewInstance()
    {
        $t = new \Test\Operator();
        $object = $t->testNewInstanceOperator('Fixture\EmptyClass');

        $this->assertInstanceOf('Fixture\EmptyClass', $object);
    }

}
