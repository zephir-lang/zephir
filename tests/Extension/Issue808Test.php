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
use Stub\Issue808;

/**
 * Regression test for https://github.com/zephir-lang/zephir/issues/808
 *
 * unset(obj->{variable}) must compile and execute correctly.
 * Before the fix the compiler threw:
 *   CompilerException: Cannot use expression type: property-dynamic-access in "unset"
 */
final class Issue808Test extends TestCase
{
    private Issue808 $obj;

    protected function setUp(): void
    {
        $this->obj = new Issue808();
    }

    /**
     * After unsetting with a variable key the property must no longer be set.
     */
    public function testDynamicUnset(): void
    {
        $this->assertFalse($this->obj->testDynamicUnset('myProp'));
    }

    /**
     * Only the named property is removed; others are preserved.
     */
    public function testDynamicUnsetSelective(): void
    {
        $result = $this->obj->testDynamicUnsetSelective('remove');

        $this->assertObjectHasProperty('keep', $result);
        $this->assertObjectNotHasProperty('remove', $result);
        $this->assertSame('keep_value', $result->keep);
    }

    /**
     * unset(obj->{"literal"}) — string literal in braces — must also work.
     */
    public function testDynamicUnsetStringKey(): void
    {
        $this->assertFalse($this->obj->testDynamicUnsetStringKey());
    }
}
