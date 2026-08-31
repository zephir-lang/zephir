namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2666
 *
 * Zephir's `long`/`ulong` mapped to a C `long`/`unsigned long`, which is
 * 32-bit under LLP64 (Windows x64) and 64-bit under LP64 (Linux, macOS),
 * while a PHP integer is always a 64-bit `zend_long`. Every method here moves
 * a value that only fits in 64 bits across that boundary, and the test asserts
 * the result against the same expression evaluated in plain PHP.
 *
 * The division and modulo methods cover the second half of the fix: the
 * `zephir_safe_mod_*` family was declared and emitted but mostly undefined,
 * and both families answered a zero divisor with a warning instead of PHP 8's
 * DivisionByZeroError.
 */
class Issue2666
{
	/**
	 * The reproduction from the issue: `Z_PARAM_LONG(number)` stores a
	 * `zend_long` through `&number`.
	 */
	public function longParamToString(long number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	public function ulongParamToString(ulong number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	public function intParamToString(int number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	public function longRoundTrip(long number) -> long
	{
		return number;
	}

	public function ulongRoundTrip(ulong number) -> ulong
	{
		return number;
	}

	/**
	 * Parameter -> local -> return, so the value passes through a declared
	 * local rather than staying in the parameter slot.
	 */
	public function longThroughLocal(long number) -> long
	{
		long copy;
		let copy = number;
		return copy;
	}

	/**
	 * `zephir_get_intval()`, the funnel every cast shares. Its `_ex` helper
	 * returned a C `long`, so this truncated even for Zephir `int`.
	 */
	public function castToInt(var value) -> int
	{
		int result;
		let result = (int) value;
		return result;
	}

	public function castToLong(var value) -> long
	{
		long result;
		let result = (long) value;
		return result;
	}

	/**
	 * Arithmetic on two 64-bit locals: no kernel helper, but the operands and
	 * the result must all be 64-bit wide.
	 */
	public function longAdd(long a, long b) -> long
	{
		return a + b;
	}

	/** Array key built from a `long`: `zephir_array_update_long()`. */
	public function longArrayKey(long key, var value) -> array
	{
		array result = [];
		let result[key] = value;
		return result;
	}

	/** `zephir_array_fetch_long()` with a 64-bit key. */
	public function longArrayFetch(array! source, long key)
	{
		return source[key];
	}

	/** `zephir_array_isset_long()`. */
	public function longArrayIsset(array! source, long key) -> bool
	{
		return isset source[key];
	}

	/** `zephir_less_long()` / `zephir_greater_long()`: zval against a native. */
	public function varLessThanLong(var value, long limit) -> bool
	{
		return value < limit;
	}

	public function varGreaterThanLong(var value, long limit) -> bool
	{
		return value > limit;
	}

	/** `zephir_substr()` took `long from, long length`. */
	public function substrWithLongOffset(string! source, long from, long length) -> string
	{
		return substr(source, from, length);
	}

	/** `zephir_fast_explode()` took a `long limit`. */
	public function explodeWithLongLimit(string! source, long limit) -> array
	{
		return explode(",", source, limit);
	}

	/**
	 * The eight `zephir_safe_div_*` / `zephir_safe_mod_*` shapes the compiler
	 * can actually reach. A double *literal* on the right is the only way to
	 * reach the `*_double` variants: a `double` variable there is rejected by
	 * ModOperator/DivOperator.
	 */
	public function divLongLong(long a, long b)
	{
		return a / b;
	}

	public function divLongDouble(long a)
	{
		return a / 2.5;
	}

	public function divDoubleLong(double a, long b)
	{
		return a / b;
	}

	public function divDoubleDouble(double a)
	{
		return a / 2.5;
	}

	public function divVarLong(var a, long b)
	{
		return a / b;
	}

	public function divVarDouble(var a)
	{
		return a / 2.5;
	}

	public function divLongVar(long a, var b)
	{
		return a / b;
	}

	public function divDoubleVar(double a, var b)
	{
		return a / b;
	}

	public function modLongLong(long a, long b)
	{
		return a % b;
	}

	public function modLongDouble(long a)
	{
		return a % 2.5;
	}

	public function modDoubleLong(double a, long b)
	{
		return a % b;
	}

	public function modDoubleDouble(double a)
	{
		return a % 2.5;
	}

	public function modVarLong(var a, long b)
	{
		return a % b;
	}

	public function modVarDouble(var a)
	{
		return a % 2.5;
	}

	public function modLongVar(long a, var b)
	{
		return a % b;
	}

	public function modDoubleVar(double a, var b)
	{
		return a % b;
	}

	/** Literal double on the left: the only shape reaching `*_double_double`. */
	public function divLiteralDoubleByDouble(double b)
	{
		return 2.5 / b;
	}

	public function modLiteralDoubleByDouble(double b)
	{
		return 2.5 % b;
	}

	/**
	 * Multi-dimensional keys reach `zephir_array_update_multi()` through a
	 * variadic slot read back as `zend_long`. An integer literal is a C `int`,
	 * so the upper half of the slot was whatever the ABI left there: on Windows
	 * `a[0][1]` produced the key 140733193388033. `uint`/`ulong` were worse,
	 * passing `&i` and using the pointer as the key.
	 */
	public function multiLiteralKeys() -> array
	{
		array result = [];
		let result[0][1] = "v";
		return result;
	}

	public function multiIntKey(int key) -> array
	{
		array result = [];
		let result[key][2] = "v";
		return result;
	}

	public function multiUintKey(uint key) -> array
	{
		array result = [];
		let result[key][2] = "v";
		return result;
	}

	public function multiLongKey(long key) -> array
	{
		array result = [];
		let result[key][2] = "v";
		return result;
	}

	public function multiUlongKey(ulong key) -> array
	{
		array result = [];
		let result[key][2] = "v";
		return result;
	}

	/** Enough offsets to spill past the register arguments. */
	public function multiDeepKeys() -> array
	{
		array result = [];
		let result[0][1][2][3][4][5][6][7][8] = "v";
		return result;
	}

	/** Mixed string and integer offsets in one call. */
	public function multiMixedKeys(long key) -> array
	{
		array result = [];
		let result["k"][key] = "v";
		return result;
	}
}
