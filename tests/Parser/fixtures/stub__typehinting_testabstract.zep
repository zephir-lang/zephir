namespace Stub\TypeHinting;

abstract class TestAbstract
{
	abstract public function testFunc(array text = [], string text2 = "", boolean flag = true, int optional = null);
	abstract public function returnOneOfScalar() -> int | uint | long | float | double | string | boolean | char;
	abstract public function returnInt() -> int;
	abstract public function returnUint() -> uint;
	abstract public function returnLong() -> long;
	abstract public function returnFloat() -> float;
	abstract public function returnDouble() -> double;
	abstract public function returnString() -> string;
	abstract public function returnBoolean() -> boolean;
	abstract public function returnChar() -> char;
}
