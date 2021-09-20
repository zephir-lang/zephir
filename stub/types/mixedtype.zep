
namespace Stub\Types;

class MixedType
{
    /**
     * Mixed only as return type methods
     */

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

    /**
     * Mixed only as parameter in method
     */

     public function paramMixed(mixed val)
     {
        return val;
     }

     public function paramMixedTwo(mixed val1, mixed val2)
     {
        return [val1, val2];
     }

     public function paramMixedWithMulti(int intVal, string stringVal, mixed mixedVal)
     {
        return [intVal, stringVal, mixedVal];
     }

     /**
      * Mixed as as parameter and return type in method
      */

     public function paramAndReturnMixed(mixed val) -> mixed
     {
        return val;
     }
}
