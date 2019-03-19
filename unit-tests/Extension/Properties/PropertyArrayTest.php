<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Test\Properties\PropertyArray;

class PropertyArrayTest extends TestCase
{
    public function testAssertations()
    {
        $t = new PropertyArray();
        $this->assertSame([1], $t->someArray);
    }

    public function testIssues1831()
    {
		$header = [
			"header 1",
			"header 2",
			"header 3"
		];

		$data = [
			"header 1",
			"header 2",
			"header 3",
			"data 1",
			"data 2",
			"data 3",
			"data 4",
			"data 5",
			"data 6",
			"data 7",
			"data 8"
		];

        $t = new PropertyArray();
		$t->setOtherArray($data);
        $this->assertSame($header, $t->testIssues1831());
    }
}
