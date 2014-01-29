
/**
 * Function calls
 */

namespace Test;

class Fcall
{
	public function testCall1()
	{
		return strpos("hello", "h");
	}

	public function testCall2()
	{
		loop {
			return mt_rand(0, 100);
		}
	}

	public function testCall1FromVar()
	{
		var funcName;
		let funcName = "strpos";
		return {funcName}("hello", "l");
	}

	public function testStrtokFalse()
	{
		return strtok("/");
	}

	public function testStrtokVarBySlash(var value)
	{
		return strtok(value, "/");
	}

	public function testFunctionGetArgs(var param1, var param2)
	{
		return func_get_args();
	}
}
