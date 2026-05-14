<?php

namespace Stub\PhpStan;

/**
 * Class with non-whitelisted custom tags.
 *
 * @phan-template T
 * @my-custom-tag something
 */
class PhpStanNegative
{


    /**
     * Method with non-whitelisted tags.
     *
     * @phan-return Foo
     * @my-internal-marker
     */
    public function doStuff()
    {
    }
}
