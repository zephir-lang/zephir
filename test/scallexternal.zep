
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

	public function testCall2()
	{
		return Test\Scall::testMethod2();
	}

	public function testCall3()
	{
		return Test\Scall::testMethod3();
	}

	public function testCall4(var a, var b)
	{
		return Test\Scall::testMethod4(a, b);
	}

	public function testCall5(var a, var b)
	{
		return Test\Scall::testMethod5(a, b);
	}

	public function testCall6(var a, var b)
	{
		return Test\Scall::testMethod6(a, b);
	}

	public function testCall7()
	{
		return self::testMethod1();
	}

	public function testCall8()
	{
		return self::testMethod2();
	}

	public function testCall9()
	{
		return self::testMethod3();
	}

	public function testCall10(var a, var b)
	{
		return self::testMethod4(a, b);
	}

	public function testCall11(var a, var b)
	{
		return self::testMethod5(a, b);
	}

	public function testCall12(var a, var b)
	{
		return self::testMethod6(a, b);
	}

}
