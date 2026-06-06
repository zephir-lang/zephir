namespace Stub;

class ArrayIterator implements \Iterator
{
	protected position = 0;
	protected test;

	public function __construct()
	{
		let this->test = [
			"one",
			"two",
			"three"
		];
		let this->position = 0;
	}

	public function rewind() -> void
	{
	    let this->position = 0;
	}

	public function current() -> mixed
	{
	    return this->test[this->position];
	}

	public function key() -> mixed
	{
	    return this->position;
	}

	public function next() -> void
	{
	    let this->position++;
	}

	public function valid() -> bool
	{
	    return isset this->test[this->position];
	}
}
