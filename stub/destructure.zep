namespace Stub;

/**
 * Destructuring assignment: let [a, b, c] = expr;
 *
 * @see https://github.com/zephir-lang/zephir/issues/2496
 */
class Destructure
{
	protected calls = 0;

	protected data = [7, 8];

	public function basic() -> array
	{
		var arr, a, b, c;

		let arr = [1, 2, 3];
		let [a, b, c] = arr;

		return [a, b, c];
	}

	public function skipped() -> array
	{
		var arr, a, c;

		let arr = [1, 2, 3];
		let [a, , c] = arr;

		return [a, c];
	}

	public function trailingSkip()
	{
		var arr, a;

		let arr = [1, 2];
		let [a, ] = arr;

		return a;
	}

	public function literal() -> array
	{
		var a, b;

		let [a, b] = [1, 2];

		return [a, b];
	}

	public function pair() -> array
	{
		let this->calls = this->calls + 1;

		return [10, 20];
	}

	public function fromCall() -> array
	{
		var a, b;

		let [a, b] = this->pair();

		return [a, b, this->calls];
	}

	public function swap() -> array
	{
		var a, b;

		let a = 1, b = 2;
		let [a, b] = [b, a];

		return [a, b];
	}

	public function fewerTargets() -> array
	{
		var arr, a, b;

		let arr = [1, 2, 3];
		let [a, b] = arr;

		return [a, b];
	}

	public function missingSlot() -> array
	{
		var arr, a, b, c;

		let arr = [1, 2];
		let [a, b, c] = arr;

		return [a, b, c];
	}

	public function nestedValue() -> array
	{
		var a, b;

		let [a, b] = [[1, 2], [3, 4]];

		return [a, b];
	}

	public function typedTargets() -> array
	{
		int i; string s;

		let [i, s] = [5, "x"];

		return [i, s];
	}

	public function keyedSource() -> array
	{
		var arr, a, b;

		let arr = [0: "z", 1: "y"];
		let [a, b] = arr;

		return [a, b];
	}

	public function inLoop() -> array
	{
		var pairs, item, out, k, v;

		let pairs = [[1, 2], [3, 4]], out = [];

		for item in pairs {
			let [k, v] = item;
			let out[] = k + v;
		}

		return out;
	}

	public function chained() -> array
	{
		var arr, a, b, c;

		let arr = [1, 2];
		let [a, b] = arr, c = 9;

		return [a, b, c];
	}

	public function fromProperty() -> array
	{
		var a, b;

		let [a, b] = this->data;

		return [a, b];
	}
}
