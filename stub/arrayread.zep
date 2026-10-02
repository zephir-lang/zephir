namespace Stub;

/**
 * Reading `a[k]` raises PHP 8's diagnostics, not PHP 5's: a missing key is
 * the warning `Undefined array key`, a null or scalar container is the
 * warning `Trying to access array offset on ...`, an offset is converted with
 * PHP's own deprecations and TypeError, and an object without ArrayAccess is
 * read through its own handler. Every read form the compiler emits is here;
 * the test compares each against plain PHP.
 */
class ArrayRead
{
	public function read(var a, var k)
	{
		return a[k];
	}

	public function readFromArray(array a, var k)
	{
		return a[k];
	}

	public function readStringLiteral(var a)
	{
		return a["zz"];
	}

	public function readIntLiteral(var a)
	{
		return a[7];
	}

	public function readLong(var a, long k)
	{
		return a[k];
	}

	public function readString(var a, string k)
	{
		return a[k];
	}

	public function readNested(var a, var i, var j)
	{
		return a[i][j];
	}

	public function readNestedLiteral(var a)
	{
		return a["zz"]["yy"];
	}

	public function readIntoLocal(var a, var k)
	{
		var value;

		let value = a[k];

		return value;
	}
}
