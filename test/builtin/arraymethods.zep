
namespace Test\BuiltIn;

class ArrayMethods
{
	public function getJoin1()
	{
		return [1, 2, 3]->join("-");
	}

	public function getReversed1()
	{
		return [1, 2, 3]->reversed();
	}

	public function getMap1()
	{
		return [1, 2, 3]->map(x => x * 100);
	}
}