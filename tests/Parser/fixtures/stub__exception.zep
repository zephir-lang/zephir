
namespace Stub;

/**
 * Sample exception class
 */
class Exception extends \Exception
{
	/**
	 * @link https://github.com/zephir-lang/zephir/issues/915
	 */
	public function testRuntimePropertyFetch(string! message)
	{
		let this->message = message;
	}
}
