
namespace Stub\Optimizers;

class Tan
{
	public function testInt()
	{
		int a = 0;

		return tan(a);
	}

	public function testVar()
	{
		var a = 0;

		return tan(a);
	}

	public function testVar2()
	{
		var a = 1;

		return tan(a);
	}


	public function testIntValue1()
	{
		return tan(1);
	}

	public function testIntValue2()
	{
		return tan(2);
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
