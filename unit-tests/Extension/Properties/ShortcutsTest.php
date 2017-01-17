<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension\Properties;

use Test\Properties\Shortcuts;

class ShortcutsesTest extends \PHPUnit_Framework_TestCase
{
    public function testPerson()
    {
        $t = new Shortcuts();
        $t->setName('Julia');
        $t->setAge(21);
        $t->setMale(false);
        $t
            ->addPhone('0123456789')
            ->addEmail('julia@example.com');

        $this->assertTrue($t->getName() === 'Julia');
        $this->assertTrue($t->getAge() === 21);
        $this->assertTrue($t->isMale() === false);
        $this->assertTrue($t->getPhone() === '0123456789');
        $this->assertTrue($t->getEmail() === 'julia@example.com');
        
        $t->setMale(1);
        $this->assertTrue($t->isMale() === true);
    }
}
