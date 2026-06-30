namespace Stub;

abstract class Issue1628Abstract
{
    public function concrete() -> string
    {
        return "concrete";
    }

    abstract public function mustImplement() -> string;
}
