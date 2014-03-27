namespace Test;

class MethodArgs
{
    protected a;

    public function setCallable(callable a)
    {
        let this->a = a;
    }

    public function setObject(object a)
    {
        let this->a = a;
    }
}
