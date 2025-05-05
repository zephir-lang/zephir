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
use Stub\Closures;

final class ClosureTest extends TestCase
{
    public function testUseCommand(): void
    {
        $test = new Closures();

        $this->assertSame(2, $test->testUseCommand(1)());
        $this->assertInstanceOf(\stdClass::class, $test->issue642());
    }

    public function testUseCommandMultiple(): void
    {
        $test = new Closures();

        $callbackA = $test->testUseCommand(1);
        $callbackB = $test->testUseCommand(2);

        $this->assertSame(2, $callbackA());
        $this->assertSame(3, $callbackB());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1036
     */
    public function testIssue1036(): void
    {
        $test = new Closures();

        $test->issue1036SetArgument(true);
        $test->issue1036SetFunction(fn ($argument) => $argument);

        $this->assertTrue($test->issue1036Call());
    }
}
