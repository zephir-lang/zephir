namespace Stub;

/**
 * `isset a[k]`, `empty(a[k])` and `fetch v, a[k]` convert the offset as PHP's
 * isset does: the same deprecations and warnings as a read, a TypeError naming
 * "isset or empty" for an illegal offset, and an object without ArrayAccess
 * asked through its own handler. The test compares each against plain PHP.
 */
class ArrayIsset
{
	public function issetVar(var a, var k) -> bool
	{
		return isset a[k];
	}

	public function emptyVar(var a, var k) -> bool
	{
		return empty(a[k]);
	}

	public function fetchVar(var a, var k) -> bool
	{
		var value;

		return fetch value, a[k];
	}

	public function issetStringLiteral(var a) -> bool
	{
		return isset a["zz"];
	}

	public function issetIntLiteral(var a) -> bool
	{
		return isset a[1];
	}

	public function issetNested(var a, var i, var j) -> bool
	{
		return isset a[i][j];
	}
}
