
/**
 * Control Flow
 */

namespace Test;

class Assign
{

	public function testAssign1()
	{
		int a;
		let a = 1;
		return a;
	}

	public function testAssign2()
	{
		int a;
		let a = true;
		return a;
	}

	public function testAssign3()
	{
		int a;
		let a = false;
		return a;
	}

	public function testAssign4()
	{
		int a;
		let a = null;
		return a;
	}

	public function testAssign5()
	{
		int a;
		let a = 2.0;
		return a;
	}

	public function testAssign6()
	{
		boolean a;
		let a = 1;
		return a;
	}

	public function testAssign7()
	{
		boolean a;
		let a = 1.0;
		return a;
	}

	public function testAssign8()
	{
		boolean a;
		let a = true;
		return a;
	}

	public function testAssign9()
	{
		boolean a;
		let a = false;
		return a;
	}

	public function testAssign10()
	{
		boolean a;
		let a = null;
		return a;
	}

	public function testAssign11()
	{
		double a;
		let a = null;
		return a;
	}

	public function testAssign12()
	{
		double a;
		let a = 4;
		return a;
	}

	public function testAssign13()
	{
		double a;
		let a = false;
		return a;
	}

	public function testAssign14()
	{
		double a;
		let a = true;
		return a;
	}

	public function testAssign15()
	{
		double a;
		let a = 5.0;
		return a;
	}

	public function testAssign16()
	{
		var a;
		let a = 1;
		return a;
	}

	public function testAssign17()
	{
		var a;
		let a = 1.0;
		return a;
	}

	public function testAssign18()
	{
		var a;
		let a = false;
		return a;
	}

	public function testAssign19()
	{
		var a;
		let a = true;
		return a;
	}

	public function testAssign20()
	{
		var a;
		let a = null;
		return a;
	}

	public function testAssign21()
	{
		int a, b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign22()
	{
		double a, b;
		let a = 1.0,
			b = a;
		return b;
	}

	public function testAssign23()
	{
		boolean a, b;
		let a = true,
			b = a;
		return b;
	}

	public function testAssign24()
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign25()
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign26()
	{
		int b;
		double a;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign27()
	{
		int b;
		double a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign28()
	{
		int b;
		boolean a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign29()
	{
		double b;
		boolean a;
		let b = 1.0,
			a = b;
		return a;
	}

	public function testAssign30()
	{
		boolean b;
		int a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign31()
	{
		boolean b;
		double a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign32()
	{
		boolean b;
		var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign33()
	{
		boolean b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign34()
	{
		int b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign35()
	{
		double b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign36()
	{
		var b; var a;
		let b = false,
			a = b;
		return a;
	}

}
