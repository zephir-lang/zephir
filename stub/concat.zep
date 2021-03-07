
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
}
