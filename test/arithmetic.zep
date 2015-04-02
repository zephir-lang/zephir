
/**
 * Arithmetic operations
 */

namespace Test;

class Arithmetic
{
	protected tmp1 = 100;

	/* ---------- SUM ----------------*/

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

	public function boolSumExpression()
	{
		boolean a;
		let a = true;
		return a + exp(0);
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

	public function doubleSumExpression()
	{
		return 1.0 + exp(0);
	}

	public function doubleSumVarExpression()
	{
		double a;
		let a = 1.0;
		return a + exp(0);
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

	public function addSum2b()
	{
		int a;
		let a = 5,
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

	public function addSum8b()
	{
		double a;
		let a = 6.3,
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
		int a, b;
		let a = 0,
			b = 10,
			a += b;
		return a;
	}

	public function addSum13()
	{
		int a; boolean b;
		let a = 0,
			b = true,
			a += b;
		return a;
	}

	public function addSum14()
	{
		int a; boolean b;
		let a = 0,
			b = false,
			a += b;
		return a;
	}

	public function addSum15()
	{
		int a; double b;
		let a = 0,
			b = 1.0,
			a += b;
		return a;
	}

	public function addSum16()
	{
		double a, b;
		let a = 0.0,
			b = 10.0,
			a += b;
		return a;
	}

	public function addSum17()
	{
		double a; boolean b;
		let a = 0.0,
			b = true,
			a += b;
		return a;
	}

	public function addSum18()
	{
		double a; boolean b;
		let a = 0.0,
			b = false,
			a += b;
		return a;
	}

	public function addSum19()
	{
		double a; int b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum20()
	{
		int a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum21()
	{
		double a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum22()
	{
		var a; var b;
		let a = 0.0,
			b = 1,
			a += b;
		return a;
	}

	public function addSum23()
	{
		var a;

		let a = 1;
		let a += 1 << 10;

		return a;
	}

	public function addSum24(var a)
	{
		let a += 1 << 10;

		return a;
	}

	/* --------------- SUB ----------------*/

	public function intSub()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function intLetSub()
	{
		int b, c;

		let c = 1,
			b = 2,
			c -= b;

		return c;
	}

    public function intSub2()
    {
        int a, c;

        let a = 1,
            c = a - 2;

        return c;
    }

	public function intSubSimple()
	{
		int c;

		let c = 1 - 2;
		return c;
	}

	public function boolSub()
	{
		boolean a, b, c;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function bool2Sub()
	{
		boolean a, c;

		let a = 1,
			c = a - 2;

		return c;
	}

	public function bool3Sub()
	{
		boolean a, c;

		let a = true,
			c = a - false;

		return c;
	}

	public function bool4Sub()
	{
		boolean a, c;

		let a = true,
			c = a - true;

		return c;
	}

	public function boolSubSimple()
	{
		boolean c;

		let c = 1 - 2;
		return c;
	}

	public function doubleSub()
	{
		double a, b, c;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function double2Sub()
	{
		double a, b, c;

		let a = 1.0,
			b = 2.0,
			c = a - b;

		return c;
	}

	public function doubleSubSimple()
	{
		double c;

		let c = 1 - 2;
		return c;
	}

	public function doubleSub2Simple()
	{
		double c;

		let c = 1.0 - 2.0;
		return c;
	}

	public function varSub()
	{
		var a, b, c;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function varSubSimple()
	{
		var c;

		let c = 1 - 2;
		return c;
	}

	public function intDoubleSub()
	{
		int b, c;
		double a;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function intDoubleSubSimple()
	{
		int c;

		let c = 1 - 2.0;
		return c;
	}

	public function doubleIntSub()
	{
		int b;
		double a, c;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function doubleIntSubSimple()
	{
		double c;

		let c = 1 - 2.0;
		return c;
	}

	public function varIntSub()
	{
		var b, c;
		int a;

		let a = 1,
			b = 2,
			c = a - b;

		return c;
	}

	public function intVarSub()
	{
		var b;
		int c, a;

		let a = 1,
			b = 2,
			c = b - a;

		return c;
	}

	public function intVarImplicitCastSub()
	{
		var a, b;
		int c;

		let a = "1",
			b = "2",
			c = b - a;

		return c;
	}

	public function intVarImplicitCast2Sub()
	{
		var a, b;
		int c;

		let a = "1",
			b = 2,
			c = b - a;

		return c;
	}

	public function complexSub()
	{
		var c;
		let c = 2 - 1.0;
		return c;
	}

	public function complex2Sub()
	{
		var c;
		let c = true - true;
		return c;
	}

	public function complex3Sub()
	{
		var c;
		let c = true - 1.0;
		return c;
	}

	public function complex4Sub()
	{
		var c;
		let c = 1.0 - true;
		return c;
	}

	public function complex5Sub()
	{
		var c;
		let c = 1 - true;
		return c;
	}

	public function complex6Sub()
	{
		var c;
		let c = true - 1;
		return c;
	}

	public function complex7Sub()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = a - b;
		return c;
	}

	public function complex9Sub()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = b - a;
		return c;
	}

	public function complex10Sub()
	{
		boolean a; int b; var c;

		let a = 1.0,
			b = 2,
			c = b - a;
		return c;
	}

	public function complex11Sub()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex12Sub()
	{
		boolean a; int b; int c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex13Sub()
	{
		boolean a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex14Sub()
	{
		var a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex15Sub()
	{
		var a; var b; boolean c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex16Sub()
	{
		var a; var b; double c;

		let a = 1,
			b = 2.0,
			c = b - a;
		return c;
	}

	public function complex17Sub()
	{
		var a; boolean b; double c;

		let a = 1,
			b = 2.0,
			c = a - b;
		return c;
	}

	public function complex18Sub()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a - b,
			c = a - b - d;
		return c;
	}

	public function complex19Sub()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a - b,
			c = a - d - b;
		return c;
	}

	public function complex20Sub()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a - b,
			c = d - d - d - d;
		return c;
	}

	public function complex21Sub()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a - b,
			c = b - d - b - d - d - b - d - b;
		return c;
	}

	public function complex22Sub()
	{
		var a, d; int b; double c;

		let a = 1,
			b = 2.0,
			d = a - b,
			c = b - d - d - d - d - b - d - b;
		return c;
	}

	public function complex23Sub()
	{
		var a; boolean b; double d;
		let a = 1,
			b = 2.0,
			d = a - b;
		return d;
	}

	public function complex24Sub()
	{
		var a; var d; boolean b;
		let a = 1,
			b = 2.0,
			d = 1 - a - false - b;
		return d;
	}

	public function sub1()
	{
		int a;
		let a = 0,
			a -= 10;
		return a;
	}

	public function sub2()
	{
		int a;
		let a = 0,
			a -= null;
		return a;
	}

	public function sub2b()
	{
		int a;
		let a = 5,
			a -= null;
		return a;
	}

	public function sub3()
	{
		int a;
		let a = 0,
			a -= true;
		return a;
	}

	public function sub4()
	{
		int a;
		let a = 0,
			a -= false;
		return a;
	}

	public function sub5()
	{
		int a;
		let a = 0,
			a -= 1.0;
		return a;
	}

	public function sub6()
	{
		int a;
		let a = 0,
			a -= 1.0;
		return a;
	}

	public function sub7()
	{
		double a;
		let a = 0.0,
			a -= 10;
		return a;
	}

	public function sub8()
	{
		double a;
		let a = 0.0,
			a -= null;
		return a;
	}

	public function sub8b()
	{
		double a;
		let a = 6.3,
			a -= null;
		return a;
	}

	public function sub9()
	{
		double a;
		let a = 0.0,
			a -= true;
		return a;
	}

	public function sub10()
	{
		double a;
		let a = 0.0,
			a -= false;
		return a;
	}

	public function sub11()
	{
		double a;
		let a = 0.0,
			a -= 1.0;
		return a;
	}

	public function sub12()
	{
		int a, b;
		let a = 0,
			b = 10,
			a -= b;
		return a;
	}

	public function sub13()
	{
		int a; boolean b;
		let a = 0,
			b = true,
			a -= b;
		return a;
	}

	public function sub14()
	{
		int a; boolean b;
		let a = 0,
			b = false,
			a -= b;
		return a;
	}

	public function sub15()
	{
		int a; double b;
		let a = 0,
			b = 1.0,
			a -= b;
		return a;
	}

	public function sub16()
	{
		double a, b;
		let a = 0.0,
			b = 10.0,
			a -= b;
		return a;
	}

	public function sub17()
	{
		double a; boolean b;
		let a = 0.0,
			b = true,
			a -= b;
		return a;
	}

	public function sub18()
	{
		double a; boolean b;
		let a = 0.0,
			b = false,
			a -= b;
		return a;
	}

	public function sub19()
	{
		double a; int b;
		let a = 0.0,
			b = 1,
			a -= b;
		return a;
	}

	public function sub20()
	{
		int a; var b;
		let a = 0.0,
			b = 1,
			a -= b;
		return a;
	}

	public function sub21()
	{
		double a; var b;
		let a = 0.0,
			b = 1,
			a -= b;
		return a;
	}

	public function sub22()
	{
		var a; var b;
		let a = 0.0,
			b = 1,
			a -= b;
		return a;
	}

	public function sub23()
	{
		var a;

		let a = 1;
		let a -= 1 << 10;

		return a;
	}

	public function sub24(var a)
	{
		let a -= 1 << 10;

		return a;
	}

	public function mul1()
	{
		var a = 1;
		let a *= 5;

		return a;
	}

	public function mul2(var a)
	{
		let a *= 5;

		return a;
	}

	public function mul3()
	{
		var a = 1;
		let a *= 1 << 10;

		return a;
	}

	/* Less */

	public function less1()
	{
		var a = 1, b = 2;

		return a < b;
	}

	public function less2()
	{
		var a = 2, b = 1;

		return a < b;
	}

	public function less3(var a)
	{
		return a < 5;
	}

	public function less4(var a)
	{
		return a < 5.0;
	}

	/* Greater */

	public function greater1()
	{
		var a = 1, b = 2;

		return a > b;
	}

	public function greater2()
	{
		var a = 2, b = 1;

		return a > b;
	}

	public function greater3(var a)
	{
		return a > 5;
	}

	public function greater4(var a)
	{
		return a > 5.0;
	}

	public function letStatementIntMinus()
	{
		int a;
		let a = -1;
		return a;
	}

	public function declaredIntMinus()
	{
		int a = -1;
		return a;
	}

	/**
	 * Boolean var can`t be -1 when we set -1
	 */
	public function letStatementBoolMinus(var b)
	{
		boolean a;
		let a = -b;
		return a;
	}

	public function letStatementVarMinus(var b)
	{
		var a;
		let a = -b;
		return a;
	}

	public function div1()
	{
		var a = 100;
		return (a - 1) / 4;
	}

	public function div2()
	{
		return (this->tmp1 - 1) / 4;
	}
}
