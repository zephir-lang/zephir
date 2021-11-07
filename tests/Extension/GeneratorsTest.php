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
use Stub\Generators;

final class GeneratorsTest extends TestCase
{
    private Generators $test;

    protected function setUp(): void
    {
        $this->test = new Generators();
    }

    public function testYield(): void
    {
        $this->assertInstanceOf(\Generator::class, $this->test->simpleYield());
    }
}
