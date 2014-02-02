
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

	public function testAnd4(var a, int b)
	{
		if typeof a == "array" && isset a[b] {
			return a[b];
		}
		return null;
	}

	
}
