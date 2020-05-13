
/**
 * Static Function calls
 */

namespace Stub;

class ScallParent
{
	static public function testMethod1() -> string
	{
		return "hello parent public";
	}

	static protected function testMethod2() -> string
	{
		return "hello parent protected";
	}

	static public function testCallStatic() -> string
	{
		return static::testMethodStatic();
	}

	static protected function testMethodStatic() -> string
	{
		return "hello ScallParent";
	}
}
