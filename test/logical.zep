
/**
 * Arithmetic operations
 */

namespace Test;

class Logical
{
    /* ---------- And ----------------*/

	public function testAnd1()
	{
		return 0 && 0;
	}

	public function testAnd2()
	{
		return 0 && 1.5;
	}

	public function testAnd3()
	{
		return 0 && 1.5 && 'A';
	}

	public function testAnd4()
	{
		return 1 && 0 && null;
	}

	public function testAnd9(var a, int b)
	{
		if typeof a == "array" && isset a[b] {
			return a[b];
		}
		return null;
	}

	public function testOr1()
	{
		return 0 || 1;
	}

	public function testOr2()
	{
		return 0 || true;
	}

	
}
