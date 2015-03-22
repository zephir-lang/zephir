
namespace Test\Optimizers;

class Substr
{
	public function testTwoArguments(var str, var start)
	{
		return substr(str, start);
	}
	
	public function testThreeArguments(var str, var start, var offset)
	{
		return substr(str, start, offset);
	}
}
