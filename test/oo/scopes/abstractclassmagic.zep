namespace Test\Oo\Scopes;

abstract class AbstractClassMagic
{
    public setCount = 0;
    private privateProperty = "private";
    protected protectedProperty = "protected";
    public publicProperty = "public";

    public function __set(var name, var value)
    {
        let this->{name} = value;
        let this->setCount = this->setCount + 1;
    }

    public function __get(var name)
    {
        return this->{name};
    }
}
