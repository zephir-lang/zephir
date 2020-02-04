namespace Test\Oo\Scopes;

abstract class AbstractClass
{
    public function setProperty(var name, var value)
    {
        let this->name = value;

        return this;
    }
}
