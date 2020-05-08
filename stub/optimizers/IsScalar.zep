
namespace Stub\Optimizers;

class IsScalar
{
	public function testIntVar() -> bool
	{
		int a = 1;
		return is_scalar(a);
	}

    public function testDoubleVar() -> bool
    {
        double a = 1;
        return is_scalar(a);
    }

    public function testBoolVar() -> bool
    {
        bool a = true;
        return is_scalar(a);
    }

    public function testStringVar() -> bool
    {
        string a = "test string";
        return is_scalar(a);
    }

    public function testEmptyArrayVar() -> bool
    {
        array a = [];
        return is_scalar(a);
    }

    public function testVar() -> bool
    {
        var a = 1;
        return is_scalar(a);
    }

    public function testVarParameter(var a) -> bool
    {
        return is_scalar(a);
    }
}
