
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

	}

	public function testConcat10()
	{
		var url;
		let url = "test";
		let url = "append" . url;
		return url;
	}
}