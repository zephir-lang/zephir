
/**
 * Call external static functions
 */

namespace Test;

class ScallExternal
{

	public function testCall1(var a, var b)
	{
		return Test\Scall::testMethod4(a, b);
	}

	public function testCall2(var a, var b)
	{
		return Test\Scall::testMethod5(a, b);
	}

	public function testCall3(var a, var b)
	{
		return Test\Scall::testMethod6(a, b);
	}

}
