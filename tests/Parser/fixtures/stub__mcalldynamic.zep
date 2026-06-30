
/**
 * Method calls
 */

namespace Stub;


use Stub\Mcall\Caller;

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

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1751
	 */
	public function testCallAnonymousFunctionWithContext() -> var
	{
		var result;

		let result = Caller::start(function () {
			 return Caller::perform();
		});

		return result;
	}
}
