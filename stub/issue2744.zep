namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2744
 *
 * A zval combined with a native number by `+`, `-` or `*` must give PHP's
 * result: a float stays a float, an int overflows to float, and an operand
 * PHP cannot treat as a number throws TypeError. Every shape
 * ArithmeticalBaseOperator can emit is here, and the test compares each
 * against plain PHP.
 */
class Issue2744
{
	public function addVarLiteral(var a)
	{
		return a + 3;
	}

	public function subVarLiteral(var a)
	{
		return a - 3;
	}

	public function mulVarLiteral(var a)
	{
		return a * 3;
	}

	public function addLiteralVar(var a)
	{
		return 3 + a;
	}

	public function subLiteralVar(var a)
	{
		return 3 - a;
	}

	public function mulLiteralVar(var a)
	{
		return 3 * a;
	}

	public function addVarDoubleLiteral(var a)
	{
		return a + 1.5;
	}

	public function subVarDoubleLiteral(var a)
	{
		return a - 1.5;
	}

	public function mulVarDoubleLiteral(var a)
	{
		return a * 1.5;
	}

	public function addDoubleLiteralVar(var a)
	{
		return 1.5 + a;
	}

	public function subDoubleLiteralVar(var a)
	{
		return 1.5 - a;
	}

	public function mulDoubleLiteralVar(var a)
	{
		return 1.5 * a;
	}

	public function addLongVar(long b, var a)
	{
		return b + a;
	}

	public function subLongVar(long b, var a)
	{
		return b - a;
	}

	public function mulLongVar(long b, var a)
	{
		return b * a;
	}

	public function addVarLong(var a, long b)
	{
		return a + b;
	}

	public function subVarLong(var a, long b)
	{
		return a - b;
	}

	public function mulVarLong(var a, long b)
	{
		return a * b;
	}

	public function addDoubleVar(double d, var a)
	{
		return d + a;
	}

	public function subDoubleVar(double d, var a)
	{
		return d - a;
	}

	public function mulDoubleVar(double d, var a)
	{
		return d * a;
	}

	public function addVarDouble(var a, double d)
	{
		return a + d;
	}

	public function subVarDouble(var a, double d)
	{
		return a - d;
	}

	public function mulVarDouble(var a, double d)
	{
		return a * d;
	}

	public function addBoolVar(bool b, var a)
	{
		return b + a;
	}

	public function subBoolVar(bool b, var a)
	{
		return b - a;
	}

	public function mulBoolVar(bool b, var a)
	{
		return b * a;
	}

	public function addVarBool(var a, bool b)
	{
		return a + b;
	}

	public function subVarBool(var a, bool b)
	{
		return a - b;
	}

	public function mulVarBool(var a, bool b)
	{
		return a * b;
	}

	public function addCharVar(var a)
	{
		char c = 'a';

		return c + a;
	}

	public function subCharVar(var a)
	{
		char c = 'a';

		return c - a;
	}

	public function mulCharVar(var a)
	{
		char c = 'a';

		return c * a;
	}

	public function addArrayLiteral(array a)
	{
		return a + 3;
	}

	public function subArrayLiteral(array a)
	{
		return a - 3;
	}

	public function mulArrayLiteral(array a)
	{
		return a * 3;
	}

	public function assignedToVar(var a)
	{
		var r;

		let r = a + 1;

		return r;
	}

	public function assignedToLong(var a) -> long
	{
		long r;

		let r = a + 1;

		return r;
	}

	public function assignedToDouble(var a) -> double
	{
		double r;

		let r = a + 1;

		return r;
	}

	public function loopCarried() -> array
	{
		var a, results = [];
		int i;

		let a = 1;
		for i in range(1, 2) {
			let results[] = a * 2;
			let a = 0.5;
		}

		return results;
	}

	public function addAssign(var a)
	{
		let a += 1;

		return a;
	}

	public function subAssign(var a)
	{
		let a -= 1;

		return a;
	}

	public function mulAssign(var a)
	{
		let a *= 2;

		return a;
	}

	public function increment(var a)
	{
		let a++;

		return a;
	}

	public function decrement(var a)
	{
		let a--;

		return a;
	}
}
