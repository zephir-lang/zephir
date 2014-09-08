
namespace Test;

class Exceptions
{
	public function testException1()
	{
		throw new Exception("hello1");
	}

	public function testExceptionStringEscape()
	{
		throw new Exception("hello \"simple code\" test");
	}

	public function testException2()
	{
		var msg;
		let msg = "hello2";
		throw new Exception(msg);
	}

	public function testException3()
	{
		var ex, msg;
		let msg = "hello3";
		let ex = new Exception(msg);
		throw ex;
	}

	protected function getException()
	{
		return new Exception("hello4");
	}

	public function testException4()
	{
		throw this->getException();
	}
}

