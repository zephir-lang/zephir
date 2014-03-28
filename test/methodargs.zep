namespace Test;

class MethodArgs
{
    public a;

    public function setCallable(callable a)
    {
        let this->a = a;
    }

    public function setObject(object a)
    {
        let this->a = a;
    }

    public function setCallableStrict(callable! a)
    {
    	let this->a = a;
    }

    public function setObjectStrict(object! a)
    {
    	let this->a = a;
    }
}
