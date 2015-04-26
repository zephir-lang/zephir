
namespace Test;

/**
 * Sample exception class
 */
class Exception extends \Exception
{
	/**
	 * @link https://github.com/phalcon/zephir/issues/915
	 */
	public function testRuntimePropertyFetch(string! message)
	{
		let this->message = message;
	}
}
