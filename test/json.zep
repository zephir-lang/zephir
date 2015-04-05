
namespace Test;

class Json
{
	public function testEncodeObject()
	{
		var obj;
		let obj = ["a": "hello", "b": "world", "c": 128];
		return json_encode(obj);
	}

	public function testEncodeArray()
	{
		var arr;
		let arr = [1, 2, 3];
		return json_encode(arr);
	}

	public function testEncodeOptions()
	{
		var arr;
		let arr = ["<foo>", "'bar'", "&blong&", "\xc3\xa9"];
		return json_encode(arr, JSON_HEX_TAG);
	}

	public function testDecodeObject()
	{
		var obj = "{\"a\":\"hello\",\"b\":\"world\",\"c\":128}";
		return json_decode(obj);
	}

	public function testDecodeObject2()
	{
		var obj = "{\"a\":\"hello\",\"b\":\"world\",\"c\":128}";
		return json_decode(obj, true);
	}

	public function testDecodeArray()
	{
		var arr;
		let arr = "[1,2,3]";
		return json_decode(arr);
	}
}
