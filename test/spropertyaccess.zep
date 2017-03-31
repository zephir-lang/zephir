/**
 * Class with constructor + params
 */

namespace Test;

class SPropertyAccess
{
	protected static a;
	private static b;
	protected static delimiter = ".";
	protected static _delimiterWithUnderscore = ".";

	public function __construct()
	{
		let self::a = "abc";
		let self::b = substr(self::a, 0, 1);
		let ScallExternal::sproperty = self::b;
		let self::b = ScallExternal::sproperty;
	}

	public function testArgumentWithUnderscore(var delimiter = null)
	{
		if empty delimiter {
			let delimiter = self::_delimiterWithUnderscore;
		}

		return delimiter;
	}

	public function testArgument(var delimiter = null)
    {
    	if empty delimiter {
    		let delimiter = self::delimiter;
    	}

    	return delimiter;
    }
}
