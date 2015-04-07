
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

	public function testMixed1()
	{
		var first, second;

		let first = 0;
		let second = 1;
		if first && second || second {
			echo "ok";
		} else {
			echo "wrong";
		}
	}

	public function testMixed2(var match, var minLength)
	{
		if is_array(match) && count(match) == 2 && strlen(match[0]) >= minLength {
			return true;
		}
		return false;
	}

	public function testMixed3()
	{
		boolean a, b;
		let a = 0 && 1 || 1, b = (0 && 1) || 1;
		return a == b;
	}

	public function testMixed4(boolean a, boolean b)
	{
		return !a && !b || a && b;
	}
}
