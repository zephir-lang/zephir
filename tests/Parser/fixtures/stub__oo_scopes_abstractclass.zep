namespace Stub\Oo\Scopes;

abstract class AbstractClass
{
    private privateProperty2 = "private2";
    protected protectedProperty2 = "protected2";

    public function setProperty(string name, var value) -> var
    {
        let this->{name} = value;

        return this;
    }

    public function getPrivateProperty2() -> var
    {
        return this->privateProperty2;
    }

    public function getProtectedProperty2() -> var
    {
        return this->protectedProperty2;
    }
}
