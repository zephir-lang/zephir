
namespace Stub\Optimizers;

class Cos
{
	public function testInt()
	{
		int a = 4;

		return cos(a);
	}

	public function testVar()
	{
		var a = 4;

		return cos(a);
	}

	public function testIntValue1()
	{
		return cos(4);
	}

	public function testIntValue2()
	{
		return cos(16);
	}

	public function testIntParameter(int a)
	{
		return cos(a);
	}

	public function testVarParameter(var a)
	{
		return cos(a);
	}
}
