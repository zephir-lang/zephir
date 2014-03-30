
/**
 * Control Flow
 */

namespace Test;

class Assign
{
	protected testVar {get};

	protected myArray {get};

	protected static testVarStatic;

	public function testAssign1()
	{
		int a;
		let a = 1;
		return a;
	}

	public function testAssign2()
	{
		int a;
		let a = true;
		return a;
	}

	public function testAssign3()
	{
		int a;
		let a = false;
		return a;
	}

	public function testAssign4()
	{
		int a;
		let a = null;
		return a;
	}

	public function testAssign5()
	{
		int a;
		let a = 2.0;
		return a;
	}

	public function testAssign6()
	{
		boolean a;
		let a = 1;
		return a;
	}

	public function testAssign7()
	{
		boolean a;
		let a = 1.0;
		return a;
	}

	public function testAssign8()
	{
		boolean a;
		let a = true;
		return a;
	}

	public function testAssign9()
	{
		boolean a;
		let a = false;
		return a;
	}

	public function testAssign10()
	{
		boolean a;
		let a = null;
		return a;
	}

	public function testAssign11()
	{
		double a;
		let a = null;
		return a;
	}

	public function testAssign12()
	{
		double a;
		let a = 4;
		return a;
	}

	public function testAssign13()
	{
		double a;
		let a = false;
		return a;
	}

	public function testAssign14()
	{
		double a;
		let a = true;
		return a;
	}

	public function testAssign15()
	{
		double a;
		let a = 5.0;
		return a;
	}

	public function testAssign16()
	{
		var a;
		let a = 1;
		return a;
	}

	public function testAssign17()
	{
		var a;
		let a = 1.0;
		return a;
	}

	public function testAssign18()
	{
		var a;
		let a = false;
		return a;
	}

	public function testAssign19()
	{
		var a;
		let a = true;
		return a;
	}

	public function testAssign20()
	{
		var a;
		let a = null;
		return a;
	}

