namespace Stub\Traits;

/**
 * Trait with property, constant, static and abstract members (zephir#504).
 */
trait Counter
{
	protected count = 0;

	protected steps = [10, 20];

	const STEP = 2;

	public function increment() -> int
	{
		let this->count = this->count + self::STEP;
		return this->count;
	}

	public static function describe() -> string
	{
		return "counter";
	}

	abstract public function label() -> string;

	public function tagged() -> string
	{
		return this->label() . ":" . strval(this->count);
	}

	public function getSteps() -> array
	{
		return this->steps;
	}
}
