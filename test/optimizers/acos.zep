
namespace Test\Optimizers;

class ACos
{
	public function testInt()
	{
		int a = 4;

		return acos(a);
	}

	public function testVar()
	{
		var a = 4;

		return acos(a);
	}

	public function testIntValue1()
	{
		return acos(4);
	}

	public function testIntValue2()
	{
		return acos(16);
	}

	public function testIntParameter(int a)
	{
		return acos(a);
	}

	public function testVarParameter(var a)
	{
		return acos(a);
	}
}
