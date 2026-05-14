
namespace Stub;

class Returns
{
    protected stringDefaultNull  = null;
    protected intDefaultNull     = null;
    protected arrayDefaultNull   = null;
    protected doubleDefaultNull  = null;
    protected validString        = "hello";
    protected nullableProperty   = null;

	public function testReturnCast1() -> int
	{
		return (int) 5.0;
	}

	public function testReturnCast2() -> int
	{
		return (int) true;
	}

	public function testReturnCast3() -> int
	{
		return (int) false;
	}

	public function testReturnCast4() -> int
	{
		if true {
			return (int)1;
		} else {
			return (int)0;
		}
	}

	public static function returnWithParameter(string parameter)
	{
		return "Return back";
	}

	public static function returnWithoutParameter()
	{
		return "Return back";
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
	public function returnNullOnString() -> string
	{
	    return this->stringDefaultNull;
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function returnNullOnInt() -> int
    {
        return this->intDefaultNull;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function returnNullOnArray() -> array
    {
        return this->arrayDefaultNull;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function returnNullOnDouble() -> double
    {
        return this->doubleDefaultNull;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     * Happy path: the property already holds a string, so no TypeError.
     */
    public function returnValidString() -> string
    {
        return this->validString;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     * Nullable return type — returning null must NOT throw.
     */
    public function returnNullOnNullableString() -> string | null
    {
        return this->nullableProperty;
    }
}
