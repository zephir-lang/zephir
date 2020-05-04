<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

use Test\Oo\Scopes\AbstractClass;

class TestScopeExtending extends AbstractClass
{
    private $privateProperty = 'private';
    protected $protectedProperty = 'protected';
    public $publicProperty = 'public';

    /**
     * @return string
     */
    public function getProtectedProperty(): string
    {
        return $this->protectedProperty;
    }
}
