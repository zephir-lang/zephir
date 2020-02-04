<?php

class TestScopeExtendingMagic extends Test\Oo\Scopes\AbstractClassMagic
{
    private $privateProperty2 = 'private';

    /**
     * @return string
     */
    public function getProtectedProperty()
    {
        return $this->protectedProperty;
    }
}

