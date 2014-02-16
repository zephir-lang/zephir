/**
 * Class with constructor + params
 */

namespace Test;

class SPropertyAccess
{
	protected static a;
	private static b;

	public function __construct()
	{
		let self::a = "abc";
		let self::b = self::a[0];
		let ScallExternal::sproperty = self::b;
		let self::b = ScallExternal::sproperty;
	}
}