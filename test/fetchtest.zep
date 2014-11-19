
/**
 * Fetch statement tests
 */

namespace Test;

class FetchTest
{

	public function testFetchArray1(var a, var b)
	{
		var c;
		return fetch c, a[b];
	}

	public function testFetchArray2(var a, int b)
	{
		var c;
		return fetch c, a[b];
	}

	public function testFetchArray3(var a, string b)
	{
		var c;
		return fetch c, a[b];
	}

	public function testFetchObject1(var a, var b)
	{
		var c;
		return fetch c, a->{b};
	}
}