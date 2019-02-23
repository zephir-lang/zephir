namespace Test;

class ArrayIteratorTest
{
	public static function test()
	{
		var arr, k, v;
		let arr = new ArrayIterator();
		for k, v in arr {
			return k . v;
		} 
	}
}
