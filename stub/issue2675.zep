namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2675
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * PHP's `/` returns an `int` when both operands are integers and the division
 * is exact, and a `float` otherwise. Every shape DivOperator can emit is here,
 * plus the `/=` forms, and the test compares each against plain PHP.
 */
class Issue2675
{
	public function divLongLong(long a, long b)
	{
		return a / b;
	}

	public function divLongVar(long a, var b)
	{
		return a / b;
	}

	public function divVarLong(var a, long b)
	{
		return a / b;
	}

	public function divVarVar(var a, var b)
	{
		return a / b;
	}

	public function divLongDouble(long a, double b)
	{
		return a / b;
	}

	public function divDoubleLong(double a, long b)
	{
		return a / b;
	}

	public function divDoubleDouble(double a, double b)
	{
		return a / b;
	}

	public function divVarDouble(var a, double b)
	{
		return a / b;
	}

	public function divDoubleVar(double a, var b)
	{
		return a / b;
	}

	public function divVarLiteralDouble(var a)
	{
		return a / 2.0;
	}

	public function divLiteralExact()
	{
		return 4 / 2;
	}

	public function divLiteralInexact()
	{
		return 7 / 2;
	}

	public function divLongBool(long a, bool b)
	{
		return a / b;
	}

	public function divBoolLong(bool a, long b)
	{
		return a / b;
	}

	public function divVarBool(var a, bool b)
	{
		return a / b;
	}

	public function divBoolVar(bool a, var b)
	{
		return a / b;
	}

	public function divDoubleBool(double a, bool b)
	{
		return a / b;
	}

	public function divBoolDouble(bool a, double b)
	{
		return a / b;
	}

	public function divBoolBool(bool a, bool b)
	{
		return a / b;
	}

	public function divLongByTrue(long a)
	{
		return a / true;
	}

	public function divTrueByLong(long b)
	{
		return true / b;
	}

	public function divInferredLocal(long a, long b)
	{
		var x = a / b;

		return x;
	}

	public function divReturnDouble(long a, long b) -> double
	{
		return a / b;
	}

	public function divTypedDouble(long a, long b)
	{
		double d = a / b;

		return d;
	}

	public function divTypedLong(long a, long b)
	{
		long k = a / b;

		return k;
	}

	public function divChained(long a, long b)
	{
		return (a / b) * 2;
	}

	public function divAssignVarLong(var a, long b)
	{
		var x;

		let x = a;
		let x /= b;

		return x;
	}

	public function divAssignVarVar(var a, var b)
	{
		var x;

		let x = a;
		let x /= b;

		return x;
	}

	public function divAssignInferredLocal(long b)
	{
		var x = 42;

		let x /= b;

		return x;
	}

	public function divAssignProperty(var obj, long b)
	{
		let obj->value /= b;

		return obj->value;
	}

	public function divAssignPropertyVar(var obj, var b)
	{
		let obj->value /= b;

		return obj->value;
	}

	public function divAssignPropertyLiteral(var obj)
	{
		let obj->value /= 2;

		return obj->value;
	}
}
