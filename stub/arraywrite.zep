namespace Stub;

/**
 * Writing `a[k] = v` raises PHP 8's diagnostics: null and false become
 * arrays (false deprecated since 8.1), a scalar container throws, an object
 * is written through its own handler, the offset is converted with PHP's own
 * deprecations and TypeError, and a full array refuses `[]`. Every write form
 * the compiler emits is here; the test compares each against plain PHP.
 */
class ArrayWrite
{
	public p;

	public static sp;

	public function write(var a, var k, var v)
	{
		let a[k] = v;

		return a;
	}

	public function writeStringLiteral(var a, var v)
	{
		let a["k"] = v;

		return a;
	}

	public function writeIntLiteral(var a, var v)
	{
		let a[1] = v;

		return a;
	}

	public function writeLong(var a, long k, var v)
	{
		let a[k] = v;

		return a;
	}

	public function writeNative(var a, var k)
	{
		let a[k] = 5;

		return a;
	}

	public function writeNested(var a, var i, var j, var v)
	{
		let a[i][j] = v;

		return a;
	}

	public function append(var a, var v)
	{
		let a[] = v;

		return a;
	}

	public function appendNative(var a)
	{
		let a[] = 5;

		return a;
	}

	public function appendNested(var a, var k, var v)
	{
		let a[k][] = v;

		return a;
	}

	public function writeThis(var p, var k, var v)
	{
		let this->p = p;
		let this->p[k] = v;

		return this->p;
	}

	public function writeThisNested(var p, var i, var j, var v)
	{
		let this->p = p;
		let this->p[i][j] = v;

		return this->p;
	}

	public function appendThis(var p, var v)
	{
		let this->p = p;
		let this->p[] = v;

		return this->p;
	}

	public function writeStatic(var p, var k, var v)
	{
		let self::sp = p;
		let self::sp[k] = v;

		return self::sp;
	}

	public function appendStatic(var p, var v)
	{
		let self::sp = p;
		let self::sp[] = v;

		return self::sp;
	}

	public function concatDeep(var a, var i, var j, var k, var v)
	{
		let a[i][j][k] .= v;

		return a;
	}

	public function writeObject(var o, var k, var v)
	{
		let o->p[k] = v;

		return o;
	}
}
