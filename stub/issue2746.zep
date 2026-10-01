namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2746
 *
 * Converting a zval to an integer or a float must give PHP's `(int)` and
 * `(float)` result: an object runs its own cast handler or warns, a float
 * string beyond the int range saturates, and a resource reads as its
 * handle. Every conversion path the compiler emits is here, and the test compares
 * each against plain PHP.
 */
class Issue2746
{
	public function toLong(var a) -> long
	{
		long k = a;

		return k;
	}

	public function castInt(var a) -> int
	{
		return (int) a;
	}

	public function intvalOf(var a) -> int
	{
		return intval(a);
	}

	public function toDouble(var a) -> double
	{
		double d = a;

		return d;
	}

	public function castDouble(var a) -> double
	{
		return (double) a;
	}

	public function doublevalOf(var a) -> double
	{
		return doubleval(a);
	}

	public function doubleParam(double a) -> double
	{
		return a;
	}

	public function optionalDoubleParam(double a = 1.5) -> double
	{
		return a;
	}

	public function nullableDoubleParam(double a = null) -> double
	{
		return a;
	}
}
