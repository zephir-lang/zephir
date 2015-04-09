
namespace Test;

class Concat
{
	protected static testProperty = null;

	static public function getTestProperty()
	{
		return self::testProperty;
	}

	static public function testConcatBySelfProperty(string title)
	{
		let self::testProperty .= title;
	}

	public function testConcat1()
	{
		var url;
		let url = "test";
		let url = "append" . url;
		return url;
	}

	public function testConcat2()
	{
		var url;
		let url = "test";
		let url = "append" . url . "other";
		let url = "append" . "other";
		let url = "append" . "other" . url;
		return url;
	}

	public function testConcatSelf1()
	{
		var url = "";
		let url .= "test";
		let url .= "append" . url . "other";
		let url .= "append" . "other";
		let url .= "append" . "other" . url;
		return url;
	}
}
