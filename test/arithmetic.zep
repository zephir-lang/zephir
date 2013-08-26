
/**
 * Arithmetic operations
 */

namespace Test;

class Arithmetic
{
	public function intSum()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function int2Sum()
	{
		int a, c;

		let a = 1,
			c = a + 2;

		return c;
	}

	public function intSumSimple()
	{
		int c;

		let c = 1 + 2;
		return c;
	}

	public function boolSum()
	{
		boolean a, b, c;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function bool2Sum()
	{
		boolean a, c;

		let a = 1,
			c = a + 2;

		return c;
	}

	public function bool3Sum()
	{
		boolean a, c;

		let a = true,
			c = a + false;

		return c;
	}

	public function boolSumSimple()
	{
		boolean c;

		let c = 1 + 2;
		return c;
	}

	public function doubleSum()
	{
		double a, b, c;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function double2Sum()
	{
		double a, b, c;

		let a = 1.0,
			b = 2.0,
			c = a + b;

		return c;
	}

	public function doubleSumSimple()
	{
		double c;

		let c = 1 + 2;
		return c;
	}

	public function doubleSum2Simple()
	{
		double c;

		let c = 1.0 + 2.0;
		return c;
	}

	public function varSum()
	{
		var a, b, c;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function varSumSimple()
	{
		var c;

		let c = 1 + 2;
		return c;
	}

	public function intDoubleSum()
	{
		int b, c;
		double a;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function intDoubleSumSimple()
	{
		int c;

		let c = 1 + 2.0;
		return c;
	}

	public function doubleIntSum()
	{
		int b;
		double a, c;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function doubleIntSumSimple()
	{
		double c;

		let c = 1 + 2.0;
		return c;
	}

	public function varIntSum()
	{
		var b, c;
		int a;

		let a = 1,
			b = 2,
			c = a + b;

		return c;
	}

	public function intVarSum()
	{
		var b;
		int c, a;

		let a = 1,
			b = 2,
			c = b + a;

		return c;
	}

	public function intVarImplicitCastSum()
	{
		var a, b;
		int c;

		let a = "1",
			b = "2",
			c = b + a;

		return c;
	}

	public function intVarImplicitCast2Sum()
	{
		var a, b;
		int c;

		let a = "1",
			b = 2,
			c = b + a;

		return c;
	}

	public function complexSum()
	{
		var c;
		let c = 2 + 1.0;
		return c;
	}

	public function complex2Sum()
	{
		var c;
		let c = true + true;
		return c;
	}

	public function complex3Sum()
	{
		var c;
		let c = true + 1.0;
		return c;
	}

	public function complex4Sum()
	{
		var c;
		let c = 1.0 + true;
		return c;
	}

	public function complex5Sum()
	{
		var c;
		let c = 1 + true;
		return c;
	}

	public function complex6Sum()
	{
		var c;
		let c = true + 1;
		return c;
	}

	public function complex7Sum()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = a + b;
		return c;
	}

	public function complex9Sum()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = b + a;
		return c;
	}

	public function complex10Sum()
	{
		boolean a; int b; var c;

		let a = 1.0,
			b = 2,
			c = b + a;
		return c;
	}

	public function complex11Sum()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex12Sum()
	{
		boolean a; int b; int c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex13Sum()
	{
		boolean a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex14Sum()
	{
		var a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex15Sum()
	{
		var a; var b; boolean c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex16Sum()
	{
		var a; var b; double c;

		let a = 1,
			b = 2.0,
			c = b + a;
		return c;
	}

	public function complex17Sum()
	{
		var a; boolean b; double c;

		let a = 1,
			b = 2.0,
			c = a + b;
		return c;
	}

	public function complex18Sum()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a + b,
			c = a + b + d;
		return c;
	}

	public function complex19Sum()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a + b,
			c = a + d + b;
		return c;
	}

	public function complex20Sum()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a + b,
			c = d + d + d + d;
		return c;
	}

	public function complex21Sum()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a + b,
			c = b + d + b + d + d + b + d + b;
		return c;
	}

	public function complex22Sum()
	{
		var a, d; int b; double c;

		let a = 1,
			b = 2.0,
			d = a + b,
			c = b + d + d + d + d + b + d + b;
		return c;
	}

	public function complex23Sum()
	{
		var a; boolean b; double d;
		let a = 1,
			b = 2.0,
			d = a + b;
		return d;
	}

	public function complex24Sum()
	{
		var a; var d; boolean b;
		let a = 1,
			b = 2.0,
			d = 1 + a + false + b;
		return d;
	}

	public function addSum1()
	{
		int a;
		let a = 0,
			a += 10;
		return a;
	}

	public function addSum2()
	{
		int a;
		let a = 0,
			a += null;
		return a;
	}

	public function addSum3()
	{
		int a;
		let a = 0,
			a += true;
		return a;
	}

	public function addSum4()
	{
		int a;
		let a = 0,
			a += false;
		return a;
	}

	public function addSum5()
	{
		int a;
		let a = 0,
			a += 1.0;
		return a;
	}

	public function addSum6()
	{
		int a;
		let a = 0,
			a += 1.0;
		return a;
	}

	public function addSum7()
	{
		double a;
		let a = 0.0,
			a += 10;
		return a;
	}

	public function addSum8()
	{
		double a;
		let a = 0.0,
			a += null;
		return a;
	}

	public function addSum9()
	{
		double a;
		let a = 0.0,
			a += true;
		return a;
	}

	public function addSum10()
	{
		double a;
		let a = 0.0,
			a += false;
		return a;
	}

	public function addSum11()
	{
		double a;
		let a = 0.0,
			a += 1.0;
		return a;
	}

	public function addSum12()
	{
		double a;
		let a = 0.0,
			a += 1.0;
		return a;
	}

	public function addSum13()
	{
		int a, b;
		let a = 0,
			b = 10,
			a += b;
		return a;
	}

	public function addSum14()
	{
		int a; boolean b;
		let a = 0,
			b = true,
			a += b;
		return a;
	}

	public function addSum15()
	{
		int a; boolean b;
		let a = 0,
			b = false,
			a += b;
		return a;
	}

	public function addSum16()
	{
		int a; double b;
		let a = 0,
			b = 1.0,
			a += b;
		return a;
	}

	public function addSum17()
	{
		double a, b;
		let a = 0.0,
			b = 10.0,
			a += b;
		return a;
	}

	public function addSum18()
	{
		double a; boolean b;
		let a = 0.0,
			b = true,
			a += b;
		return a;
	}

	public function addSum19()
	{
		double a; boolean b;
		let a = 0.0,
			b = false,
			a += b;
		return a;
	}

	public function addSum20()
	{
		double a; int b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum21()
	{
		int a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum22()
	{
		double a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum23()
	{
		var a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function complexSub()
	{
		var c;
		let c = 2 - 1.0;
		return c;
	}

}
