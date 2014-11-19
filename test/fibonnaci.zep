
namespace Test;

class Fibonnaci
{

	public function fibInt()
	{
		int x, y, n, z;

		let z = 0,
			x = 1,
			y = 2,
			n = 10;
		while z < n {
			let z = x + y,
				x = y,
				y = z;
		}
		return z;
	}

	public function fibDouble()
	{
		double x, y, n, z;

		let z = 0.0,
			x = 1.0,
			y = 2.0,
			n = 10.0;
		while z < n {
			let z = x + y,
				x = y,
				y = z;
		}
		return z;
	}

	public function fibArray()
	{
		var fib, a, b, c;
		int i, n, k, j;

		let fib = [0, 1],
			i = 2,
			n = 10;

		while i < n {
			let k = i - 1,
				j = i - 2,
				a = fib[k],
				b = fib[j],
				c = a + b,
				fib[i] = c,
				i++;
		}

		return fib;
	}

	public function fibArray2()
	{
		var fib; int i, n;

		let fib = [0, 1], i = 2, n = 10;
		while i < n {
			let fib[i] = fib[i - 1] + fib[i - 2], i++;
		}

		return fib;
	}

	private function fibonacciRecursive(int n)
	{
		if n == 0 {
			return 0;
		} else {
			if n == 1 {
				return 1;
			} else {
				return this->fibonacciRecursive(n - 1) + this->fibonacciRecursive(n - 2);
			}
		}
	}

	private final function fibonacciFinalRecursive(int n)
	{
		if n == 0 {
			return 0;
		} else {
			if n == 1 {
				return 1;
			} else {
				return this->fibonacciFinalRecursive(n - 1) + this->fibonacciFinalRecursive(n - 2);
			}
		}
	}
}
