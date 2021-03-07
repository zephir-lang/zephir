/**
 * Class with constructor + params
 */

namespace Stub;

class SPropertyAccess
{
	protected static a;
	private static b;
	protected static delimiter = ".";
	protected static _delimiterWithUnderscore = ".";
	protected static stringVar = "";
	protected static intVar = 0;
	protected static doubleVar = 0.0;
	protected static arrayVar = [];

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

	public static function mutateStringVarInsideCycle() -> string
	{
		var i;

		for _ in range(0, 3) {
			let i = " + ";
			let self::stringVar = self::stringVar . i;
		}

		return self::stringVar;
	}

	/**
	 * @see https://github.com/zephir-lang/zephir/issues/1494
	 */
	public static function mutateIntVarInsideCycle() -> int
	{
		var i;

		for _ in range(0, 3) {
			let i = 42;
			let self::intVar = i;
		}

		return self::intVar;
	}

	/**
	 * @see https://github.com/zephir-lang/zephir/issues/1494
	 */
	public static function mutateDoubleVarInsideCycle() -> double
	{
		var i;

		for _ in range(0, 3) {
			let i = 3.14;
			let self::doubleVar = i;
		}

		return self::doubleVar;
	}

	public static function mutateArrayVarInsideCycle() -> array
	{
		var i, j;

		for i in range(0, 3) {
			let j = [i: i];
			let self::arrayVar = self::arrayVar + j;
		}

		return self::arrayVar;
	}
}
