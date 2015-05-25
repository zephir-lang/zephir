
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

	internal function fibonacci(long n) -> long
	{
  		if n == 1 || n == 2 {
    		return 1;
  		}
    	return this->fibonacci(n - 1) + this->fibonacci(n - 2);
  	}

  	public function callFibonacci(long n) -> long
  	{
  		return this->fibonacci(n);
  	}
}
