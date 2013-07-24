
/**
 * Control Flow
 */

namespace Test;

class NativeArray
{
	public function testArray1()
	{
		var a;
		let a = [];
		return a;
	}

	public function testArray2()
	{
		var a;
		let a = [1, 2, 3];
		return a;
	}

	public function testArray3()
	{
		var a;
		let a = [1.1, 2.2, 3.3];
		return a;
	}

	public function testArray4()
	{
		var a;
		let a = [false, true, false];
		return a;
	}

	public function testArray5()
	{
		var a;
		let a = [null, null, null];
		return a;
	}

	public function testArray6()
	{
		var a;
		let a = ["x", "y", "z"];
		return a;
	}

	public function testArray7()
	{
		var d; int a, b, c;
		let a = 1, b = 2, c = 3;
		let d = [a, b, c];
		return d;
	}

	public function testArray8()
	{
		var d; double a, b, c;
		let a = 1, b = 2, c = 3;
		let d = [a, b, c];
		return d;
	}

	public function testArray9()
	{
		var d; boolean a, b, c;
		let a = true, b = false, c = true;
		let d = [a, b, c];
		return d;
	}

	public function testArray10()
	{
		var d; string a, b, c;
		let a = "hello1", b = "hello2", c = "hello3";
		let d = [a, b, c];
		return d;
	}

}
