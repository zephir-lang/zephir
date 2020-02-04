namespace Test\Oo\Scopes;

class HasPrivateProtectedProperty
{
    private privateProperty;

    protected protectedProperty;

    public publicProperty;

    public function __construct
    {
        let this->privateProperty = "private";
        let this->protectedProperty = "protected";
        let this->publicProperty = "public";
    }
}
