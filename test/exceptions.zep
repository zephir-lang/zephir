
namespace Test;

class Exceptions
{
	public function testException1()
	{
		throw new Test\Exception("hello1");
	}

	public function testException2()
	{
		var msg;
		let msg = "hello2";
		throw new Test\Exception(msg);
	}

	public function testException3()
	{
		var ex, msg;
		let msg = "hello3";
		let ex = new Test\Exception(msg);
		throw ex;
	}

	protected function getException()
	{
		return new Test\Exception("hello4");
	}

	public function testException4()
	{
		throw this->getException();
	}
}

