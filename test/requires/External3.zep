
/**
 * External3 operations
 */

namespace Test\Requires;

class External3
{
	protected someVariable;

	public function req(var path, var requires)
	{
		ob_clean();
		require path;
		requires->setContent(ob_get_contents());
		ob_clean();
	}
}
