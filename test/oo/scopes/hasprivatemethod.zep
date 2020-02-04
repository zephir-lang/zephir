namespace Test\Oo\Scopes;

class HasPrivateMethod
{
    private test;

    public function __construct()
    {
        let this->test = "test";
    }

	public function callPrivateMethod() -> string
	{
		return this->isPrivate();
	}

	private function isPrivate() -> string
	{
		return __FUNCTION__;
	}
}
