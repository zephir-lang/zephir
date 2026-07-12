
/**
 * Multi-dimensional array assignment (`let a[i][j] = v`).
 *
 * Exercises the offset shapes handled by zephir_array_update_multi_ex:
 * dynamic keys ("z"), literal long keys ("l"), literal string keys ("s"),
 * mixed chains, append-final ("a"), and last-level overwrite. Backs the
 * regression guard for issue #1884 (final-offset fast-path in the kernel).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884
 */

namespace Stub;

class MultiDimArray
{
	/**
	 * The exact snippet from issue #1884: nested range loops building an
	 * (size+1) x (size+1) matrix of 1s. `i`/`j` are dynamic vars, so the
	 * inner write compiles to zephir_array_update_multi(..., SL("zz"), ...).
	 */
	public function generate(int size) -> array
	{
		var output = [];
		var i = 0;
		var j = 0;

		for i in range(0, size) {
			let output[i] = [];

			for j in range(0, size) {
				let output[i][j] = 1;
			}
		}

		return output;
	}

	/**
	 * Same build with int-typed loop vars: the inner offsets resolve to
	 * literal-long keys, i.e. SL("ll") instead of SL("zz").
	 */
	public function generateTyped(int size) -> array
	{
		array output = [];
		int i = 0, j = 0;

		for i in range(0, size) {
			let output[i] = [];

			for j in range(0, size) {
				let output[i][j] = 1;
			}
		}

		return output;
	}

	/**
	 * Literal long keys at both levels: SL("ll").
	 */
	public function intKeys() -> array
	{
		var a = [];
		let a[0][1] = 10;
		return a;
	}

	/**
	 * Literal string keys at both levels: SL("ss").
	 */
	public function stringKeys() -> array
	{
		var a = [];
		let a["x"]["y"] = 10;
		return a;
	}

	/**
	 * Three levels of dynamic string keys: SL("sss").
	 */
	public function threeLevel() -> array
	{
		var a = [];
		let a["x"]["y"]["z"] = 10;
		return a;
	}

	/**
	 * Mixed key kinds down the chain: SL("sl") and SL("ls").
	 */
	public function mixedKeys() -> array
	{
		var a = [];
		let a["k"][0] = 1;
		let a[1]["k"] = 2;
		return a;
	}

	/**
	 * Append on the final level: SL("la") / SL("sa").
	 */
	public function appendFinal() -> array
	{
		var a = [];
		let a[0][] = "first";
		let a[0][] = "second";
		let a["bag"][] = 1;
		return a;
	}

	/**
	 * Overwrite the same final slot twice. Exercises the last-level store
	 * path where the pre-fetch previously found an existing scalar.
	 */
	public function overwriteFinal() -> array
	{
		var a = [];
		let a[0][0] = 1;
		let a[0][0] = 2;
		return a;
	}

	/**
	 * The final slot first holds an array, then is overwritten by a scalar.
	 * Exercises the branch where the pre-fetch previously found an array.
	 */
	public function overwriteArrayWithScalar() -> array
	{
		var a = [];
		let a["p"]["q"] = [1, 2, 3];
		let a["p"]["q"] = 99;
		return a;
	}
}
