/**
 * Call external static functions
 */

namespace Test;

class ScallExternal
{
	public static sproperty;

	public function testCall1()
	{
		return Scall::testMethod1();
	}

	public function testCall2(var a, var b)
	{
		return Scall::testMethod4(a, b);
	}

	public static function testMethod3(long a, long b)
	{
		return a + b;
	}
}
