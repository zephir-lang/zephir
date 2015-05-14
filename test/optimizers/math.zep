
namespace Test\Optimizers;

class Math
{
	public function testSqrtInt()
	{
		int a = 4;

		return sqrt(a);
	}

	public function testSqrtVar()
	{
		var a = 4;

		return sqrt(a);
	}

	public function testSqrtIntValue1()
	{
		return sqrt(4);
	}

	public function testSqrtIntValue2()
	{
		return sqrt(16);
	}

	public function testSqrtIntParameter(int a)
	{
		return sqrt(a);
	}

	public function testSqrtVarParameter(var a)
	{
		return sqrt(a);
	}
}
