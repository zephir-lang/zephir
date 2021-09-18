
namespace Stub\Types;

class MixedType
{
    public function returnMixedObject() -> mixed
	{
	    return new \stdClass();
	}

	public function returnMixedArray() -> mixed
	{
	    return [];
	}

	public function returnMixedString() -> mixed
	{
	    return "mixed string";
	}

	public function returnMixedInt() -> mixed
	{
	    return 1;
	}

	public function returnMixedFloat() -> mixed
	{
	    return 3.14;
	}

	public function returnMixedBool() -> mixed
	{
	    return true;
	}

	public function returnMixedNull() -> mixed
	{
	    return null;
	}

	public function returnMixed74(bool diff = false) -> mixed
	{
	    if (diff) {
	        return "string";
	    }

	    return new \stdClass();
	}

    public function returnMultiButAlwaysMixed() -> mixed | var | int
    {
        // Maybe in future make sense to generate with `IS_MIXED`.
        return "ZEND_BEGIN_ARG_INFO_EX";
    }
}
