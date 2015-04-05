
namespace Test;

class Functional
{

	public function map1(array a)
	{
		return a->map(x => x * x);
	}

	public function map2(array a, callable b)
	{
		return a->map(b);
	}
}
