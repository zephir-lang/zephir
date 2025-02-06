
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

     public function castToStringMixedAndReturnMixed(mixed val) -> mixed
     {
        return (string)val;
     }

     public function castToStringInternallyMixedAndReturnMixed(mixed val) -> mixed
     {
        let val = (string)val;

        return val;
     }

     public function castToIntMixedAndReturnMixed(mixed val) -> mixed
     {
        return (int)val;
     }

     public function castToIntInternallyMixedAndReturnMixed(mixed val) -> mixed
     {
        let val = (int)val;

        return val;
     }

     public function castToBoolMixedAndReturnMixed(mixed val) -> mixed
     {
        return (bool)val;
     }

     public function castToBoolInternallyMixedAndReturnMixed(mixed val) -> mixed
     {
        let val = (bool)val;

        return val;
     }

     public function castToFloatMixedAndReturnMixed(mixed val) -> mixed
     {
        return (float)val;
     }

     public function castToFloatInternallyMixedAndReturnMixed(mixed val) -> mixed
     {
        let val = (float)val;

        return val;
     }

     /**
      * Only used during generation
      */
     public function mixedInCondition(mixed val) -> void
     {
        if unlikely val {
            // OK
        }

        if val === 1 {
            // OK
        }

        if 1 === val {

        }

        echo val;
     }
}
