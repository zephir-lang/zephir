
/**
 * OO operations
 */

namespace Test;

class Instanceoff
{
	public function testInstanceOf1()
	{
		var a;
		let a = new \stdClass();
		return a instanceof \stdClass;
	}

	public function testInstanceOf2()
	{
		var a;
		let a = new self();
		return a instanceof Instanceoff;
	}

	public function testInstanceOf3()
	{
		var a;
		let a = new \stdClass();
		return a instanceof UnknownClass;
	}

}
