
namespace Test\Optimizers;

class ASin
{
	public function testInt()
	{
		int a = 4;

		return asin(a);
	}

	public function testVar()
	{
		var a = 4;

		return asin(a);
	}

	public function testIntValue1()
	{
		return asin(4);
	}

	public function testIntValue2()
	{
		return asin(16);
	}

	public function testIntParameter(int a)
	{
		return asin(a);
	}

	public function testVarParameter(var a)
	{
		return asin(a);
	}
}
