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
use Stub\Cast;

class CastTest extends TestCase
{
    /**
     * @issue https://github.com/phalcon/zephir/issues/1524
     */
    public function testShouldReturnResourceIdWhenCastingResourceToInteger()
    {
        $test = new Cast();

        $this->assertEquals((int) STDIN, $test->testCastStdinToInteger());
        $this->assertEquals((int) STDOUT, $test->testCastStdoutToInteger());

        $file = fopen(__DIR__.'/../../fixtures/exists.php', 'r');
        $this->assertEquals((int) $file, $test->testCastFileResourceToInteger($file));
        fclose($file);
    }
}
