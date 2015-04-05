
namespace Test;

class Factorial
{

	public function intIterativeFactorial(int n) -> int
	{
		long result = 1, i;
		for i in range(2, n) {
			let result *= i;
		}
		return result;
	}

	public function intRecursiveFactorial(int num) -> int
	{
		return num === 0 || num === 1 ? 1 : num * this->intRecursiveFactorial(num - 1);
	}
}
