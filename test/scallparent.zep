
/**
 * Static Function calls
 */

namespace Test;

class ScallParent
{
	static public function testMethod1()
	{
		return "hello parent public";
	}

	static protected function testMethod2()
	{
		return "hello parent protected";
	}

	static public function testCallStatic()
	{
		return static::testMethodStatic();
	}

	static protected function testMethodStatic()
	{
		return "hello ScallParent";
	}
}
