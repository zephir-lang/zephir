<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php70;

use PHPUnit\Framework\TestCase;
use Test\Cast;

class CastTest extends TestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1524
     *
     * @author Alexander Andriiako <AlexNDR@phalconphp.com>
     */
    public function shouldReturnResourceIdWhenCastingResourceToInteger()
    {
        $t = new Cast();

        $this->assertEquals((int) STDIN, $t->testCastStdinToInteger());
        $this->assertEquals((int) STDOUT, $t->testCastStdoutToInteger());

        $file = fopen(__DIR__.'/../../fixtures/exists.php', 'r');
        $this->assertEquals((int) $file, $t->testCastFileResourceToInteger($file));
        fclose($file);
    }
}
