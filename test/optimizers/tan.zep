
namespace Test\Optimizers;

class Tan
{
	public function testInt()
	{
		int a = 4;

		return tan(a);
	}

	public function testVar()
	{
		var a = 4;

		return tan(a);
	}

	public function testIntValue1()
	{
		return tan(4);
	}

	public function testIntValue2()
	{
		return tan(16);
	}

	public function testIntParameter(int a)
	{
		return tan(a);
	}

	public function testVarParameter(var a)
	{
		return tan(a);
	}
}
