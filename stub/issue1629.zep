namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1629
 *
 * A string offset read (`s[i]`) is PHP's `$s[$i]`: a 1-char string. It boxes as
 * one whenever it crosses into a zval. A declared `char`/`uchar` variable is a
 * native numeric byte instead (it does arithmetic), so boxing that yields its
 * integer value -- see `Chars::arrayOfChars()` and #1988.
 */
class Issue1629
{
	protected parts = [];

	protected static staticParts = [];

	/**
	 * Case 1: a `string` target. Used to emit dead PHP-5 C that gcc rejected.
	 */
	public function stringTarget(string keyspace, int nb) -> string
	{
		string ch;
		let ch = keyspace[nb];
		return ch;
	}

	/**
	 * Case 2: a declared `char` boxed into an array keeps its byte value.
	 * Used to fail with "Unknown type: char".
	 */
	public function charTarget(string keyspace, int nb) -> array
	{
		char ch;
		array pieces = [];

		let ch = keyspace[nb];
		let pieces[] = ch;

		return pieces;
	}

	/**
	 * Case 3: append the offset read straight into an array, then implode.
	 * Used to fail with "Unknown type: uchar".
	 */
	public function rebuild(string keyspace) -> string
	{
		int i, max;
		array pieces = [];

		let max = strlen(keyspace) - 1;

		for i in range(0, max) {
			let pieces[] = keyspace[i];
		}

		return implode("", pieces);
	}

	/**
	 * The workaround the reporter settled on -- must match PHP's `$s[$i]`.
	 */
	public function varTarget(string keyspace, int nb)
	{
		var ch;
		let ch = keyspace[nb];
		return ch;
	}

	/**
	 * A native-typed target still reads the raw byte (unchanged).
	 */
	public function intTarget(string keyspace, int nb) -> int
	{
		int c;
		let c = keyspace[nb];
		return c;
	}

	public function arrayIndexTarget(string keyspace, int nb) -> array
	{
		array pieces = [];
		let pieces[3] = keyspace[nb];
		return pieces;
	}

	public function propertyAppendTarget(string keyspace, int nb) -> array
	{
		let this->parts = [];
		let this->parts[] = keyspace[nb];
		return this->parts;
	}

	public function propertyIndexTarget(string keyspace, int nb) -> array
	{
		let this->parts = [];
		let this->parts["k"] = keyspace[nb];
		return this->parts;
	}

	public static function staticIndexTarget(string keyspace, int nb) -> array
	{
		let self::staticParts = [];
		let self::staticParts[0] = keyspace[nb];
		return self::staticParts;
	}

	public static function staticAppendTarget(string keyspace, int nb) -> array
	{
		let self::staticParts = [];
		let self::staticParts[] = keyspace[nb];
		return self::staticParts;
	}

	/**
	 * A declared `char` assigned to a `string` target: the string-typed left
	 * hand side asks for the character, not the byte.
	 */
	public function charToStringTarget(string keyspace, int nb) -> string
	{
		char ch;
		string out;

		let ch = keyspace[nb];
		let out = ch;

		return out;
	}

	/**
	 * Sibling of the same dead PHP-5 codegen: an `int` assigned to a `string`.
	 */
	public function intToStringTarget() -> string
	{
		int n = 42;
		string out;

		let out = n;

		return out;
	}

	public function castStringOfChar(string keyspace, int nb) -> string
	{
		char ch;
		let ch = keyspace[nb];
		return (string) ch;
	}

	public function castStringOfOffset(string keyspace, int nb) -> string
	{
		string out;
		let out = (string) keyspace[nb];
		return out;
	}

	public function castCharOfVar(var value) -> char
	{
		return (char) value;
	}

	public function castUcharOfVar(var value) -> uchar
	{
		return (uchar) value;
	}

	public function ucharParam(uchar value) -> int
	{
		int out;
		let out = value;
		return out;
	}

	public function keyedCharArray(string keyspace, int nb) -> array
	{
		char ch;
		let ch = keyspace[nb];
		return [1: ch, "k": ch];
	}

	public function doubleFromChar(string keyspace, int nb) -> double
	{
		char ch;
		double d;

		let ch = keyspace[nb];
		let d = ch;

		return d;
	}

	public function boolFromChar(string keyspace, int nb) -> bool
	{
		char ch;
		bool b;

		let ch = keyspace[nb];
		let b = ch;

		return b;
	}

	/**
	 * A char cast assigned to a dynamic variable used to emit `ZVAL_LONG(&x, ''a'')`.
	 */
	public function dynamicFromCharCast()
	{
		var dyn;

		// Seed with a string so the variable cannot be inferred as a native
		// char and the cast really has to be boxed into a zval.
		let dyn = "seed";
		let dyn = (char) 'a';

		return dyn;
	}

	/**
	 * A bare char literal returned directly used to emit `(unsigned char) (a)`.
	 */
	public function returnCharLiteral() -> char
	{
		return 'a';
	}

	public function appendCharLiteral() -> array
	{
		array pieces = [];
		let pieces[] = 'a';
		return pieces;
	}
}
