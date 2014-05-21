
/**
 * Arithmetic operations
 */

namespace Test;

class Bitwise
{
	/* ---------- And ----------------*/

	public function intAnd()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function int2And()
	{
		int a, c;

		let a = 1,
			c = a & 2;

		return c;
	}

	public function intAndSimple()
	{
		int c;

		let c = 1 & 2;
		return c;
	}

	public function boolAnd()
	{
		boolean a, b, c;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function bool2And()
	{
		boolean a, c;

		let a = 1,
			c = a & 2;

		return c;
	}

	public function bool3And()
	{
		boolean a, c;

		let a = true,
			c = a & false;

		return c;
	}

	public function boolAndSimple()
	{
		boolean c;

		let c = 1 & 2;
		return c;
	}

	public function doubleAnd()
	{
		double a, b, c;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function double2And()
	{
		double a, b, c;

		let a = 1.0,
			b = 2.0,
			c = a & b;

		return c;
	}

	public function doubleAndSimple()
	{
		double c;

		let c = 1 & 2;
		return c;
	}

	public function doubleAnd2Simple()
	{
		double c;

		let c = 1.0 & 2.0;
		return c;
	}

	public function varAnd()
	{
		var a, b, c;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function varAndSimple()
	{
		var c;

		let c = 1 & 2;
		return c;
	}

	public function intDoubleAnd()
	{
		int b, c;
		double a;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function intDoubleAndSimple()
	{
		int c;

		let c = 1 & 2.0;
		return c;
	}

	public function doubleIntAnd()
	{
		int b;
		double a, c;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function doubleIntAndSimple()
	{
		double c;

		let c = 1 & 2.0;
		return c;
	}

	public function varIntAnd()
	{
		var b, c;
		int a;

		let a = 1,
			b = 2,
			c = a & b;

		return c;
	}

	public function intVarAnd()
	{
		var b;
		int c, a;

		let a = 1,
			b = 2,
			c = b & a;

		return c;
	}

	public function intVarImplicitCastAnd()
	{
		var a, b;
		int c;

		let a = "1",
			b = "2",
			c = b & a;

		return c;
	}

	public function intVarImplicitCast2And()
	{
		var a, b;
		int c;

		let a = "1",
			b = 2,
			c = b & a;

		return c;
	}

	public function complexAnd()
	{
		var c;
		let c = 2 & 1.0;
		return c;
	}

	public function complex2And()
	{
		var c;
		let c = true & true;
		return c;
	}

	public function complex3And()
	{
		var c;
		let c = true & 1.0;
		return c;
	}

	public function complex4And()
	{
		var c;
		let c = 1.0 & true;
		return c;
	}

	public function complex5And()
	{
		var c;
		let c = 1 & true;
		return c;
	}

	public function complex6And()
	{
		var c;
		let c = true & 1;
		return c;
	}

	public function complex7And()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = a & b;
		return c;
	}

	public function complex9And()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = b & a;
		return c;
	}

	public function complex10And()
	{
		boolean a; int b; var c;

		let a = 1.0,
			b = 2,
			c = b & a;
		return c;
	}

	public function complex11And()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex12And()
	{
		boolean a; int b; int c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex13And()
	{
		boolean a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex14And()
	{
		var a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex15And()
	{
		var a; var b; boolean c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex16And()
	{
		var a; var b; double c;

		let a = 1,
			b = 2.0,
			c = b & a;
		return c;
	}

	public function complex17And()
	{
		var a; boolean b; double c;

		let a = 1,
			b = 2.0,
			c = a & b;
		return c;
	}

	public function complex18And()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a & b,
			c = a & b & d;
		return c;
	}

	public function complex19And()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a & b,
			c = a & d & b;
		return c;
	}

	public function complex20And()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a & b,
			c = d & d & d & d;
		return c;
	}

	public function complex21And()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a & b,
			c = b & d & b & d & d & b & d & b;
		return c;
	}

	public function complex22And()
	{
		var a, d; int b; double c;

		let a = 1,
			b = 2.0,
			d = a & b,
			c = b & d & d & d & d & b & d & b;
		return c;
	}

	public function complex23And()
	{
		var a; boolean b; double d;
		let a = 1,
			b = 2.0,
			d = a & b;
		return d;
	}

	public function complex24And()
	{
		var a; var d; boolean b;
		let a = 1,
			b = 2.0,
			d = 1 & a & false & b;
		return d;
	}


	/* --------------- OR ----------------*/

	public function intOr()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function int2Or()
	{
		int a, c;

		let a = 1,
			c = a | 2;

		return c;
	}

	public function intOrSimple()
	{
		int c;

		let c = 1 | 2;
		return c;
	}

	public function boolOr()
	{
		boolean a, b, c;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function bool2Or()
	{
		boolean a, c;

		let a = 1,
			c = a | 2;

		return c;
	}

	public function bool3Or()
	{
		boolean a, c;

		let a = true,
			c = a | false;

		return c;
	}

	public function bool4Or()
	{
		boolean a, c;

		let a = true,
			c = a | true;

		return c;
	}

	public function boolOrSimple()
	{
		boolean c;

		let c = 1 | 2;
		return c;
	}

	public function doubleOr()
	{
		double a, b, c;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function double2Or()
	{
		double a, b, c;

		let a = 1.0,
			b = 2.0,
			c = a | b;

		return c;
	}

	public function doubleOrSimple()
	{
		double c;

		let c = 1 | 2;
		return c;
	}

	public function doubleOr2Simple()
	{
		double c;

		let c = 1.0 | 2.0;
		return c;
	}

	public function varOr()
	{
		var a, b, c;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function varOrSimple()
	{
		var c;

		let c = 1 | 2;
		return c;
	}

	public function intDoubleOr()
	{
		int b, c;
		double a;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function intDoubleOrSimple()
	{
		int c;

		let c = 1 | 2.0;
		return c;
	}

	public function doubleIntOr()
	{
		int b;
		double a, c;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function doubleIntOrSimple()
	{
		double c;

		let c = 1 | 2.0;
		return c;
	}

	public function varIntOr()
	{
		var b, c;
		int a;

		let a = 1,
			b = 2,
			c = a | b;

		return c;
	}

	public function intVarOr()
	{
		var b;
		int c, a;

		let a = 1,
			b = 2,
			c = b | a;

		return c;
	}

	public function intVarImplicitCastOr()
	{
		var a, b;
		int c;

		let a = "1",
			b = "2",
			c = b | a;

		return c;
	}

	public function intVarImplicitCast2Or()
	{
		var a, b;
		int c;

		let a = "1",
			b = 2,
			c = b | a;

		return c;
	}

	public function complexOr()
	{
		var c;
		let c = 2 | 1.0;
		return c;
	}

	public function complex2Or()
	{
		var c;
		let c = true | true;
		return c;
	}

	public function complex3Or()
	{
		var c;
		let c = true | 1.0;
		return c;
	}

	public function complex4Or()
	{
		var c;
		let c = 1.0 | true;
		return c;
	}

	public function complex5Or()
	{
		var c;
		let c = 1 | true;
		return c;
	}

	public function complex6Or()
	{
		var c;
		let c = true | 1;
		return c;
	}

	public function complex7Or()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = a | b;
		return c;
	}

	public function complex9Or()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2,
			c = b | a;
		return c;
	}

	public function complex10Or()
	{
		boolean a; int b; var c;

		let a = 1.0,
			b = 2,
			c = b | a;
		return c;
	}

	public function complex11Or()
	{
		boolean a; int b; var c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex12Or()
	{
		boolean a; int b; int c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex13Or()
	{
		boolean a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex14Or()
	{
		var a; boolean b; int c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex15Or()
	{
		var a; var b; boolean c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex16Or()
	{
		var a; var b; double c;

		let a = 1,
			b = 2.0,
			c = b | a;
		return c;
	}

	public function complex17Or()
	{
		var a; boolean b; double c;

		let a = 1,
			b = 2.0,
			c = a | b;
		return c;
	}

	public function complex18Or()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a | b,
			c = a | b | d;
		return c;
	}

	public function complex19Or()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a | b,
			c = a | d | b;
		return c;
	}

	public function complex20Or()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a | b,
			c = d | d | d | d;
		return c;
	}

	public function complex21Or()
	{
		var a, d; boolean b; double c;

		let a = 1,
			b = 2.0,
			d = a | b,
			c = b | d | b | d | d | b | d | b;
		return c;
	}

	public function complex22Or()
	{
		var a, d; int b; double c;

		let a = 1,
			b = 2.0,
			d = a | b,
			c = b | d | d | d | d | b | d | b;
		return c;
	}

	public function complex23Or()
	{
		var a; boolean b; double d;
		let a = 1,
			b = 2.0,
			d = a | b;
		return d;
	}

	public function complex24Or()
	{
		var a; var d; boolean b;
		let a = 1,
			b = 2.0,
			d = 1 | a | false | b;
		return d;
	}

		/* ------- shift left -------- */
	public function intShiftLeft()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a << b;

		return c;
	}

	public function int2ShiftLeft()
	{
		int a, c;

		let a = 1,
			c = a << 2;

		return c;
	}

	public function intShiftLeftSimple()
	{
		int c;

		let c = 1 << 2;
		return c;
	}

	/* ------- shift right -------- */
	public function intShiftRight()
	{
		int a, b, c;

		let a = 1,
			b = 2,
			c = a >> b;

		return c;
	}

	public function int2ShiftRight()
	{
		int a, c;

		let a = 1,
			c = a >> 2;

		return c;
	}

	public function intShiftRightSimple()
	{
		int c;

		let c = 1 >> 2;
		return c;
	}
}
