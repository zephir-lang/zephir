namespace Stub\Traits;

/**
 * Zephir class consuming a Zephir trait (zephir#504).
 */
class CounterUser
{
	use Counter;

	public function label() -> string
	{
		return "c";
	}
}
