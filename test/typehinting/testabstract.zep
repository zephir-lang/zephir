namespace Test\TypeHinting;

abstract class TestAbstract
{
	abstract public function testFunc(array text = [], string text2 = "", boolean flag = true, int optional = null);
}
