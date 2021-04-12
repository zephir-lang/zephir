
/**
 * Control Flow
 */

namespace Stub;

class Assign
{
	protected testVar { get };

	protected myArray { get };

	protected static testVarStatic;

	public function testAssign1() -> int
	{
		int a;
		let a = 1;
		return a;
	}

	public function testAssign2() -> int
	{
		int a;
		let a = true;
		return a;
	}

	public function testAssign3() -> int
	{
		int a;
		let a = false;
		return a;
	}

	public function testAssign4() -> int
	{
		int a;
		let a = null;
		return a;
	}

	public function testAssign5() -> int
	{
		int a;
		let a = 2.0;
		return a;
	}

	public function testAssign6() -> bool
	{
		boolean a;
		let a = 1;
		return a;
	}

	public function testAssign7() -> bool
	{
		boolean a;
		let a = 1.0;
		return a;
	}

	public function testAssign8() -> bool
	{
		boolean a;
		let a = true;
		return a;
	}

	public function testAssign9() -> bool
	{
		boolean a;
		let a = false;
		return a;
	}

	public function testAssign10() -> bool
	{
		boolean a;
		let a = null;
		return a;
	}

	public function testAssign11() -> double
	{
		double a;
		let a = null;
		return a;
	}

	public function testAssign12() -> double
	{
		double a;
		let a = 4;
		return a;
	}

	public function testAssign13() -> double
	{
		double a;
		let a = false;
		return a;
	}

	public function testAssign14() -> double
	{
		double a;
		let a = true;
		return a;
	}

	public function testAssign15() -> double
	{
		double a;
		let a = 5.0;
		return a;
	}

	public function testAssign16() -> int
	{
		var a;
		let a = 1;
		return a;
	}

	public function testAssign17() -> double
	{
		var a;
		let a = 1.0;
		return a;
	}

	public function testAssign18() -> bool
	{
		var a;
		let a = false;
		return a;
	}

	public function testAssign19() -> bool
	{
		var a;
		let a = true;
		return a;
	}

	public function testAssign20() -> null
	{
		var a;
		let a = null;
		return a;
	}

