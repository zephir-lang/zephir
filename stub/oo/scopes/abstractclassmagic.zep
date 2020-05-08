namespace Stub\Oo\Scopes;

abstract class AbstractClassMagic
{
    public setCount = 0;
    private privateProperty = "private";
    protected protectedProperty = "protected";
    public publicProperty = "public";

    public function __set(string name, var value) -> void
    {
        let this->{name} = value;
        let this->setCount = this->setCount + 1;
    }

    public function __get(string name) -> var
    {
        return this->{name};
    }
}
