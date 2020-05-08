
namespace Stub\Optimizers;

class ASin
{
	public function testInt()
	{
		int a = 1;

		return asin(a);
	}

	public function testVar()
	{
		var a = 1;

		return asin(a);
	}

	public function testIntValue1()
	{
		return asin(0);
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
