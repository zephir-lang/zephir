namespace Stub\Interfaces;

class ImplementInt implements InterfaceInt
{
    protected val = 0;

    public function set(int val) -> void
    {
        let this->val = val;
    }

    public function get() -> int
    {
        return this->val;
    }
}
