
/**
 * OO operations
 */

namespace Stub;

class Instanceoff
{
	public function testInstanceOf1() -> bool
	{
		var a;
		let a = new \stdClass();
		return a instanceof \stdClass;
	}

	public function testInstanceOf2() -> bool
	{
		var a;
		let a = new self();
		return a instanceof Instanceoff;
	}

	public function testInstanceOf3() -> bool
	{
		var a;
		let a = new \stdClass();
		return a instanceof UnknownClass;
	}

	public function testInstanceOf4(var a) -> bool
	{
		if a instanceof \Traversable {
			return true;
		}

		return false;
	}

	public function testInstanceOf5(var a) -> bool
	{
		if a instanceof this {
			return true;
		}

		return false;
	}

	public function testInstanceOf6() -> bool
	{
		var a;
		let a = new static();
		return a instanceof Instanceoff;
	}

	public function testInstanceOf7(object test) -> bool
	{
		return test instanceof Instanceoff;
	}

	public function testInstanceOf8(string test) -> bool
	{
		var a;
		let a = new static();
		return a instanceof test;
	}

	public function testInstanceOf9(object a, string test) -> bool
	{
		return a instanceof test;
	}
}
