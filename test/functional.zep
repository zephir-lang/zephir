
namespace Test;

use SplFixedArray as FixedArray;

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

	public function map3()
	{
		var p;
		let p = new FixedArray(5);
		p->offsetSet(0, 300);
		p->offsetSet(1, 300);
		p->offsetSet(2, 300);
		return p;
	}
}
