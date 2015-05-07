
/**
 * Cast tests
 */

namespace Test;

class Cast
{
	/** To int cast */

	public function testIntCastFromFloat() -> int
	{
		return (int) 5.0;
	}

	public function testIntCastFromVariableFloat() -> int
	{
		var a = 5.0;
		return (int) a;
	}

	public function testIntCastFromBooleanTrue() -> int
	{
		return (int) true;
	}

	public function testIntCastFromBooleanFalse() -> int
	{
		return (int) false;
	}

	public function testIntCastFromVariableBooleanTrue() -> int
	{
		var a = true;
		return (int) a;
	}

	public function testIntCastFromVariableBooleanFalse() -> int
	{
		var a = false;
		return (int) a;
	}

	public function testIntCastFromVariableNull() -> int
	{
		var a = null;
		return (int) a;
	}

	public function testIntCastFromStringValue() -> int
	{
		return (int) "test";
	}

	public function testIntCastFromVariableString() -> int
	{
		string a;
		let a = "test";

		return (int) a;
	}

	public function testIntCastFromParameterString(const string a) -> int
	{
		return (int) a;
	}

	public function testIntCastFromNull() -> int
	{
		return (int) null;
	}

	public function testIntCastFromVariableEmptyArray() -> int
	{
		var a = [];
		return (int) a;
	}

	public function testIntCastFromEmptyArray() -> int
	{
		return (int) [];
	}

	public function testIntCastFromVariableArray() -> int
	{
		var a = [1, 2, 3, 4];
		return (int) a;
	}

	public function testIntCastFromArray() -> int
	{
		return (int) [1, 2, 3, 4];
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testIntCastFromStdClass() -> int
	{
		return (int) (new \StdClass);
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testIntCastFromVariableStdClass() -> int
	{
		var a;
		let a = new \StdClass;
		return (int) a;
	}

	/** To float cast */

	public function testFloatCastFromFloat()
	{
		return (float) 5.0;
	}

	public function testFloatCastFromVariableFloat()
	{
		var a = 5.0;
		return (float) a;
	}

	public function testFloatCastFromBooleanTrue()
	{
		return (float) true;
	}

	public function testFloatCastFromBooleanFalse()
	{
		return (float) false;
	}

	public function testFloatCastFromVariableBooleanTrue()
	{
		var a = true;
		return (float) a;
	}

	public function testFloatCastFromVariableBooleanFalse()
	{
		var a = false;
		return (float) a;
	}

	public function testFloatCastFromVariableNull()
	{
		var a = null;
		return (float) a;
	}

	public function testFloatCastFromNull()
	{
		return (float) null;
	}

	public function testFloatCastFromVariableEmptyArray()
	{
		var a = [];
		return (float) a;
	}

	public function testFloatCastFromEmptyArray()
	{
		return (float) [];
	}

	public function testFloatCastFromVariableArray()
	{
		var a = [1, 2, 3, 4];
		return (float) a;
	}

	public function testFloatCastFromArray()
	{
		return (float) [1, 2, 3, 4];
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testFloatCastFromStdClass()
	{
		return (float) (new \StdClass);
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testFloatCastFromVariableStdClass()
	{
		var a;
		let a = new \StdClass;
		return (float) a;
	}

	/** To boolean cast */

	public function testBooleanCastFromIntTrue1()
	{
		return (boolean) 1;
	}

	public function testBooleanCastFromIntTrue2()
	{
		return (boolean) 1000;
	}

	public function testBooleanCastFromIntFalse()
	{
		return (boolean) 0;
	}

	public function testBooleanCastFromObject()
	{
		var simpleObject;
		let simpleObject = new \StdClass();
		return (boolean) simpleObject;
	}

	public function testBooleanCastFromEmptyArray()
	{
		var a = [];
		return (boolean) a;
	}

	public function testBooleanCastFromArray()
	{
		var a = [1, 2, 3, 4];
		return (boolean) a;
	}

	public function testBooleanCastFromNull()
	{
		var a = null;
		return (boolean) a;
	}

	/** To object cast */

	public function testObjectCastFromInt()
	{
		return (object) 5;
	}

	public function testObjectCastFromFloat()
	{
		return (object) 5.0;
	}

	public function testObjectCastFromFalse()
	{
		return (object) false;
	}

	public function testObjectCastFromTrue()
	{
		return (object) true;
	}

	public function testObjectCastFromNull()
	{
		return (object) null;
	}

	public function testObjectCastFromEmptyArray()
	{
		return (object) [];
	}

	public function testObjectCastFromArray()
	{
		return (object) [1, 2, 3, 4];
	}

	public function testObjectCastFromEmptyString()
	{
		return (object) "";
	}

	public function testObjectCastFromString()
	{
		return (object) "test string";
	}
}
