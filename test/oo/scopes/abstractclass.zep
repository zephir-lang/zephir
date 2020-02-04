namespace Test\Oo\Scopes;

abstract class AbstractClass
{
    private privateProperty2 = "private2";
    protected protectedProperty2 = "protected2";

    public function setProperty(var name, var value)
    {
        let this->{name} = value;

        return this;
    }

    public function getPrivateProperty2()
    {
        return this->privatePropert2;
    }

    public function getProtectedProperty2()
    {
        return this->protectedProperty2;
    }
}
