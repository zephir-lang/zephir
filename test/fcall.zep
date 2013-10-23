
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

	public function testCall1FromVar()
	{
		var funcName;
		let funcName = "strpos";
		return {funcName}("hello", "l");
	}

}
