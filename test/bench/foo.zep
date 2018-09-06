
namespace Test\Bench;

class Foo
{
	public static a = 0;
	public b = 0;
	const TEST = 0;

	public static function emptyForInRange(var n)
	{
		var i;
		for i in range(1, n) {

		}
	}

	public static function readStatic(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = self::a;
		}
	}

	public static function writeStatic(var n)
	{
		var i;
		for i in range(1, n) {
			let self::a = 0;
		}
	}

	public static function issetStatic(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = isset(self::a);
		}
	}

	public static function emptyStatic(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = empty(self::a);
		}
	}

	public function readProp(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = this->b;
		}
	}

	public function writeProp(var n)
	{
		var i;

		for i in range(1, n) {
			let this->b = 0;
		}
	}

	public function assignAddProp(var n)
	{
		var i;
		for i in range(1, n) {
			let this->b += 2;
		}
	}

	/**
	public function pre_inc_prop(var n) {
		var i;

		for i in range(1, n) {
			++this->b;
		}
	}

	public function pre_dec_prop($n) {
		var i;

		for i in range(1, n) {
			--$this->b;
		}
	}*/

	public function postIncProp(n)
	{
		var i;

		for i in range(1, n) {
			let this->b++;
		}
	}

	public function postDecProp(n)
	{
		var i;

		for i in range(1, n) {
			let this->b--;
		}
	}

	public function issetProp(n)
	{
		var i, x;

		for i in range(1, n) {
			let x = isset(this->b);
		}
	}

	public function emptyProp($n)
	{
		var i, x;

		for i in range(1, n) {
			let x = empty(this->b);
		}
	}

	public function g()
	{

	}

	public function call(var n)
	{
		var i;
		for i in range(1, n) {
			this->g();
		}
	}

	static public function staticMethod() -> void
	{
	}

	static public function staticMethod1() -> boolean
	{
		return true;
	}

	public function scall(var n)
	{
		var i;

		for i in range(1, n) {
			self::staticMethod();
		}
	}

	public function scallWithReturnTrue(var n)
	{
		var i;

		for i in range(1, n) {
			self::staticMethod1();
		}
	}

	public function readConst(var n)
	{
		var i, x;
		for i in range(1, n) {
			let x = this::TEST;
		}
	}
}
