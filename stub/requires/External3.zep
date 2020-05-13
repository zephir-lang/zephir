
/**
 * External3 operations
 */

namespace Stub\Requires;

class External3
{
	protected someVariable;

	public function req(var path, var requires) -> void
	{
		ob_clean();
		require path;
		requires->setContent(ob_get_contents());
	}
}
