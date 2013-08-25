
/**
 * Static Function calls
 */

namespace Test;

class Scall
{

	static + public function testMethod1()
	{
		return "hello public";
	}

	static + protected function testMethod2()
	{
		return "hello protected";
	}

	public function testCall1()
	{
		return Test\Scall::testMethod1();
	}

	public function testCall2()
	{
		return Test\Scall::testMethod2();
	}

}