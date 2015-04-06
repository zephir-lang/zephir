
/**
 * Method calls
 */

namespace Test;

class McallDynamic
{
	public function testMethod1()
	{
		return 1;
	}

	public function testMagicCall1()
	{
		return this->{"method1"}() + this->{"METHOD1"}();
	}

	public function __call(method, arguments)
	{
		var realMethod;
		let realMethod = "test" . method;
		return this->{realMethod}();
	}
}
