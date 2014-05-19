
/**
 * X
 */

namespace Test;

class Returns
{
	public function testReturnCast1() -> int
	{
		return (int) 5.0;
	}

	public function testReturnCast2() -> int
	{
		return (int) true;
	}

	public function testReturnCast3() -> int
	{
		return (int) false;
	}

	public function testReturnCast4() -> int
	{
		if true {
			return (int)1;
		} else {
			return (int)0;
		}
	}
}
