
namespace Test\Globals;

class Post
{
	/**
	 * Tests "Invalid superglobal for _POST"
	 *
	 * @issue 1623
	 */
	public function hasValue(string name) -> boolean
	{
		return isset _POST[name];
	}
}
