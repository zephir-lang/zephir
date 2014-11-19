/**
 * Static Function calls
 */

namespace Test;

class Scall extends ScallParent
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

	static public function testMethod4(var a, var b)
	{
		return a + b;
	}

	static protected function testMethod5(var a, var b)
	{
		return a + b;
	}

	static private function testMethod6(var a, var b)
	{
		return a + b;
	}

	static public function testMethod7() -> <\stdClass>
	{
		return new \stdClass();
	}

	public function testCall1()
	{
		return Scall::testMethod1();
	}

	public function testCall2()
	{
		return Scall::testMethod2();
	}

	public function testCall3()
	{
		return Scall::testMethod3();
	}

	public function testCall4(var a, var b)
	{
		return Scall::testMethod4(a, b);
	}

	public function testCall5(var a, var b)
	{
		return Scall::testMethod5(a, b);
	}

	public function testCall6(var a, var b)
	{
		return Scall::testMethod6(a, b);
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

	public function testCall13()
	{
		return parent::testMethod1();
	}

	public function testCall14()
	{
		return parent::testMethod2();
	}

	public function testCall15()
	{
		return self::testMethod7();
	}

	public static function testMethod16(long a, long b)
	{
		return a + b;
	}

	public static function testCall17(long k, var p)
	{
		long i, j = 0;
		for i in range(1, k) {
			let j += \Test\ScallExternal::testMethod3(p, p);
		}
		return j;
	}

	public static function testCall18(long k, var p)
	{
		long i, j = 0;
		for i in range(1, k) {
			let j += self::testMethod16(p, p);
		}
		return j;
	}

	static protected function testMethodStatic()
	{
		return "hello Scall";
	}
}
