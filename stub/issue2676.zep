namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * PHP's `%` converts both operands to int and yields an int, and throws
 * TypeError for an operand it cannot treat as a number. Every shape
 * ModOperator can emit is here, plus the `%=` forms, and the test compares
 * each against plain PHP. The typed-local `/=` forms share the `%=` rewrite.
 */
class Issue2676
{
	public function modLongLong(long a, long b)
	{
		return a % b;
	}

	public function modLongVar(long a, var b)
	{
		return a % b;
	}

	public function modVarLong(var a, long b)
	{
		return a % b;
	}

	public function modVarVar(var a, var b)
	{
		return a % b;
	}

	public function modLongDouble(long a, double b)
	{
		return a % b;
	}

	public function modDoubleLong(double a, long b)
	{
		return a % b;
	}

	public function modDoubleDouble(double a, double b)
	{
		return a % b;
	}

	public function modVarDouble(var a, double b)
	{
		return a % b;
	}

	public function modDoubleVar(double a, var b)
	{
		return a % b;
	}

	public function modVarLiteralDouble(var a)
	{
		return a % 2.5;
	}

	public function modVarLiteralLong(var a)
	{
		return a % 4;
	}

	public function modLiteral()
	{
		return 7 % 3;
	}

	public function modLongBool(long a, bool b)
	{
		return a % b;
	}

	public function modBoolLong(bool a, long b)
	{
		return a % b;
	}

	public function modVarBool(var a, bool b)
	{
		return a % b;
	}

	public function modBoolVar(bool a, var b)
	{
		return a % b;
	}

	public function modDoubleBool(double a, bool b)
	{
		return a % b;
	}

	public function modBoolDouble(bool a, double b)
	{
		return a % b;
	}

	public function modBoolBool(bool a, bool b)
	{
		return a % b;
	}

	public function modLongByTrue(long a)
	{
		return a % true;
	}

	public function modTrueByLong(long b)
	{
		return true % b;
	}

	public function modVarByTrue(var a)
	{
		return a % true;
	}

	public function modInferredLocal(var a, long b)
	{
		var x = a % b;

		return x;
	}

	public function modTypedLong(var a, long b)
	{
		long k = a % b;

		return k;
	}

	public function modAssignVarLong(var a, long b)
	{
		var x;

		let x = a;
		let x %= b;

		return x;
	}

	public function modAssignVarVar(var a, var b)
	{
		var x;

		let x = a;
		let x %= b;

		return x;
	}

	public function modAssignInferredLocal(long b)
	{
		var x = 42;

		let x %= b;

		return x;
	}

	public function modAssignTypedLong(long a, long b)
	{
		long x;

		let x = a;
		let x %= b;

		return x;
	}

	public function modAssignTypedLongVar(long a, var b)
	{
		long x;

		let x = a;
		let x %= b;

		return x;
	}

	public function modAssignTypedDouble(double a, long b)
	{
		double x;

		let x = a;
		let x %= b;

		return x;
	}

	public function modAssignProperty(var obj, long b)
	{
		let obj->value %= b;

		return obj->value;
	}

	public function modAssignPropertyVar(var obj, var b)
	{
		let obj->value %= b;

		return obj->value;
	}

	public function modAssignPropertyLiteral(var obj)
	{
		let obj->value %= 4;

		return obj->value;
	}

	public function divAssignTypedLong(long a, long b)
	{
		long x;

		let x = a;
		let x /= b;

		return x;
	}

	public function divAssignTypedDouble(double a, long b)
	{
		double x;

		let x = a;
		let x /= b;

		return x;
	}
}
