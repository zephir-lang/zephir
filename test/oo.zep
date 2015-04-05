/**
 * OO operations
 */

namespace Test;

class Oo
{
	public function testInstance1()
	{
		var o;
		let o = new \stdClass;
		return o;
	}

	public function testInstance2()
	{
		var o;
		let o = new Oo\OoConstruct;
		return o;
	}

	public function testInstance3()
	{
		var o;

		let o = new Oo\OoNoConstruct;

		return o;
	}

	public function testInstance4()
	{
		var o, a, b;

		let a = "a",
			b = "b",
			o = new Oo\OoConstructParams(a, b);

		return o;
	}

	public function testInstance5()
	{
		var o;
		let o = new Oo\OoConstructParams("a", "b");
		return o;
	}

	public function testInstance6()
	{
		var o;
		let o = new Oo\OoConstructParams(1, 2);
		return o;
	}

	public function testInstance7()
	{
		var o;
		let o = new Oo\OoConstructParams(false, true);
		return o;
	}

	public function testInstance8()
	{
		var o;
		let o = new Oo\OoConstructParams(1.2, 7.30);
		return o;
	}

	public function testInstance9()
	{
		var o;
		let o = Oo\OoDynamicA::getNew();
		return o;
	}

	public function testInstance10()
	{
		var o;
		let o = Oo\OoDynamicB::getNew();
		return o;
	}

	public function testInstance11()
	{
		var o;
		let o = new Oo\OoConstructParams(1, 2);
		return o;
	}
}
