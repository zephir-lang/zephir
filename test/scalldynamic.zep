/**
 * Static Function calls
 */

namespace Test;

class ScallDynamic extends ScallParent
{

	static public function testMethod1()
	{
		return "hello public";
	}

	static protected function testMethod2()
	{
		return "hello protected";
	}

	static private function testMethod3()
	{
		return "hello private";
	}

	static public function selfDynamicCall1(var methodName)
	{
		//return self::{methodName}();
	}
}
