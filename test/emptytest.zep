
/**
 * Control Flow
 */

namespace Test;

/**
 * @link http://ru2.php.net/empty
 */
class EmptyTest
{
	public function testDynamicVarArrayEmpty()
	{
		var a;
		let a = [];

		return empty(a);
	}

	public function testDynamicVarArrayNotEmpty()
	{
		var a;
		let a = [1, 2, 3, 4];

		return empty(a);
	}

	public function testEmptyString()
	{
		var a;
		let a = "";

		return empty(a);
	}

	public function testNotEmptyString()
	{
		var a;
		let a = "test string";

		return empty(a);
	}

	public function testString(string a)
	{
		return empty(a);
	}
}
