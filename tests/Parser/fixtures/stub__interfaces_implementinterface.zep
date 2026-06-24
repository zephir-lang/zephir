namespace Stub\Interfaces;

class ImplementInterface implements InterfaceIntSignature
{
    protected obj;

    public function get(<InterfaceInt> obj) -> int
    {
        return obj->get();
    }

    public function getVoid(<InterfaceInt> obj) -> void
    {
        let this->obj = obj;
    }

    public function getObj() -> <InterfaceInt>
    {
        return this->obj;
    }
}
