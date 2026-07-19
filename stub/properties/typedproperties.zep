
namespace Stub\Properties;

/**
 * Typed class properties (issue #2608).
 */
class TypedProperties
{
	public array orgs = [];

	protected bool valid = false;

	public int counter = 7;

	public double ratio = 1.5;

	public string label = "hi";

	private ?string val = "";

	public ?int maybeNum = null;

	public <PublicProperties> friend;

	public ?<PublicProperties> maybeFriend = null;

	public function getVal() -> string | null
	{
		return this->val;
	}

	public function getMaybeNum() -> int | null
	{
		return this->maybeNum;
	}
}
