
namespace Stub\Types;

class Unions
{
    public int|float num = 1;
    public int|string|null note = null;
    public string|<MayBe> tag;

    public function kind(int|float value) -> string
    {
        return typeof value;
    }

    public function pick(string|<MayBe> value) -> string
    {
        return typeof value;
    }
}
