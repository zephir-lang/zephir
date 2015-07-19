
namespace Test\Optimizers;

class Sqrt
{
	public function testInt()
	{
		int a = 4;

		return sqrt(a);
	}

	public function testVar()
	{
		var a = 4;

		return sqrt(a);
	}

	public function testIntValue1()
	{
		return sqrt(4);
	}

	public function testIntValue2()
	{
		return sqrt(16);
	}

	public function testIntParameter(int a)
	{
		return sqrt(a);
	}

	public function testVarParameter(var a)
	{
        var_dump(a);
        var_dump(sqrt(a));
        var_dump(sqrt(16));
		return sqrt(a);
	}
}
