namespace Stub\Traits;

/**
 * Trait exposed to PHP userland (zephir#504).
 */
trait Nameable
{
	protected name = "unnamed";

	public function setName(string name)
	{
		let this->name = name;
	}

	public function getName() -> string
	{
		return this->name;
	}
}
