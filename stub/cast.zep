
/**
 * Cast tests
 */

namespace Stub;

class Cast
{
    // To char cast

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testCharCastFromChar() -> char
    {
        return (char)  'a';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testCharCastFromVariableChar() -> char
    {
        var a = 'A';
        return (char) a;
    }

    // To string cast

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testStringCastChar() -> string
    {
        return (string) 'z';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testStringCastVariableChar() -> string
    {
        char a = 'X';
        return (string) a;
    }

    // To int cast

    public function testIntCastFromFloat() -> int
    {
        return (int) 5.0;
    }

    public function testIntCastFromVariableFloat() -> int
    {
        var a = 5.0;
        return (int) a;
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testIntCastFromChar() -> int
    {
        return (int) 'A';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testIntCastFromVariableChar() -> int
    {
        var a = 'a';
        return (int) a;
    }

    public function testIntCastFromBooleanTrue() -> int
    {
        return (int) true;
    }

    public function testIntCastFromBooleanFalse() -> int
    {
        return (int) false;
    }

    public function testIntCastFromVariableBooleanTrue() -> int
    {
        var a = true;
        return (int) a;
    }

    public function testIntCastFromVariableBooleanFalse() -> int
    {
        var a = false;
        return (int) a;
    }

    public function testIntCastFromVariableNull() -> int
    {
        var a = null;
        return (int) a;
    }

    public function testIntCastFromStringValue() -> int
    {
        return (int) "test";
    }

    public function testIntCastFromVariableString() -> int
    {
        string a;
        let a = "test";

        return (int) a;
    }

    public function testIntCastFromParameterString(const string a) -> int
    {
        return (int) a;
    }

    public function testIntCastFromNull() -> int
    {
        return (int) null;
    }

    public function testIntCastFromVariableEmptyArray() -> int
    {
        var a = [];
        return (int) a;
    }

    public function testIntCastFromEmptyArray() -> int
    {
        return (int) [];
    }

    public function testIntCastFromVariableArray() -> int
    {
        var a = [1, 2, 3, 4];
        return (int) a;
    }

    public function testIntCastFromArray() -> int
    {
        return (int) [1, 2, 3, 4];
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testIntCastFromStdClass() -> int
    {
        return (int) (new \StdClass);
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testIntCastFromVariableStdClass() -> int
    {
        var a;
        let a = new \StdClass;
        return (int) a;
    }

    // To long cast

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testLongCastFromChar() -> long
    {
        return (long) 'a';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testLongCastFromVariableChar() -> long
    {
        char a = 'A';
        return (long) a;
    }

    // To float cast

    public function testFloatCastFromFloat() -> float
    {
        return (float) 5.0;
    }

    public function testFloatCastFromVariableFloat() -> float
    {
        var a = 5.0;
        return (float) a;
    }

    public function testFloatCastFromBooleanTrue() -> float
    {
        return (float) true;
    }

    public function testFloatCastFromBooleanFalse() -> float
    {
        return (float) false;
    }

    public function testFloatCastFromVariableBooleanTrue() -> float
    {
        var a = true;
        return (float) a;
    }

    public function testFloatCastFromVariableBooleanFalse() -> float
    {
        var a = false;
        return (float) a;
    }

    public function testFloatCastFromVariableNull() -> float
    {
        var a = null;
        return (float) a;
    }

    public function testFloatCastFromNull() -> float
    {
        return (float) null;
    }

    public function testFloatCastFromVariableEmptyArray() -> float
    {
        var a = [];
        return (float) a;
    }

    public function testFloatCastFromEmptyArray() -> float
    {
        return (float) [];
    }

    public function testFloatCastFromVariableArray() -> float
    {
        var a = [1, 2, 3, 4];
        return (float) a;
    }

    public function testFloatCastFromArray() -> float
    {
        return (float) [1, 2, 3, 4];
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testFloatCastFromStdClass() -> float
    {
        return (float) (new \StdClass);
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testFloatCastFromVariableStdClass() -> float
    {
        var a;
        let a = new \StdClass;
        return (float) a;
    }

    // To double cast

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testDoubleCastFromVChar() -> double
    {
        return (double) 'a';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testDoubleCastFromVariableChar() -> double
    {
        char a = 'A';
        return (double) a;
    }

    // To boolean cast

    public function testBooleanCastFromIntTrue1() -> boolean
    {
        return (boolean) 1;
    }

    public function testBooleanCastFromIntTrue2() -> boolean
    {
        return (boolean) 1000;
    }

    public function testBooleanCastFromIntFalse() -> boolean
    {
        return (boolean) 0;
    }

    public function testBooleanCastFromObject() -> boolean
    {
        var simpleObject;
        let simpleObject = new \StdClass();
        return (boolean) simpleObject;
    }

    public function testBooleanCastFromEmptyArray() -> boolean
    {
        var a = [];
        return (boolean) a;
    }

    public function testBooleanCastFromArray() -> boolean
    {
        var a = [1, 2, 3, 4];
        return (boolean) a;
    }

    public function testBooleanCastFromNull() -> boolean
    {
        var a = null;
        return (boolean) a;
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testBooleanCastFromChar() -> boolean
    {
        return (boolean) 'a';
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testBooleanCastFromVariableChar() -> boolean
    {
        char a = 'A';
        return (boolean) a;
    }

    // To object cast

    public function testObjectCastFromInt() -> var
    {
        return (object) 5;
    }

    public function testObjectCastFromFloat() -> var
    {
        return (object) 5.0;
    }

    public function testObjectCastFromFalse() -> var
    {
        return (object) false;
    }

    public function testObjectCastFromTrue() -> var
    {
        return (object) true;
    }

    public function testObjectCastFromNull() -> var
    {
        return (object) null;
    }

    public function testObjectCastFromEmptyArray() -> var
    {
        return (object) [];
    }

    public function testObjectCastFromArray() -> var
    {
        return (object) [1, 2, 3, 4];
    }

    public function testObjectCastFromEmptyString() -> var
    {
        return (object) "";
    }

    public function testObjectCastFromString() -> var
    {
        return (object) "test string";
    }

    public function testCastStdinToInteger() -> int
    {
        var handle;
        let handle = STDIN;

        return (int) handle;
    }

    public function testCastStdoutToInteger() -> int
    {
        return (int) STDOUT;
    }

    public function testCastFileResourceToInteger(var fileName) -> int
    {
        var id;

        let id = (int) fileName;

        return id;
    }

    // To array cast

    public function testArrayCastFromVariableArray() -> array
    {
        var uids = [1, "2", 3];
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableTrue() -> array
    {
        var uids = true;
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableFalse() -> array
    {
        var uids = false;
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableNull() -> array
    {
        var uids = null;
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableInteger() -> array
    {
        var uids = 1;
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableFloat() -> array
    {
        var uids = 1.1;
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableString() -> array
    {
        var uids = "aaa";
        let uids = (array) uids;
        return uids;
    }

    public function testArrayCastFromVariableStdClass() -> array
    {
        var uids;
        let uids = new \StdClass;
        let uids->p1 = "v1";
        let uids->p2 = "v2";
        let uids = (array) uids;
        return uids;
    }

}
