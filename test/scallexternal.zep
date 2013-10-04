
/**
 * Call external static functions
 */

namespace Test;

class ScallExternal
{

	public function testCall1()
	{
		return Test\Scall::testMethod1();
	}

	public function testCall2(var a, var b)
	{
		return Test\Scall::testMethod4(a, b);
	}

}
