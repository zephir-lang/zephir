
namespace Stub\Optimizers;

class Ldexp
{
	public function testInt()
	{
		int x = 2;
		int exponent = 3;

		return ldexp(x, exponent);
	}

	public function testDoubleInt()
	{
		double x = 2.0;
		int exponent = 3;

		return ldexp(x, exponent);
	}

	public function testDouble()
	{
		double x = 2.0;
		double exponent = 3.0;

		return ldexp(x, exponent);
	}

	public function testVar()
	{
		var x = 2;
		var exponent = 3;

		return ldexp(x, exponent);
	}

	public function testIntValue1()
	{
		return ldexp(2, 3);
	}

	public function testIntParameter(int x, int exponent)
	{
		return ldexp(x, exponent);
	}

	public function testVarParameter(var x, var exponent)
	{
		return ldexp(x, exponent);
	}
}
