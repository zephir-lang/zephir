namespace Test;

class Foo
{
	/**
	 * Adds counting behavior.
	 */
	use Counter;

	public function m() -> int
	{
		return 1;
	}
}