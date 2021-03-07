<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\PropertyUpdate;

final class PropertyUpdateTest extends TestCase
{
    public function testUpdate11()
    {
        // before fixed. Assertion failed: (((ht)->gc.refcount == 1) || ((ht)->u.flags & (1<<6))), function _zend_hash_index_add_or_update_i
        $test = new PropertyUpdate();
        $test->p1 = [111];
        $test->update1();

        $this->assertSame($test->p1, [111, 'aaa']);
    }
}
