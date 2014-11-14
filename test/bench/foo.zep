
namespace Test\Bench;

class Foo
{
	public static a = 0;
	public b = 0;
	const TEST = 0;

	public static function empty_for_in_range(var n)
	{
		var i;
		for i in range(1, n) {

		}
	}

	public static function read_static(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = self::a;
		}
	}

	public static function write_static(var n)
	{
		var i;
		for i in range(1, n) {
			let self::a = 0;
		}
	}

	public static function isset_static(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = isset(self::a);
		}
	}

	public static function empty_static(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = empty(self::a);
		}
	}

	public static function f()
	{
	}

	public static function call_static(var n)
	{
		var i;
		for i in range(1, n) {
			self::f();
		}
	}

	public function read_prop(var n)
	{
		var x, i;
		for i in range(1, n) {
			let x = this->b;
		}
	}

	public function write_prop(var n)
	{
		var i;

		for i in range(1, n) {
			let this->b = 0;
		}
	}

	public function assign_add_prop(var n)
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

	public function post_inc_prop(n)
	{
		var i;

		for i in range(1, n) {
			let this->b++;
		}
	}

	public function post_dec_prop(n)
	{
		var i;

		for i in range(1, n) {
			let this->b--;
		}
	}

	public function isset_prop(n)
	{
		var i, x;

		for i in range(1, n) {
			let x = isset(this->b);
		}
	}

	public function empty_prop($n)
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

	public function read_const($n)
	{
		var i, x;
		for i in range(1, n) {
			let x = this::TEST;
		}
	}
}
