
namespace Stub\Types;

class Obj
{
    public function nullableObjectReturnNull() -> object | null
    {
        return null;
    }

    public function nullableObjectReturnObj() -> object | null
    {
        return new \stdClass();
    }
}