	public function testAssign21() -> int
	{
		int a, b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign22() -> double
	{
		double a, b;
		let a = 1.0,
			b = a;
		return b;
	}

	public function testAssign23() -> bool
	{
		boolean a, b;
		let a = true,
			b = a;
		return b;
	}

	public function testAssign24() -> double
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign25() -> double
	{
		int a;
		double b;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign26() -> int
	{
		int b;
		double a;
		let a = 1,
			b = a;
		return b;
	}

	public function testAssign27() -> double
	{
		int b;
		double a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign28() -> bool
	{
		int b;
		boolean a;
		let b = 1,
			a = b;
		return a;
	}

	public function testAssign29() -> bool
	{
		double b;
		boolean a;
		let b = 1.0,
			a = b;
		return a;
	}

	public function testAssign30() -> int
	{
		boolean b;
		int a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign31() -> double
	{
		boolean b;
		double a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign32() -> bool
	{
		boolean b;
		var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign33() -> bool
	{
		boolean b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign34() -> bool
	{
		int b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign35() -> bool
	{
		double b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign36() -> bool
	{
		var b; var a;
		let b = false,
			a = b;
		return a;
	}

	public function testAssign37() -> array
	{
		var v = "abc";
		var arr = [];

		let arr = [
			"a": ["b_key": "b_val", "b": []]
		];

		let arr["a"]["b"]["d_key"] = "d_val";
		let arr["s"] = 1;
		let arr["a"]["b"]["c"]["d"]["e"] = "f";
		let arr[1] = [
			2: [3: 4]
		];

		let arr[1][2][5] = 6;
		let arr[1][2][v] = v;
		return arr;
	}

	public function testAssign38(index) -> array
	{
		var arr = [];
		let arr[index] = "val";
		return arr;
	}

	public function testAssign39() -> int
	{
		var a, b, c;

		// Initialize variables
		let a = "hello", b = false;

		// Change their values
		let a = 10, b = "140";

		// Perform operations between them
		let c = a + b;

		return c;
	}

	public function testAssign40() -> int
	{
		var a, b;

		// Initialize variables
		let b = new \stdClass(), a = [];

		// Change their values
		let a = "123", b = 7;

		// Perform operations between them
		return a - b;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1573
	 */
	public function testAssign41(int num) -> int
	{
		var a;
		let a = 42;
		let a /= num;

		return a;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1573
	 */
	public function testAssign42(int num) -> int
	{
		var a;
		let a = 2;
		let num /= a;

		return num;
	}

	public function testAssign43(int num) -> int
	{
		var a;
		let a = 43;
		let a %= num;

		return a;
	}

	public function testAssign44(int num) -> int
	{
		var a;
		let a = 2;
		let num %= a;

		return num;
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

	public function testPropertyArray12(index)
	{
		var temp1, temp2, temp3;

		let this->myArray[index][index][] = 1;
		let this->myArray[index][index][] = 1.5;
		let this->myArray[index][index][] = null;
		let this->myArray[index][index][] = false;
		let temp1 = this->myArray[index], temp2 = this->myArray;
		let this->myArray[index][index][] = true;
		let this->myArray[index][index][] = 'A';
		let this->myArray[index][index][] = "hello";
		let this->myArray[index][index][] = [];
		let temp3 = count(this->myArray);
		return this->myArray;
	}

	public function testPropertyArray13(index)
	{
		var temp1, temp2, temp3;

		let this->myArray[index][] = 1;
		let this->myArray[index][] = 1.5;
		let this->myArray[index][] = null;
		let this->myArray[index][] = false;
		let temp1 = this->myArray[index], temp2 = this->myArray;
		let this->myArray[index][] = true;
		let this->myArray[index][] = 'A';
		let this->myArray[index][] = "hello";
		let this->myArray[index][] = [];
		let temp3 = count(this->myArray);
		return this->myArray;
	}

	public function testPropertyArray14()
	{
		var v = "abc";
		let this->myArray = [
			"a": ["b_key": "b_val"]
		];
		let this->myArray["a"]["b"]["d_key"] = "d_val";
		let this->myArray[1] = [
			2: [3: 4]
		];

		let this->myArray["s"] = 1;
		let this->myArray["a"]["b"]["c"]["d"]["e"] = "f";
		let this->myArray[1][2][5] = 6;
		let this->myArray[1][2][v] = v;
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
		let self::testVarStatic = [1, 2, 3];
		return self::testVarStatic;
	}

	public function testStaticPropertyAssign2()
	{
		int a = 1;
		double b = 1.5;
		var c = null;
		boolean d = false;
		char e = 'A';
		array f = [];

		let self::testVarStatic = a;
		let self::testVarStatic = b;
		let self::testVarStatic = c;
		let self::testVarStatic = d;
		let self::testVarStatic = e;
		let self::testVarStatic = f;
		return self::testVarStatic;
	}

	public function testStaticPropertyArray1()
	{
		let self::testVarStatic   = [];
		let self::testVarStatic[0] = 1;
		let self::testVarStatic[0] = 1.5;
		let self::testVarStatic[0] = null;
		let self::testVarStatic[0] = false;
		let self::testVarStatic[0] = true;
		let self::testVarStatic[0] = 'A';
		let self::testVarStatic[0] = "hello";
		let self::testVarStatic[0] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArray2()
	{
		let self::testVarStatic   = [];
		let self::testVarStatic["hello"] = 1;
		let self::testVarStatic["hello"] = 1.5;
		let self::testVarStatic["hello"] = null;
		let self::testVarStatic["hello"] = false;
		let self::testVarStatic["hello"] = true;
		let self::testVarStatic["hello"] = 'A';
		let self::testVarStatic["hello"] = "hello";
		let self::testVarStatic["hello"] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArray3(index)
	{
		let self::testVarStatic   = [];
		let self::testVarStatic[index] = 1;
		let self::testVarStatic[index] = 1.5;
		let self::testVarStatic[index] = null;
		let self::testVarStatic[index] = false;
		let self::testVarStatic[index] = true;
		let self::testVarStatic[index] = 'A';
		let self::testVarStatic[index] = "hello";
		let self::testVarStatic[index] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayAppend()
	{
		let self::testVarStatic = [];
		let self::testVarStatic[] = "test";
		let self::testVarStatic[] = 1;
		let self::testVarStatic[] = 1.5;
		let self::testVarStatic[] = false;
		let self::testVarStatic[] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayMutli1()
	{
		let self::testVarStatic       = [];
		let self::testVarStatic[0][0] = 1;
		let self::testVarStatic[0][0] = 1.5;
		let self::testVarStatic[0][0] = null;
		let self::testVarStatic[0][0] = false;
		let self::testVarStatic[0][0] = true;
		let self::testVarStatic[0][0] = 'A';
		let self::testVarStatic[0][0] = "hello";
		let self::testVarStatic[0][0] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayMutli2()
	{
		let self::testVarStatic                   = [];
		let self::testVarStatic["hello"]["hello"] = 1;
		let self::testVarStatic["hello"]["hello"] = 1.5;
		let self::testVarStatic["hello"]["hello"] = null;
		let self::testVarStatic["hello"]["hello"] = false;
		let self::testVarStatic["hello"]["hello"] = true;
		let self::testVarStatic["hello"]["hello"] = 'A';
		let self::testVarStatic["hello"]["hello"] = "hello";
		let self::testVarStatic["hello"]["hello"] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayMutli3(index)
	{
		let self::testVarStatic               = [];
		let self::testVarStatic[index][index] = 1;
		let self::testVarStatic[index][index] = 1.5;
		let self::testVarStatic[index][index] = null;
		let self::testVarStatic[index][index] = false;
		let self::testVarStatic[index][index] = true;
		let self::testVarStatic[index][index] = 'A';
		let self::testVarStatic[index][index] = "hello";
		let self::testVarStatic[index][index] = [];
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayMulti4()
	{
		var v = "abc";
		let self::testVarStatic = [
			"a": ["b_key": "b_val"]
		];
		let self::testVarStatic["a"]["b"]["d_key"] = "d_val";
		let self::testVarStatic[1] = [
			2: [3: 4]
		];

		let self::testVarStatic["s"] = 1;
		let self::testVarStatic["a"]["b"]["c"]["d"]["e"] = "f";
		let self::testVarStatic[1][2][5] = 6;
		let self::testVarStatic[1][2][v] = v;
		return self::testVarStatic;
	}

	public function testStaticPropertyArrayAppend1()
	{
		let self::testVarStatic      = [];
		let self::testVarStatic[0][] = 1;
		let self::testVarStatic[0][] = 1.5;
		let self::testVarStatic[0][] = null;
		let self::testVarStatic[0][] = false;
		let self::testVarStatic[0][] = true;
		let self::testVarStatic[0][] = 'A';
		let self::testVarStatic[0][] = "hello";
		let self::testVarStatic[0][] = [];
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
	 * @link https://github.com/zephir-lang/zephir/issues/159
	 */
	public function testGlobalVarAssign(var index, var value)
	{
		let _POST[index] = value;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/523
	 */
	public function testConstantKeyAssign()
	{
		var elements;
		let elements = ["abc": 1, ABDAY_1: DAY_1, ABDAY_2: DAY_2];
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/746
	 *
	 */
	public function testArrayBoolExpressionAssign()
	{
		var str = "abc";
		let this->myArray = [];
		let this->myArray["a"] = str == "abc";
		let this->myArray["b"] = str != "abc";
		return this->myArray;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/725
	 */
	public function testAssignSuperGlobals()
	{
		var v = "stest2";
		let _GET["steststr"] = "stest";
		let _GET["steststr2"] = v;
		let _GET["stestint"] = 1;
		let _GET["stestint2"] = 2;
		let _GET[v] = "testval";
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1917
	 */
	public function testAssignSuperGlobalsSERVER()
	{
	    let _SERVER = array_merge(_SERVER, ["g1": "aaa", "g2": "bbb"]);
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1917
	 */
	public function testAssignSuperGlobalsGET()
	{
	    let _GET = array_merge(_GET, ["g1": "aaa", "g2": "bbb"]);
	}

	public function issue597()
	{
	    if isset _POST["a"] {
            if isset _GET["r"] {
                // Nothing here
            }
        }

        if isset _GET["s"] {
            var s;
            let s = _GET["s"] * 5;
            let _GET["s"] = s;

            return s;
        }
	}
}
