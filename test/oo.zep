
/**
 * Arithmetic operations
 */

namespace Test;

class Oo
{
	public function testInstance1()
	{
		var o;

		let o = new stdClass;

		return o;
	}

	public function testInstance2()
	{
		var o;

		let o = new Test\Oo\OoConstruct;

		return o;
	}

	public function testInstance3()
	{
		var o;

		let o = new Test\Oo\OoNoConstruct;

		return o;
	}

	public function testInstance4()
	{
		var o, a, b;

		let a = "a",
			b = "b",
			o = new Test\Oo\OoConstructParams(a, b);

		return o;
	}

}