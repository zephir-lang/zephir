namespace Stub;

class ArrayAccessObj implements \ArrayAccess
{
	protected test;

	public function __construct()
	{
	    var obj;

	    let obj = new \stdClass();
        let obj->key1 = "val1";
        let obj->key2 = "val2";
        let obj->key3 = "val3";

        let this->test = obj;
	}

	public function offsetSet(mixed offset, mixed value) -> void
	{
	    var obj = this->test;

	    if !is_null(offset) {
	        let obj->{offset} = value;
	    }

	    let this->test = obj;
	}

	public function offsetExists(mixed offset) -> bool
	{
	    var obj = this->test;

		return isset obj->{offset};
	}

	public function offsetUnset(mixed offset) -> void
	{
	    var obj = this->test;

	    // It is not possible to unset dynamic property via Zend API.
	    let obj = (array)obj;

        unset obj[offset];

        let this->test = (object)obj;
	}

	public function offsetGet(mixed offset) -> mixed
	{
		return isset this->test->{offset} ? this->test->{offset} : null;
	}
}
