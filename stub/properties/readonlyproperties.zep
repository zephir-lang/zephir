
namespace Stub\Properties;

/**
 * Readonly class properties (issue #2614). A readonly property must be typed,
 * has no default value and is initialized exactly once (here, in the
 * constructor). The engine enforces write-once on PHP 8.1+.
 */
class ReadonlyProperties
{
	public readonly int id;

	public readonly string label;

	public readonly array tags;

	protected readonly bool flag;

	public readonly <PublicProperties> friend;

	public function __construct()
	{
		let this->id = 42;
		let this->label = "hello";
		let this->tags = ["a", "b"];
		let this->flag = true;
		let this->friend = new PublicProperties();
	}

	public function getFlag() -> bool
	{
		return this->flag;
	}
}
