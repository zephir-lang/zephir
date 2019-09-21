<?php
namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Test\Properties\PropertyUpdate;

class PropertyUpdateTest extends TestCase
{

    public function testUpdate11()
    {
        // before fixed. Assertion failed: (((ht)->gc.refcount == 1) || ((ht)->u.flags & (1<<6))), function _zend_hash_index_add_or_update_i
        $t = new PropertyUpdate();
        $t->p1 = [111];
        $t->update1();

        $this->assertSame($t->p1, [111, 'aaa']);
    }
}
