
/**
 * Cast tests
 */

namespace Test;

class Cast
{
	/** To int cast */

	public function testIntCastFromFloat()
	{
		return (int) 5.0;
	}

	public function testIntCastFromVariableFloat()
	{
		var a = 5.0;
		return (int) a;
	}

	public function testIntCastFromBooleanTrue()
	{
		return (int) true;
	}

	public function testIntCastFromBooleanFalse()
	{
		return (int) false;
	}

	public function testIntCastFromVariableBooleanTrue()
	{
		var a = true;
		return (int) a;
	}

	public function testIntCastFromVariableBooleanFalse()
	{
		var a = false;
		return (int) a;
	}

	public function testIntCastFromVariableNull()
	{
		var a = null;
		return (int) a;
	}

	public function testIntCastFromNull()
	{
		return (int) null;
	}

	public function testIntCastFromVariableEmptyArray()
	{
		var a = [];
		return (int) a;
	}

	public function testIntCastFromEmptyArray()
	{
		return (int) [];
	}

	public function testIntCastFromVariableArray()
	{
		var a = [1, 2, 3, 4];
		return (int) a;
	}

	public function testIntCastFromArray()
	{
		return (int) [1, 2, 3, 4];
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testIntCastFromStdClass()
	{
		return (int) (new \StdClass);
	}

	/**
	 * Notice: Object of class stdClass could not be converted to int
	 * @return int 1
	 */
	public function testIntCastFromVariableStdClass()
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
