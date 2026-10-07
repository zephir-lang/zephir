namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * Every arithmetic operator with an operand that is not a native number:
 * a string or array local, or a string, null or array literal, which PHP
 * coerces or rejects with TypeError. The bool shapes pin `+ - *` to the
 * integer 0 or 1, as `/` and `%` already are. The test compares each one
 * against plain PHP.
 */
class Issue2676Operands
{
	public function addStringLong(string a, long b)
	{
		return a + b;
	}

	public function addLongString(long a, string b)
	{
		return a + b;
	}

	public function addStringDouble(string a, double b)
	{
		return a + b;
	}

	public function addStringBool(string a, bool b)
	{
		return a + b;
	}

	public function addStringVar(string a, var b)
	{
		return a + b;
	}

	public function addStringString(string a, string b)
	{
		return a + b;
	}

	public function addArrayLong(array a, long b)
	{
		return a + b;
	}

	public function addArrayVar(array a, var b)
	{
		return a + b;
	}

	public function addLiteralString(var b)
	{
		return "10" + b;
	}

	public function addLiteralAbc(var b)
	{
		return "abc" + b;
	}

	public function addLiteralLong(long b)
	{
		return "10" + b;
	}

	public function addNullVar(var b)
	{
		return null + b;
	}

	public function addNullLong(long b)
	{
		return null + b;
	}

	public function addLiteralArray(var b)
	{
		return [1] + b;
	}

	public function addTrueLong(long b)
	{
		return true + b;
	}

	public function addLongTrue(long a)
	{
		return a + true;
	}

	public function addBoolLong(bool a, long b)
	{
		return a + b;
	}

	public function addBoolBool(bool a, bool b)
	{
		return a + b;
	}

	public function addDoubleBool(double a, bool b)
	{
		return a + b;
	}

	public function addBoolDouble(bool a, double b)
	{
		return a + b;
	}

	public function addDoubleTrue(double a)
	{
		return a + true;
	}

	public function addLongLiteralDouble(long a)
	{
		return a + 1.5;
	}

	public function addInferredLocal(string a, long b)
	{
		var x = a + b;

		return x;
	}

	public function addAssignVarLiteralString(var a)
	{
		var x;

		let x = a;
		let x += "10";

		return x;
	}

	public function addAssignVarString(var a, string s)
	{
		var x;

		let x = a;
		let x += s;

		return x;
	}

	public function addAssignVarNull(var a)
	{
		var x;

		let x = a;
		let x += null;

		return x;
	}

	public function addAssignPropertyLiteralString(var obj)
	{
		let obj->value += "10";

		return obj->value;
	}

	public function addAssignPropertyNull(var obj)
	{
		let obj->value += null;

		return obj->value;
	}

	public function subStringLong(string a, long b)
	{
		return a - b;
	}

	public function subLongString(long a, string b)
	{
		return a - b;
	}

	public function subStringDouble(string a, double b)
	{
		return a - b;
	}

	public function subStringBool(string a, bool b)
	{
		return a - b;
	}

	public function subStringVar(string a, var b)
	{
		return a - b;
	}

	public function subStringString(string a, string b)
	{
		return a - b;
	}

	public function subArrayLong(array a, long b)
	{
		return a - b;
	}

	public function subArrayVar(array a, var b)
	{
		return a - b;
	}

	public function subLiteralString(var b)
	{
		return "10" - b;
	}

	public function subLiteralAbc(var b)
	{
		return "abc" - b;
	}

	public function subLiteralLong(long b)
	{
		return "10" - b;
	}

	public function subNullVar(var b)
	{
		return null - b;
	}

	public function subNullLong(long b)
	{
		return null - b;
	}

	public function subLiteralArray(var b)
	{
		return [1] - b;
	}

	public function subTrueLong(long b)
	{
		return true - b;
	}

	public function subLongTrue(long a)
	{
		return a - true;
	}

	public function subBoolLong(bool a, long b)
	{
		return a - b;
	}

	public function subBoolBool(bool a, bool b)
	{
		return a - b;
	}

	public function subDoubleBool(double a, bool b)
	{
		return a - b;
	}

	public function subBoolDouble(bool a, double b)
	{
		return a - b;
	}

	public function subDoubleTrue(double a)
	{
		return a - true;
	}

	public function subLongLiteralDouble(long a)
	{
		return a - 1.5;
	}

	public function subInferredLocal(string a, long b)
	{
		var x = a - b;

		return x;
	}

	public function subAssignVarLiteralString(var a)
	{
		var x;

		let x = a;
		let x -= "10";

		return x;
	}

	public function subAssignVarString(var a, string s)
	{
		var x;

		let x = a;
		let x -= s;

		return x;
	}

	public function subAssignVarNull(var a)
	{
		var x;

		let x = a;
		let x -= null;

		return x;
	}

	public function subAssignPropertyLiteralString(var obj)
	{
		let obj->value -= "10";

		return obj->value;
	}

	public function subAssignPropertyNull(var obj)
	{
		let obj->value -= null;

		return obj->value;
	}

	public function mulStringLong(string a, long b)
	{
		return a * b;
	}

	public function mulLongString(long a, string b)
	{
		return a * b;
	}

	public function mulStringDouble(string a, double b)
	{
		return a * b;
	}

	public function mulStringBool(string a, bool b)
	{
		return a * b;
	}

	public function mulStringVar(string a, var b)
	{
		return a * b;
	}

