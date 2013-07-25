
namespace Test;

class Fibonnaci
{

	public function fibArray()
	{
		var fib, a, b, c;
		int i, n, k, j;

		let fib = [0, 1],
			i = 2,
			n = 10;

		while (i < n) {
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

}
