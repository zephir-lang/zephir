namespace Test\Oo\Scopes;

class HasPrivateMethod
{
    public function callPrivateMethod() -> string
    {
        return this->isPrivate();
    }

    private function isPrivate() -> string
    {
        return __FUNCTION__;
    }
}
