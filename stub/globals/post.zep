
namespace Stub\Globals;

class Post
{
	/**
	 * Tests "Invalid superglobal for _POST"
	 *
	 * @issue https://github.com/phalcon/zephir/issues/1623
	 */
	public function hasValue(string name) -> boolean
	{
		return isset _POST[name];
	}
}
