/**
 * Static Function calls
 */

namespace Stub;

class ScallDynamic extends ScallParent
{

	static public function testMethod1() -> string
	{
		return "hello public";
	}

	static protected function testMethod2() -> string
	{
		return "hello protected";
	}

	static private function testMethod3() -> string
	{
		return "hello private";
	}

	static public function selfDynamicCall1(var methodName) -> string
	{
		return self::{methodName}();
	}
}
