
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

	public function testInstanceOf4(var a)
	{
		if a instanceof \Traversable {
			return true;
		}

		return false;
	}

	public function testInstanceOf5(var a)
	{
		if a instanceof this {
			return true;
		}

		return false;
	}
}
