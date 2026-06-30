
namespace Stub\Optimizers;

class Sin
{
	public function testInt()
	{
		int a = 4;

		return sin(a);
	}

	public function testVar()
	{
		var a = 4;

		return sin(a);
	}

	public function testIntValue1()
	{
		return sin(4);
	}

	public function testIntValue2()
	{
		return sin(16);
	}

	public function testIntParameter(int a)
	{
		return sin(a);
	}

	public function testVarParameter(var a)
	{
		return sin(a);
	}
}
