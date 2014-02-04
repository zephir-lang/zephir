
/**
 * Unset statement tests
 */

namespace Test;

class IssetTest
{

	public function testIssetArray1(var a, var b)
	{
		return isset a[b];
	}

	public function testIssetArray2(var a, int b)
	{
		return isset a[b];
	}

	public function testIssetArray3(var a, string b)
	{
		return isset a[b];
	}

	public function testIssetArray4(var a)
	{
		return isset a[0];
	}

	public function testIssetArray5(var a)
	{
		return isset a["a"];
	}

	public function testIssetProperty1(var a)
	{
		return isset a->b;
	}

	public function testIssetProperty2(var a, var b)
	{
		return isset a->{b};
	}

	public function testIssetProperty3(var a)
	{
		return isset a->{"b"};
	}

}