	public function testAssign21()
	{
		int a, b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign22()
	{
		double a, b;
		let a = 1.0,
			b = a;
		return b;
	}

	public function testAssign23()
	{
		boolean a, b;
		let a = true,
			b = a;
		return b;
	}

	public function testAssign24()
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign25()
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign26()
	{
		int b;
		double a;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign27()
	{
		int b;
		double a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign28()
	{
		int b;
		boolean a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign29()
	{
		double b;
		boolean a;
		let b = 1.0,
			a = b;
		return a;
	}

	public function testAssign30()
	{
		boolean b;
		int a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign31()
	{
		boolean b;
		double a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign32()
	{
		boolean b;
		var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign33()
	{
		boolean b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign34()
	{
		int b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign35()
	{
		double b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign36()
	{
		var b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testPropertyAssign1()
	{
		let this->testVar = 1;
		let this->testVar = 1.5;
		let this->testVar = null;
		let this->testVar = false;
		let this->testVar = true;
		let this->testVar = 'A';
		let this->testVar = "hello";
		let this->testVar = [];
		return this->testVar;
	}

	public function testPropertyAssign2()
	{
		int a = 1;
		double b = 1.5;
		var c = null;
		boolean d = false;
		char e = 'A';
		array f = [];

		let this->testVar = a;
		let this->testVar = b;
		let this->testVar = c;
		let this->testVar = d;
		let this->testVar = e;
		let this->testVar = f;
		return this->testVar;
	}

	public function testPropertyIncr1()
	{
		let this->testVar = 1;
		let this->testVar++;
		return this->testVar;
	}

	public function testPropertyAddAssign1()
	{
		let this->testVar = 0;
		let this->testVar += 2;
		return this->testVar;
	}

	public function testPropertyAddAssign2()
	{
		let this->testVar = 1;
		let this->testVar += 2;
		return this->testVar;
	}

	public function testPropertyAssignValuePlus1()
	{
		let this->testVar = 1;
		let this->testVar = this->testVar + 1;
		return this->testVar;
	}

	public function testPropertyDecr()
	{
		let this->testVar = 2;
		let this->testVar--;
		return this->testVar;
	}

	public function testPropertySubAssign1()
	{
		let this->testVar = 0;
		let this->testVar -= 2;
		return this->testVar;
	}

	public function testPropertySubAssign2()
	{
		let this->testVar = 1;
		let this->testVar -= 2;
		return this->testVar;
	}

	public function testPropertyMulAssign1()
	{
		let this->testVar = 1;
		let this->testVar *= 2;
		return this->testVar;
	}

	public function testPropertyMulAssign2()
	{
		let this->testVar = 1;
		let this->testVar *= 3;
		return this->testVar;
	}

	public function testPropertyAssignStringConcat()
	{
		let this->testVar = "test";
		let this->testVar .= " string";
		return this->testVar;
	}

	public function testPropertyArray1()
	{
		let this->myArray = [];
		let this->myArray[] = 1;
		let this->myArray[] = 1.5;
		let this->myArray[] = null;
		let this->myArray[] = false;
		let this->myArray[] = true;
		let this->myArray[] = 'A';
		let this->myArray[] = "hello";
		let this->myArray[] = [];
		return this->myArray;
	}

	public function testPropertyArray2()
	{
		int a = 1;
		double b = 1.5;
		var c = null;
		boolean d = false;
		char e = 'A';
		array f = [];

		let this->myArray = [];
		let this->myArray[] = a;
		let this->myArray[] = b;
		let this->myArray[] = c;
		let this->myArray[] = d;
		let this->myArray[] = e;
		let this->myArray[] = f;
		return this->myArray;
	}

	public function testPropertyArray3()
	{
		let this->myArray = [];
		let this->myArray[0] = 1;
		let this->myArray[0] = 1.5;
		let this->myArray[0] = null;
		let this->myArray[0] = false;
		let this->myArray[0] = true;
		let this->myArray[0] = 'A';
		let this->myArray[0] = [];
		return this->myArray;
	}

	public function testPropertyArray4(index)
	{
		let this->myArray = [];
		let this->myArray[index] = 1;
		let this->myArray[index] = 1.5;
		let this->myArray[index] = null;
		let this->myArray[index] = false;
		let this->myArray[index] = true;
		let this->myArray[index] = 'A';
		let this->myArray[index] = [];
		return this->myArray;
	}

	public function testPropertyArray5(string index)
	{
		let this->myArray = [];
		let this->myArray[index] = 1;
		let this->myArray[index] = 1.5;
		let this->myArray[index] = null;
		let this->myArray[index] = false;
		let this->myArray[index] = true;
		let this->myArray[index] = 'A';
		let this->myArray[index] = [];
		return this->myArray;
	}

	public function testPropertyArray6()
	{
		let this->myArray = [];
		let this->myArray[0][1] = 1;
		let this->myArray[0][1] = 1.5;
		let this->myArray[0][1] = null;
		let this->myArray[0][1] = false;
		let this->myArray[0][1] = true;
		let this->myArray[0][1] = 'A';
		let this->myArray[0][1] = "hello";
		let this->myArray[0][1] = [];
		return this->myArray;
	}

	public function testPropertyArray7()
	{
		let this->myArray = [];
		let this->myArray["hello"]["hello"] = 1;
		let this->myArray["hello"]["hello"] = 1.5;
		let this->myArray["hello"]["hello"] = null;
		let this->myArray["hello"]["hello"] = false;
		let this->myArray["hello"]["hello"] = true;
		let this->myArray["hello"]["hello"] = 'A';
		let this->myArray["hello"]["hello"] = "hello";
		let this->myArray["hello"]["hello"] = [];
		return this->myArray;
	}

	public function testPropertyArray8(index)
	{
		let this->myArray = [];
		let this->myArray[index][index] = 1;
		let this->myArray[index][index] = 1.5;
		let this->myArray[index][index] = null;
		let this->myArray[index][index] = false;
		let this->myArray[index][index] = true;
		let this->myArray[index][index] = 'A';
		let this->myArray[index][index] = "hello";
		let this->myArray[index][index] = [];
		return this->myArray;
	}

	public function testPropertyArray9(int index)
	{
		let this->myArray = [];
		let this->myArray[index][index] = 1;
		let this->myArray[index][index] = 1.5;
		let this->myArray[index][index] = null;
		let this->myArray[index][index] = false;
		let this->myArray[index][index] = true;
		let this->myArray[index][index] = 'A';
		let this->myArray[index][index] = "hello";
		let this->myArray[index][index] = [];
		return this->myArray;
	}

	public function testPropertyArray10(string index)
	{
		let this->myArray = [];
		let this->myArray[index][index] = 1;
		let this->myArray[index][index] = 1.5;
		let this->myArray[index][index] = null;
		let this->myArray[index][index] = false;
		let this->myArray[index][index] = true;
		let this->myArray[index][index] = 'A';
		let this->myArray[index][index] = "hello";
		let this->myArray[index][index] = [];
		return this->myArray;
	}

	public function testPropertyArray11(index)
	{
		let this->myArray[index][index][] = 1;
		let this->myArray[index][index][] = 1.5;
		let this->myArray[index][index][] = null;
		let this->myArray[index][index][] = false;
		let this->myArray[index][index][] = true;
		let this->myArray[index][index][] = 'A';
		let this->myArray[index][index][] = "hello";
		let this->myArray[index][index][] = [];
		return this->myArray;
	}

	public function testStaticPropertyAssign1()
	{
		let self::testVarStatic = 1;
		let self::testVarStatic = 1.5;
		let self::testVarStatic = null;
		let self::testVarStatic = false;
		let self::testVarStatic = true;
		let self::testVarStatic = 'A';
		let self::testVarStatic = "hello";
		let self::testVarStatic = [];
		return self::testVarStatic;
	}

	public function testArrayVarAssign1(var index, var value)
	{
		var a;
		let a = [];
		let a[index] = value;
		return a;
	}

	public function testArrayVarAssign2(var index, var value)
	{
		var _POST;
		let _POST = [];
		let _POST[index] = value;
		return _POST;
	}

	public function testArrayProperty(var index, var value)
	{
		var _POST;
		let _POST = [];
		let _POST[index] = value;
		return _POST;
	}

	/**
	 * @link https://github.com/phalcon/zephir/issues/159
	 */
	public function testGlobalVarAssign(var index, var value)
	{
		let _POST[index] = value;
	}
}