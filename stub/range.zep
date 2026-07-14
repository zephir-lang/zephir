
/**
 * Range operators (`..` inclusive, `...` exclusive) and range loops.
 */

namespace Stub;

class Range
{
	public function inclusive1()
	{
		return 0..10;
	}

	public function exclusive1()
	{
		return 0...10;
	}

	/**
	 * `for i in a..b` with a typed int loop variable: compiles to an integer
	 * counting loop (see #2433). Before the fix this form failed to compile.
	 */
	public function loopInclusive() -> array
	{
		array result = [];
		int i = 0;
		for i in 0..5 {
			let result[] = i;
		}
		return result;
	}

	public function loopExclusive() -> array
	{
		array result = [];
		int i = 0;
		for i in 0...5 {
			let result[] = i;
		}
		return result;
	}

	public function loopReverse() -> array
	{
		array result = [];
		int i = 0;
		for i in reverse 0..5 {
			let result[] = i;
		}
		return result;
	}

	public function loopVarBounds(int a, int b) -> array
	{
		array result = [];
		int i = 0;
		for i in a..b {
			let result[] = i;
		}
		return result;
	}

	/**
	 * Equivalent loop written with an explicit range() call. Used to prove the
	 * `..` operator produces identical output to range().
	 */
	public function loopRangeFn() -> array
	{
		array result = [];
		int i = 0;
		for i in range(0, 5) {
			let result[] = i;
		}
		return result;
	}

	/**
	 * Char range: bounds are not int-compatible, so compileRange() bails and the
	 * loop falls back to the generic array path. Must still work.
	 */
	public function loopChars() -> string
	{
		string out = "";
		var ch;
		for ch in "a".."e" {
			let out .= ch;
		}
		return out;
	}
}
