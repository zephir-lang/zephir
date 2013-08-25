
/**
 * Function calls
 */

namespace Test;

class Scall
{
	static function testMethod1()
	{
		return "hello";
	}

	public function testCall1()
	{
		return Test\Scall::testMethod1();
	}

}