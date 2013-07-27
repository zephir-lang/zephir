
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

	public function testArray11()
	{
		var a, b, c;
		let a = [1, 2, 3];
		let b = [4, 5, 6];
		let c = [a, b];
		return c;
	}

	public function testArray12()
	{
		var a, b, c;
		let a = [1, 2, 3];
		let b = [a];
		let c = [b];
		return c;
	}

	public function testArray13()
	{
		var a;
		let a = [1: "hello1", 2: "hello2", 3: "hello3"];
		return a;
	}

	public function testArray14()
	{
		var a;
		let a = ["hello1": 1, "hello2": 2, "hello3": 3];
		return a;
	}

	public function testArray15()
	{
		var a;
		let a = ["hello1": true, "hello2": false, "hello3": true];
		return a;
	}

	public function testArray16()
	{
		var a;
		let a = ["hello1": 1.0, "hello2": 2.0, "hello3": 3.0];
		return a;
	}

	public function testArray17()
	{
		var a;
		let a = ["hello1": null, "hello2": null, "hello3": null];
		return a;
	}

	public function testArray18()
	{
		var a;
		let a = ["hello1": "a", "hello2": "b", "hello3": "c"];
		return a;
	}

	public function testArray19()
	{
		var a;
		let a = [0: true, 1: false, 2: true];
		return a;
	}

	public function testArray20()
	{
		var a;
		let a = [0: 1.0, 1: 2.0, 2: 3.0];
		return a;
	}

	public function testArray21()
	{
		var a;
		let a = [0: null, 1: null, 2: null];
		return a;
	}

	public function testArray22()
	{
		var a;
		let a = [0: 4, 1: 5, 2: 6];
		return a;
	}

	public function testArray23()
	{
		var a;
		int b;

		let b = 0,
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray24()
	{
		var a;
		double b;

		let b = 0.0,
			a = [0: b, 1: b, 2: b];
		return a;
	}

}
