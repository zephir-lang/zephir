<?php

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\McallInternal;

class McallInternalTest extends TestCase
{
    /**
     * @see https://github.com/phalcon/zephir/issues/1956
     */
    public function testShouldReturnInstance()
    {
        $test = new McallInternal();
        $this->assertInstanceOf(McallInternal::class, $test->issue1956());
    }
}
