
/**
 * Class with calling proteced method
 */

namespace Test\Oo;

class OoOther
{
	public function callProtected(var a)
	{
		return a->testMethod2();
	}
}
