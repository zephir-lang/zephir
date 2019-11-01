<?php

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\McallInternal;

class McallInternalTest extends TestCase
{
    /**
     * @test
     * @see https://github.com/phalcon/zephir/issues/1956
     */
    public function shouldReturnInstance()
    {
        $test = new McallInternal();
        $this->assertInstanceOf(McallInternal::class, $test->issue1956());
    }
}
