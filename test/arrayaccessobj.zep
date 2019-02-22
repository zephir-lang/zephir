namespace Test;

class ArrayAccessObj implements \ArrayAccess
{
	protected test;

	public function __construct() {
		let this->test = [
			"one":1,
			"two":2,
			"three":3
		];
	}
	
	public function offsetSet(offset, value)
	{
		if is_null(offset) {
			let this->test[] = value;
		} else {
			let this->test[offset] = value;
		}
	}
	
	public function offsetExists(offset) {
		return isset this->test[offset];
	}
	
	public function offsetUnset(offset) {
		unset this->test[offset];
	}
	
	public function offsetGet(offset) {
		return isset this->test[offset] ? this->test[offset] : null;
	}
}
