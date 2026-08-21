namespace Stub\Issue2635;

class Impl extends Base
{
    public function go() -> string
    {
        return "inner";
    }

    public function extra() -> string
    {
        return "outer";
    }
}
