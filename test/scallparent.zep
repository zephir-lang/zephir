
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

    static protected function staticMethod()
    {
        return "ScallParent";
    }

    static protected function testStaticCall()
    {
        return static::staticMethod();
    }
}
