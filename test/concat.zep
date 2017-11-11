
namespace Test;

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
	 * @link https://github.com/phalcon/zephir/issues/1573
	 */
	public function testConcat3() -> string
	{
		var a, b;
		let a = "1";
		let b = "2";
		let b .= a;
		return b;
	}
}
