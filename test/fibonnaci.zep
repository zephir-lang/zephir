
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
		while (z < n) {
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
