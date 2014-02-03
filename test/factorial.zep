
namespace Test;

class Factorial
{

	public function intFactorial(int n) -> int
	{
	    long result = 1, i;	    
	    for i in range(2, n) {
	        let result *= i;
	    }
	    return result;
	}

}