	public function mulStringString(string a, string b)
	{
		return a * b;
	}

	public function mulArrayLong(array a, long b)
	{
		return a * b;
	}

	public function mulArrayVar(array a, var b)
	{
		return a * b;
	}

	public function mulLiteralString(var b)
	{
		return "10" * b;
	}

	public function mulLiteralAbc(var b)
	{
		return "abc" * b;
	}

	public function mulLiteralLong(long b)
	{
		return "10" * b;
	}

	public function mulNullVar(var b)
	{
		return null * b;
	}

	public function mulNullLong(long b)
	{
		return null * b;
	}

	public function mulLiteralArray(var b)
	{
		return [1] * b;
	}

	public function mulTrueLong(long b)
	{
		return true * b;
	}

	public function mulLongTrue(long a)
	{
		return a * true;
	}

	public function mulBoolLong(bool a, long b)
	{
		return a * b;
	}

	public function mulBoolBool(bool a, bool b)
	{
		return a * b;
	}

	public function mulDoubleBool(double a, bool b)
	{
		return a * b;
	}

	public function mulBoolDouble(bool a, double b)
	{
		return a * b;
	}

	public function mulDoubleTrue(double a)
	{
		return a * true;
	}

	public function mulLongLiteralDouble(long a)
	{
		return a * 1.5;
	}

	public function mulInferredLocal(string a, long b)
	{
		var x = a * b;

		return x;
	}

	public function mulAssignVarLiteralString(var a)
	{
		var x;

		let x = a;
		let x *= "10";

		return x;
	}

	public function mulAssignVarString(var a, string s)
	{
		var x;

		let x = a;
		let x *= s;

		return x;
	}

	public function mulAssignVarNull(var a)
	{
		var x;

		let x = a;
		let x *= null;

		return x;
	}

	public function mulAssignPropertyLiteralString(var obj)
	{
		let obj->value *= "10";

		return obj->value;
	}

	public function mulAssignPropertyNull(var obj)
	{
		let obj->value *= null;

		return obj->value;
	}

	public function divStringLong(string a, long b)
	{
		return a / b;
	}

	public function divLongString(long a, string b)
	{
		return a / b;
	}

	public function divStringDouble(string a, double b)
	{
		return a / b;
	}

	public function divStringBool(string a, bool b)
	{
		return a / b;
	}

	public function divStringVar(string a, var b)
	{
		return a / b;
	}

	public function divStringString(string a, string b)
	{
		return a / b;
	}

	public function divArrayLong(array a, long b)
	{
		return a / b;
	}

	public function divArrayVar(array a, var b)
	{
		return a / b;
	}

	public function divLiteralString(var b)
	{
		return "10" / b;
	}

	public function divLiteralAbc(var b)
	{
		return "abc" / b;
	}

	public function divLiteralLong(long b)
	{
		return "10" / b;
	}

	public function divNullVar(var b)
	{
		return null / b;
	}

	public function divNullLong(long b)
	{
		return null / b;
	}

	public function divLiteralArray(var b)
	{
		return [1] / b;
	}

	public function divInferredLocal(string a, long b)
	{
		var x = a / b;

		return x;
	}

	public function divAssignVarLiteralString(var a)
	{
		var x;

		let x = a;
		let x /= "10";

		return x;
	}

	public function divAssignVarString(var a, string s)
	{
		var x;

		let x = a;
		let x /= s;

		return x;
	}

	public function divAssignVarNull(var a)
	{
		var x;

		let x = a;
		let x /= null;

		return x;
	}

	public function divAssignPropertyLiteralString(var obj)
	{
		let obj->value /= "10";

		return obj->value;
	}

	public function divAssignPropertyNull(var obj)
	{
		let obj->value /= null;

		return obj->value;
	}

	public function modStringLong(string a, long b)
	{
		return a % b;
	}

	public function modLongString(long a, string b)
	{
		return a % b;
	}

	public function modStringDouble(string a, double b)
	{
		return a % b;
	}

	public function modStringBool(string a, bool b)
	{
		return a % b;
	}

	public function modStringVar(string a, var b)
	{
		return a % b;
	}

	public function modStringString(string a, string b)
	{
		return a % b;
	}

	public function modArrayLong(array a, long b)
	{
		return a % b;
	}

	public function modArrayVar(array a, var b)
	{
		return a % b;
	}

	public function modLiteralString(var b)
	{
		return "10" % b;
	}

	public function modLiteralAbc(var b)
	{
		return "abc" % b;
	}

	public function modLiteralLong(long b)
	{
		return "10" % b;
	}

	public function modNullVar(var b)
	{
		return null % b;
	}

	public function modNullLong(long b)
	{
		return null % b;
	}

	public function modLiteralArray(var b)
	{
		return [1] % b;
	}

	public function modInferredLocal(string a, long b)
	{
		var x = a % b;

		return x;
	}

	public function modAssignVarLiteralString(var a)
	{
		var x;

		let x = a;
		let x %= "10";

		return x;
	}

	public function modAssignVarString(var a, string s)
	{
		var x;

		let x = a;
		let x %= s;

		return x;
	}

	public function modAssignVarNull(var a)
	{
		var x;

		let x = a;
		let x %= null;

		return x;
	}

	public function modAssignPropertyLiteralString(var obj)
	{
		let obj->value %= "10";

		return obj->value;
	}

	public function modAssignPropertyNull(var obj)
	{
		let obj->value %= null;

		return obj->value;
	}
}
