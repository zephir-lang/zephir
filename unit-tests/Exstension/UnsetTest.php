<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Exstension;

class UnsetTest
    extends \PHPUnit_Framework_TestCase
{
    public function testUnsetFromArray()
    {
        $t = new \Test\Unsettest();

        $testVar = array(1, 2, 3, 4, 5, 6);
        assert($t->testUnsetFromArray($testVar) == array(1 => 2, 3, 4, 5, 6));
        assert($t->testUnsetFromArrayByIndexVar($testVar, 0) == array(1 => 2, 3, 4, 5, 6));
    }

    public function testUnsetValueFromProperty()
    {
        $t = new \Test\Unsettest();

        $testVar = array(1, 2, 3, 4, 5, 6);
        assert($t->has('testVar') === false);
        $t->addValueToProperty('testVar', $testVar);
        assert($t->has('testVar') === true);
        $t->testUnsetValueFromProperty('testVar');
        assert($t->has('testVar') === false);
    }
}
