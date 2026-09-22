
namespace Stub;

class Concat
{
	protected static testProperty = null;

	protected static appended = "";

	protected store = "";

	static public function getTestProperty() -> string | null
	{
		return self::testProperty;
	}

	static public function testConcatBySelfProperty(string title) -> void
	{
		let self::testProperty .= title;
	}

	public function testConcat1() -> string
	{
		var url;
		let url = "test";
		let url = "append" . url;
		return url;
	}

	public function testConcat2() -> string
	{
		var url;
		let url = "test";
		let url = "append" . url . "other";
		let url = "append" . "other";
		let url = "append" . "other" . url;
		return url;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1573
	 */
	public function testConcat3() -> string
	{
		var a, b;
		let a = "1";
		let b = "2";
		let b .= a;
		return b;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1893
	 */
	public function testConcat4(var value) -> string
	{
		var min, max;
		string query = "";

		let min = value / 100 * 25,
			max = value / 100 * 50;

		let query .= "SELECT * FROM TEST WHERE value <= " . max,
			query .= " AND value >= " . min;

		return query;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1893
	 */
	public function testConcat5(double number) -> string
	{
		string retval;
		string left = "Concatenated string with number ";

		let retval = left . number;

		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatStringWithIntVar(int number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	/**
	 * `long` is a `zend_long` on every platform since #2666, so this carries
	 * the full 64-bit range exactly like the `int` variant above. It used to
	 * be a C `long`, 32-bit under Windows LLP64, and this method held a
	 * 32-bit-safe local to keep CI green.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 * @link https://github.com/zephir-lang/zephir/issues/2666
	 */
	public function testConcatStringWithLongVar(long number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatStringWithUintVar() -> string
	{
		string retval = "n=";
		uint number = 5;
		let retval .= number;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatStringWithUlongVar() -> string
	{
		string retval = "n=";
		ulong number = 5;
		let retval .= number;
		return retval;
	}

	/**
	 * Fence for the sibling helper: a char operand routes to
	 * zephir_concat_self_char(), which is defined.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatStringWithCharVar() -> string
	{
		string retval = "n=";
		char c = 'A';
		let retval .= c;
		return retval;
	}

	/**
	 * A `var` left-hand side takes a different code path than a `string` one.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatVarWithIntVar(int number) -> string
	{
		var retval = "n=";
		let retval .= number;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatVarWithCharVar() -> string
	{
		var retval = "n=";
		char c = 'A';
		let retval .= c;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2660
	 */
	public function testConcatVarWithIntLiteral() -> string
	{
		var retval = "n=";
		let retval .= 5;
		return retval;
	}

	/**
	 * A double must come out exactly as PHP renders it. PHP converts a float
	 * with `zend_double_to_str()`, which reads `EG(precision)` at run time, so
	 * the operand is boxed and converted by the engine rather than formatted
	 * by the compiler.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithDoubleVar(double number) -> string
	{
		string retval = "n=";
		let retval .= number;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithDoubleLiteral() -> string
	{
		string retval = "n=";
		let retval .= 1.5;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithBoolVar(bool flag) -> string
	{
		string retval = "n=";
		let retval .= flag;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithTrueLiteral() -> string
	{
		string retval = "n=";
		let retval .= true;
		return retval;
	}

	/**
	 * `false` appends nothing, which is not the same as doing nothing.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithFalseLiteral() -> string
	{
		string retval = "n=";
		let retval .= false;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithDoubleVar(double number) -> string
	{
		var retval = "n=";
		let retval .= number;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithDoubleLiteral() -> string
	{
		var retval = "n=";
		let retval .= 1.5;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithBoolVar(bool flag) -> string
	{
		var retval = "n=";
		let retval .= flag;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithTrueLiteral() -> string
	{
		var retval = "n=";
		let retval .= true;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithFalseLiteral() -> string
	{
		var retval = "n=";
		let retval .= false;
		return retval;
	}

	/**
	 * The left operand is an integer, so `.= false` appends nothing yet still
	 * converts it: PHP leaves the *string* "5" here.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatIntVarWithFalseLiteral() -> string
	{
		var retval = 5;
		let retval .= false;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithNullLiteral() -> string
	{
		var retval = 5;
		let retval .= null;
		return retval;
	}

	/**
	 * PHP raises "Array to string conversion" and appends "Array". A local
	 * left-hand side used to reject the operand outright while an object
	 * property accepted it, so the two disagreed.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithArrayLiteral() -> string
	{
		string retval = "n=";
		let retval .= [1, 2];
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatStringWithArrayVar() -> string
	{
		string retval = "n=";
		array a = [1, 2];
		let retval .= a;
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithArrayLiteral() -> string
	{
		var retval = "n=";
		let retval .= [1, 2];
		return retval;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatVarWithArrayVar() -> string
	{
		var retval = "n=";
		array a = [1, 2];
		let retval .= a;
		return retval;
	}

	public function getStore() -> string
	{
		return this->store;
	}

	public function resetStore() -> void
	{
		let this->store = "";
	}

	/**
	 * `.=` on a property is a read-modify-write. Every operand type here used
	 * to overwrite the property instead of appending to it.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function appendPropertyLiterals() -> void
	{
		let this->store .= "s";
		let this->store .= 5;
		let this->store .= 1.5;
		let this->store .= true;
		let this->store .= false;
		let this->store .= null;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function appendPropertyVariables(int number, double fraction, bool flag, string text) -> void
	{
		char c = 'A';

		let this->store .= number;
		let this->store .= fraction;
		let this->store .= flag;
		let this->store .= c;
		let this->store .= text;
	}

	/**
	 * PHP raises "Array to string conversion" and appends "Array".
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function appendPropertyArray() -> void
	{
		let this->store .= [1, 2];
	}

	static public function getAppended() -> string
	{
		return self::appended;
	}

	static public function resetAppended() -> void
	{
		let self::appended = "";
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	static public function appendStaticLiterals() -> void
	{
		let self::appended .= "s";
		let self::appended .= 5;
		let self::appended .= 1.5;
		let self::appended .= true;
		let self::appended .= false;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	static public function appendStaticVariables(int number, double fraction, bool flag) -> void
	{
		let self::appended .= number;
		let self::appended .= fraction;
		let self::appended .= flag;
	}

	/**
	 * The binary `.` operator rejected a bool operand, and a char operand
	 * appended nothing usable, although `.=` accepted both.
	 *
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatExprWithBoolVar(bool flag) -> string
	{
		return "n=" . flag;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatExprWithTrueLiteral() -> string
	{
		return "n=" . true;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatExprWithFalseLiteral() -> string
	{
		return "n=" . false;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatExprWithCharVar() -> string
	{
		char c = 'A';
		return "n=" . c;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/2664
	 */
	public function testConcatExprWithCharLiteral() -> string
	{
		return "n=" . 'A';
	}
}
