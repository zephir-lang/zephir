namespace Stub;

class ArrayIterator implements \Iterator
{
	protected position = 0;
	protected test;

	public function __construct() {
		let this->test = [
			"one",
			"two",
			"three"
		];
		let this->position = 0;
	}

	public function rewind() {
	    let this->position = 0;
	}

	public function current() {
	    return this->test[this->position];
	}

	public function key() {
	    return this->position;
	}

	public function next() {
	    let this->position++;
	}

	public function valid() {
	    return isset this->test[this->position];
	}
}
