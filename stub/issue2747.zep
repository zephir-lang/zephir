namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2747
 *
 * A compound assignment on an array element reads the element, applies the
 * operator and writes the result back, as PHP's ZEND_ASSIGN_DIM_OP does. It
 * must never overwrite the element with the right-hand side. Every container
 * form is here: a local, an object property and a static property, with one
 * offset, several offsets and a trailing append. The test compares each
 * against the same PHP statement.
 */
class Issue2747
{
	public p = ["k": 10];

	public static sp = ["k": 10];

	public function add(var a, var k, var v)
	{
		let a[k] += v;

		return a;
	}

	public function sub(var a, var k, var v)
	{
		let a[k] -= v;

		return a;
	}

	public function mul(var a, var k, var v)
	{
		let a[k] *= v;

		return a;
	}

	public function div(var a, var k, var v)
	{
		let a[k] /= v;

		return a;
	}

	public function mod(var a, var k, var v)
	{
		let a[k] %= v;

		return a;
	}

	public function concat(var a, var k, var v)
	{
		let a[k] .= v;

		return a;
	}

	public function bitwiseAnd(var a, var k, var v)
	{
		let a[k] &= v;

		return a;
	}

	public function bitwiseOr(var a, var k, var v)
	{
		let a[k] |= v;

		return a;
	}

	public function bitwiseXor(var a, var k, var v)
	{
		let a[k] ^= v;

		return a;
	}

	public function shiftLeft(var a, var k, var v)
	{
		let a[k] <<= v;

		return a;
	}

	public function shiftRight(var a, var k, var v)
	{
		let a[k] >>= v;

		return a;
	}

	public function addStringLiteralKey(array a, var v)
	{
		let a["k"] += v;

		return a;
	}

	public function addIntLiteralKey(array a, var v)
	{
		let a[1] += v;

		return a;
	}

	public function addLongKey(array a, long k, var v)
	{
		let a[k] += v;

		return a;
	}

	public function addStringKey(array a, string k, var v)
	{
		let a[k] += v;

		return a;
	}

	public function addIntLiteral(array a)
	{
		let a["k"] += 2;

		return a;
	}

	public function concatStringLiteral(array a)
	{
		let a["k"] .= "x";

		return a;
	}

	public function addLongValue(var a, var k, long v)
	{
		let a[k] += v;

		return a;
	}

	public function mulDoubleValue(var a, var k, double v)
	{
		let a[k] *= v;

		return a;
	}

	public function concatOwnElement(array a)
	{
		let a["k"] .= a["k"];

		return a;
	}

	public function addToCopy(array a, var k, var v) -> array
	{
		var b;

		let b = a;
		let b[k] += v;

		return [a, b];
	}

	public function addNested(var a, var i, var j, var v)
	{
		let a[i][j] += v;

		return a;
	}

	public function concatNestedLiteral(var a, var v)
	{
		let a[1]["x"] .= v;

		return a;
	}

	public function subAppend(var a, var v)
	{
		let a[] -= v;

		return a;
	}

	public function concatNestedAppend(var a, var k, var v)
	{
		let a[k][] .= v;

		return a;
	}

	public function addThis(var k, var v)
	{
		let this->p[k] += v;

		return this->p;
	}

	public function addThisNested(var k, var j, var v)
	{
		let this->p[k][j] += v;

		return this->p;
	}

	public function subThisAppend(var v)
	{
		let this->p[] -= v;

		return this->p;
	}

	public function concatThisNestedAppend(var k, var v)
	{
		let this->p[k][] .= v;

		return this->p;
	}

	public function addObject(var o, var k, var v)
	{
		let o->p[k] += v;

		return o;
	}

	public function addStatic(var k, var v)
	{
		let self::sp[k] += v;

		return self::sp;
	}

	public function addStaticNested(var k, var j, var v)
	{
		let self::sp[k][j] += v;

		return self::sp;
	}

	public function subStaticAppend(var v)
	{
		let self::sp[] -= v;

		return self::sp;
	}

	public function concatStaticNestedAppend(var k, var v)
	{
		let self::sp[k][] .= v;

		return self::sp;
	}

	public function setProperties(var value) -> void
	{
		let this->p = value;
		let self::sp = value;
	}

	/**
	 * Evaluation order: PHP evaluates an index expression before the
	 * right-hand side, except a plain variable index, which is read when the
	 * element is written. `trace` records each evaluation.
	 */
	public trace = [];

	public counter = 0;

	public function key(var n)
	{
		let this->trace[] = "k" . n;

		return n;
	}

	public function value(var n)
	{
		let this->trace[] = "v" . n;

		return n;
	}

	public function bump()
	{
		let this->counter++;

		return this->counter;
	}

	public function orderLocal() -> array
	{
		var a = [];

		let this->trace = [];
		let a[this->key(1)] = this->value(2);

		return [this->trace, a];
	}

	public function orderLocalNested() -> array
	{
		var a = [];

		let this->trace = [];
		let a[this->key(1)][this->key(2)] = this->value(3);

		return [this->trace, a];
	}

	public function orderLocalAppend() -> array
	{
		var a = [];

		let this->trace = [];
		let a[this->key(1)][] = this->value(2);

		return [this->trace, a];
	}

	public function orderCompound() -> array
	{
		var a = [1: "a"];

		let this->trace = [];
		let a[this->key(1)] .= this->value(2);

		return [this->trace, a];
	}

	public function orderThis() -> array
	{
		let this->trace = [];
		let this->p = [];
		let this->p[this->key(1)] = this->value(2);

		return [this->trace, this->p];
	}

	public function orderThisNested() -> array
	{
		let this->trace = [];
		let this->p = [];
		let this->p[this->key(1)][this->key(2)][] = this->value(3);

		return [this->trace, this->p];
	}

	public function orderStatic() -> array
	{
		let this->trace = [];
		let self::sp = [];
		let self::sp[this->key(1)][this->key(2)] = this->value(3);

		return [this->trace, self::sp];
	}

	public function orderStaticAppend() -> array
	{
		let this->trace = [];
		let self::sp = [];
		let self::sp[this->key(1)][] = this->value(2);

		return [this->trace, self::sp];
	}

	public function orderPropertyIndex() -> array
	{
		var a = [];

		let this->counter = 0;
		let a[this->counter] = this->bump();

		return a;
	}

	public function orderPropertyValue() -> array
	{
		var a = [];

		let this->counter = 0;
		let a[this->bump()] = this->counter;

		return a;
	}

	public function orderComputedIndex() -> array
	{
		var a = [];

		let this->counter = 0;
		let a[this->counter + 10] = this->bump();

		return a;
	}

	public function orderStringOffset() -> string
	{
		string s = "abc";

		let this->counter = 0;
		let s[this->counter] = (string) this->bump();

		return s;
	}

	public function orderVariableIndex() -> array
	{
		var i, f, b = [];

		let i = 0;
		let f = function () use (&i) {
			let i = 5;

			return "v";
		};
		let b[i] = {f}();

		return b;
	}

	public function orderVariableExpressionIndex() -> array
	{
		var i, f, b = [];

		let i = 0;
		let f = function () use (&i) {
			let i = 5;

			return "v";
		};
		let b[i + 1] = {f}();

		return b;
	}

	public function orderLiteralIndex() -> array
	{
		var a = [];

		let this->trace = [];
		let a[0] = this->value(1);
		let a["k"] = this->value(2);

		return [this->trace, a];
	}
}
