
namespace Stub\Types;

class MayBe
{
    public function gc(int maxlifetime) -> int | false
    {
        return 1;
    }

    public function gcFalse() -> int | false
    {
        return false;
    }
}
