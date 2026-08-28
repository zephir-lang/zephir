
namespace Stub;

class Concat
{
	protected static testProperty = null;

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
	 * @link https://github.com/zephir-lang/zephir/issues/2660
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
}
