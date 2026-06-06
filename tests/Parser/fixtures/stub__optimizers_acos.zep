
namespace Stub\Optimizers;

class ACos
{
	public function testInt()
	{
		int a = 1;

		return acos(a);
	}

	public function testVar()
	{
		var a = 1;

		return acos(a);
	}

	public function testIntValue1()
	{
		return acos(0);
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
