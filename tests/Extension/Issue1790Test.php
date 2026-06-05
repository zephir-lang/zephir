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
use Stub\Issue1790;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1790
 */
final class Issue1790Test extends TestCase
{
    public function testNonConstantBoolToStringDynamicProperty(): void
    {
        $test = new Issue1790();

        $test->setStringProp(10);
        $this->assertTrue($test->flag);

        $test->setStringProp(3);
        $this->assertFalse($test->flag);
    }

    public function testNonConstantBoolToVariableDynamicProperty(): void
    {
        $test = new Issue1790();

        $test->setVariableProp('flag', 10);
        $this->assertTrue($test->flag);

        $test->setVariableProp('flag', 1);
        $this->assertFalse($test->flag);
    }
}
