namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/760
 *
 * PHP's `$str[$off]` is a byte-indexed string offset. Zephir only supported it
 * when the source variable was declared `string`, so the compile-time type
 * switch in `NativeArrayAccess` could route it to `ZEPHIR_STRING_OFFSET`. A
 * `var` holding a string fell through to the array kernel and produced NULL.
 *
 * Every method here has a plain-PHP counterpart in the test, and the two
 * transcripts (diagnostics + value) must be byte-identical.
 */
class Issue760
{
	/**
	 * Dynamic source, native int offset. Emits `zephir_array_fetch_long()`.
	 */
	public function dynIntOffset(var s, int i)
	{
		var ch;
		let ch = s[i];
		return ch;
	}

	/**
	 * Dynamic source, dynamic offset. Emits `zephir_array_fetch()` and so
	 * exercises PHP's offset normalisation (numeric strings, casts, TypeError).
	 */
	public function dynVarOffset(var s, var i)
	{
		var ch;
		let ch = s[i];
		return ch;
	}

	/**
	 * Declared `string` source: the path that already worked, kept as a
	 * control so a regression there is visible.
	 */
	public function strIntOffset(string s, int i)
	{
		var ch;
		let ch = s[i];
		return ch;
	}

	/**
	 * A local `string` is a plain zval, unlike a `string` parameter which is a
	 * `zend_string*` plus a companion `_zv`.
	 */
	public function localStringOffset(int i)
	{
		string s = "abcdef";
		var ch;

		let ch = s[i];

		return ch;
	}

	/**
	 * A native `char` destination takes the raw byte, not a 1-char string:
	 * `char` is int-like by construction (#2069). Out of range still yields
	 * NUL, but must warn exactly as PHP does.
	 */
	public function charDest(string s, int i) -> int
	{
		char c;
		let c = s[i];
		return c;
	}

	/**
	 * Same as charDest, sourced from a `var`. Used to fail to compile with
	 * "Cannot use variable: char to assign array index".
	 */
	public function dynCharDest(var s, int i) -> int
	{
		char c;
		let c = s[i];
		return c;
	}

	/**
	 * A `string` source with a dynamic offset. Used to fail to compile with
	 * "Cannot use index type variable as offset"; the offset now runs PHP's
	 * normalisation at runtime.
	 */
	public function strVarOffset(string s, var i)
	{
		var ch;
		let ch = s[i];
		return ch;
	}

	/**
	 * A string literal source, PHP's `"abcdef"[1]`. Used to fail to compile
	 * with "Cannot use expression: string as an array".
	 */
	public function literalOffset(var i)
	{
		var ch;
		let ch = "abcdef"[i];
		return ch;
	}

	/**
	 * Returning the offset straight out, with no intermediate variable: the
	 * destination is `return_value`, which takes a different boxing decision.
	 */
	public function returnStrOffset(string s, int i)
	{
		return s[i];
	}

	public function returnDynOffset(var s, int i)
	{
		return s[i];
	}

	/**
	 * isset() on a string offset: PHP is silent for every illegal offset, and
	 * folds a negative one from the end.
	 */
	public function dynIsset(var s, var i) -> bool
	{
		return isset s[i];
	}

	public function strIsset(string s, var i) -> bool
	{
		return isset s[i];
	}

	/**
	 * empty() must not report anything either: PHP has a separate silent
	 * handler for it, so `empty(s[99])` warns in neither language.
	 */
	public function dynEmpty(var s, var i) -> bool
	{
		return empty s[i];
	}

	public function strEmpty(string s, var i) -> bool
	{
		return empty s[i];
	}

	/**
	 * `empty(arr[9])` used to emit "Undefined index", which PHP never does.
	 */
	public function arrayEmpty(array a, var i) -> bool
	{
		return empty a[i];
	}

	/**
	 * unset() and [] on a string are runtime Errors in PHP. Only reachable
	 * through a `var`; on a declared `string` the compiler rejects them
	 * outright, since the operation could never have succeeded.
	 */
	public function dynUnset(var s, var i)
	{
		unset s[i];

		return s;
	}

	public function dynAppend(var s)
	{
		let s[] = "x";

		return s;
	}

	/**
	 * Writing through a string offset. Extends the string with spaces when the
	 * offset is past the end, stores only the first byte, and refuses an empty
	 * value.
	 */
	public function dynWrite(var s, var i, var v)
	{
		let s[i] = v;

		return s;
	}

	public function dynWriteInt(var s, int i, var v)
	{
		let s[i] = v;

		return s;
	}

	/**
	 * Writing through a declared `string`. A `string` parameter is passed by
	 * value in PHP, so the caller's string must be left alone.
	 */
	public function strWrite(string s, var i, var v) -> string
	{
		let s[i] = v;

		return s;
	}

	public function localStringWrite(var i, var v) -> string
	{
		string s = "abcdef";

		let s[i] = v;

		return s;
	}

	/**
	 * `for c in s` over a dynamically typed source. A declared `string`
	 * traverses its bytes, so a `var` holding one has to do the same.
	 */
	public function dynTraverse(var s) -> array
	{
		var c;
		array out = [];

		for c in s {
			let out[] = c;
		}

		return out;
	}

	public function dynTraverseKeyed(var s) -> array
	{
		var k, c;
		array out = [];

		for k, c in s {
			let out[] = [k, c];
		}

		return out;
	}

	public function dynTraverseReverse(var s) -> array
	{
		var c;
		array out = [];

		for c in reverse s {
			let out[] = c;
		}

		return out;
	}

	public function strTraverse(string s) -> array
	{
		char c;
		array out = [];

		for c in s {
			let out[] = c;
		}

		return out;
	}
}
