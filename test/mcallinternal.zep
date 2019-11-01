
/**
 * Method calls
 */

namespace Test;

class McallInternal
{
	internal function a()
	{
		return strlen("hello");
	}

	internal function b(a, b)
	{

	}

	internal function c(long a, long b)
	{
		return a + b;
	}

	public function e()
	{
		return strlen("hello");
	}

	public function d()
	{
		int a = 0; int i;
		for i in range(0, 1000000) {
			let a += (int) this->a();
		}
		return a;
	}

	public function f()
	{
		int a = 0; int i;
		for i in range(0, 1000000) {
			let a += (int) this->e();
		}
		return a;
	}

	public function g()
	{
		int a = 0; long i;
		for i in range(0, 1000) {
			let a += (long) this->c(i, i);
		}
		return a;
	}

	internal function other(long a, long b) -> double
	{
		return a / b;
  	}

  	public function callFibonacci() -> double
  	{
		double $p = 0; long $i = 0;
		for i in range(0, 10000000) {
			let $p += (double) $this->other($i, $i + 1);
		}
		return $p;
  	}

	internal function test1956() -> <McallInternal>
	{
		return this;
  	}

  	public function issue1956() -> double
  	{
		return $this->test1956();
  	}
}
