
namespace Test;

class Issues
{
	protected adapter;

	public function setAdapter(var adapter)
	{
		let this->adapter = adapter;
	}

	public function someMethod(var methodName)
	{
		return this->adapter->{methodName}();
	}

	public function test()
	{
		echo "test";
	}
}
