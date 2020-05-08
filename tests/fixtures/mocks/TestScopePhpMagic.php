<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */
class TestScopePhpMagic
{
    public $setCount = 0;

    private $privateProperty = 'private';
    protected $protectedProperty = 'protected';
    public $publicProperty = 'public';

    public function __set($name, $value)
    {
        ++$this->setCount;
        $this->$name = $value;
    }

    public function __get($name)
    {
        return $this->$name;
    }
}
