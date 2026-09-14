namespace Stub;

/**
 * A literal string array offset must resolve to the key PHP resolves it to.
 *
 * PHP folds a constant numeric string subscript to an integer key while it
 * compiles (Zend/zend_compile.c, zend_handle_numeric_dim), so `$a["3"]` is
 * `$a[3]`. Zephir's literal-offset kernel helpers reached `zend_hash_str_*`
 * directly and so kept the string key, while every zval-indexed counterpart
 * went through `zend_symtable_*` and normalised. The two disagreed even within
 * one Zephir program: a constant array literal is emitted as `add_assoc_*_ex`,
 * which php-src implements with `zend_symtable_str_update`.
 *
 * An ArrayAccess container is the exception PHP itself carves out: it receives
 * the original string, never the folded integer.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2708
 */
class Issue2708
{
	/**
	 * The literal stores the integer key, so the literal offset has to find it.
	 */
	public function issetOnConstantLiteral() -> bool
	{
		var a;

		let a = ["3": 1];

		return isset a["3"];
	}

	public function emptyOnConstantLiteral() -> bool
	{
		var a;

		let a = ["3": 1];

		return empty(a["3"]);
	}

	public function fetchOnConstantLiteral()
	{
		var a, value;

		let a = ["3": "found"];

		if fetch value, a["3"] {
			return value;
		}

		return null;
	}

	public function readOnConstantLiteral()
	{
		var a;

		let a = ["3": "found"];

		return a["3"];
	}

	public function unsetLiteralOffset(array a) -> array
	{
		unset a["3"];

		return a;
	}

	public function writeThenReadBack()
	{
		var a;

		let a = [];
		let a["3"] = "written";

		return a[3];
	}

	/**
	 * `zephir_array_update_multi`'s string slot delegates to the same helper,
	 * so the nested write has to agree with the single one.
	 */
	public function writeMultiDimensional() -> array
	{
		var a;

		let a = [];
		let a["3"]["4"] = "deep";

		return a;
	}

	public function writeThree() -> array
	{
		var a;

		let a = [];
		let a["3"] = 1;

		return a;
	}

	public function writeZero() -> array
	{
		var a;

		let a = [];
		let a["0"] = 1;

		return a;
	}

	public function writeNegativeThree() -> array
	{
		var a;

		let a = [];
		let a["-3"] = 1;

		return a;
	}

	public function writeIntMax() -> array
	{
		var a;

		let a = [];
		let a["9223372036854775807"] = 1;

		return a;
	}

	public function writeLeadingZero() -> array
	{
		var a;

		let a = [];
		let a["03"] = 1;

		return a;
	}

	public function writeLeadingPlus() -> array
	{
		var a;

		let a = [];
		let a["+3"] = 1;

		return a;
	}

	public function writeNegativeZero() -> array
	{
		var a;

		let a = [];
		let a["-0"] = 1;

		return a;
	}

	public function writeLeadingSpace() -> array
	{
		var a;

		let a = [];
		let a[" 3"] = 1;

		return a;
	}

	public function writeTrailingSpace() -> array
	{
		var a;

		let a = [];
		let a["3 "] = 1;

		return a;
	}

	public function writeDecimalPoint() -> array
	{
		var a;

		let a = [];
		let a["3.0"] = 1;

		return a;
	}

	public function writeOverflow() -> array
	{
		var a;

		let a = [];
		let a["9223372036854775808"] = 1;

		return a;
	}

	/**
	 * An integer key moves `nNextFreeElement`, so the append that follows
	 * lands past it rather than at zero.
	 */
	public function appendAfterNumericStringKey() -> array
	{
		var a;

		let a = [];
		let a["3"] = "x";
		let a[] = "y";

		return a;
	}

	/**
	 * array_merge() renumbers integer keys and preserves string ones, so the
	 * key type is observable from the merged result.
	 */
	public function mergeAfterNumericStringKey() -> array
	{
		var a;

		let a = [];
		let a["3"] = "x";

		return array_merge(a, ["y"]);
	}

	/**
	 * The control: an ordinary string key must stay exactly what it is.
	 */
	public function writeOrdinaryKey() -> array
	{
		var a;

		let a = [];
		let a["key_a"] = 1;

		return a;
	}

	public function readOnContainer(var container)
	{
		return container["3"];
	}

	public function issetOnContainer(var container) -> bool
	{
		return isset container["3"];
	}

	public function writeOnContainer(var container) -> void
	{
		let container["3"] = "v";
	}

	public function unsetOnContainer(var container) -> void
	{
		unset container["3"];
	}
}
