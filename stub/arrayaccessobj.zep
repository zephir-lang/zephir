namespace Stub;

class ArrayAccessObj implements \ArrayAccess
{
	protected test;

	public function __construct()
	{
		let this->test = [
			"one":1,
			"two":2,
			"three":3
		];
	}

	public function offsetSet(mixed offset, mixed value) -> void
	{
		if is_null(offset) {
			let this->test[] = value;
		} else {
			let this->test[offset] = value;
		}
	}

	public function offsetExists(mixed offset) -> bool
	{
		return isset this->test[offset];
	}

	public function offsetUnset(mixed offset) -> void
	{
		unset this->test[offset];
	}

	public function offsetGet(mixed offset) -> mixed
	{
		return isset this->test[offset] ? this->test[offset] : null;
	}
}
