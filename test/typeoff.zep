
/**
 * OO operations
 */

namespace Test;

class Typeoff
{
    protected property;

	public function testNativeStringFalse()
	{
		string testVar;
		let testVar = "sdfsdf";

		return typeof testVar == "object";
	}

	public function testNativeStringTrue()
	{
		string testVar;
		let testVar = "sdfsdf";

		return typeof testVar == "string";
	}

	public function testNativeIntFalse()
	{
		int testVar;
		let testVar = 12345;

		return typeof testVar == "object";
	}

	public function testNativeIntTrue()
	{
		int testVar;
		let testVar = 12345;

		return typeof testVar == "integer";
	}

	public function testNativeDoubleTrue()
	{
		double testVar;
		let testVar = 12345;

		return typeof testVar == "double";
	}

	public function testNativeBoolTrue()
	{
		bool testVar;
		let testVar = true;

		return typeof testVar == "boolean";
	}

	public function testNativeBoolFalse()
	{
		bool testVar;
		let testVar = false;

		return typeof testVar == "double";
	}

	public function testArrayFalse()
	{
		var testVar;
		let testVar = [];

		return typeof testVar == "string";
	}

	public function testArrayTrue()
	{
		var testVar;
		let testVar = [];

		return typeof testVar == "array";
	}

	public function testClassPropertyAccess()
	{
	    let this->property = "test string";
		return typeof this->property == "string";
	}

	public function testUnknownTypeOf(var u)
	{
		return typeof u;
	}

	public function testCallableTypeOf(var cb)
	{
	    return typeof cb == "callable";
	}
}
