namespace Stub\Traits;

/**
 * Trait exposed to PHP userland (zephir#504).
 */
trait Nameable
{
	protected name = "unnamed";

	protected tags = [1, 2, 3];

	protected meta = ["a": 1, "b": [2, 3]];

	public function setName(string name)
	{
		let this->name = name;
	}

	public function getName() -> string
	{
		return this->name;
	}

	public function getTags() -> array
	{
		return this->tags;
	}

	public function addTag(var tag)
	{
		let this->tags[] = tag;
	}

	public function getMeta() -> array
	{
		return this->meta;
	}
}
