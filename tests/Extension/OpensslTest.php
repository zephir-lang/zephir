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
use Stub\Openssl;

final class OpensslTest extends TestCase
{
    private Openssl $test;

    protected function setUp(): void
    {
        $this->test = new Openssl();
    }

    /**
     * @dataProvider providerLength
     */
    public function testRandomPseudoBytes(int $length): void
    {
        for ($i = 0; $i < 20; $i++) {
            $this->assertSame($length, strlen($this->test->randomPseudoBytes($length)));
        }
    }

    public function testRandomPseudoBytesLoop(): void
    {
        $this->assertSame(10, count($this->test->randomPseudoBytesLoop()));
    }

    public function providerLength(): array
    {
        return [
            [1],
            [2],
            [5],
            [10],
            [30],
            [50],
            [100],
        ];
    }
}
