<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

use Test\Oo\Scopes\AbstractClassMagic;

class TestScopeExtendingMagic extends AbstractClassMagic
{
    private $privateProperty2 = 'private';

    public function getProtectedProperty(): string
    {
        return $this->protectedProperty;
    }

    public function getPrivateProperty2()
    {
        return $this->privateProperty2;
    }
}
