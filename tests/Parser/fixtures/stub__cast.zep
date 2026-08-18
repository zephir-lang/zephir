
/**
 * Cast tests
 */

namespace Stub;

class Cast
{
    // To char cast

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testCharCastFromChar() -> char
    {
        return (char)  'a';
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testCharCastFromVariableChar() -> char
    {
        var a = 'A';
        return (char) a;
    }

    // To string cast

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testStringCastChar() -> string
    {
        return (string) 'z';
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testStringCastVariableChar() -> string
    {
        char a = 'X';
        return (string) a;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2232
     */
    public function testStringCastFromNull() -> string
    {
        var a = null;

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
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testIntCastFromChar() -> int
    {
        return (int) 'A';
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
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
        return (int) (new \stdClass);
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testIntCastFromVariableStdClass() -> int
    {
        var a;
        let a = new \stdClass;
        return (int) a;
    }

    // To long cast

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testLongCastFromChar() -> long
    {
        return (long) 'a';
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
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
        return (float) (new \stdClass);
    }

    /**
     * Notice: Object of class stdClass could not be converted to int
     * @return int 1
     */
    public function testFloatCastFromVariableStdClass() -> float
    {
        var a;
        let a = new \stdClass;
        return (float) a;
    }

    // To double cast

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testDoubleCastFromVChar() -> double
    {
        return (double) 'a';
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
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
        let simpleObject = new \stdClass();
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
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testBooleanCastFromChar() -> boolean
    {
        return (boolean) 'a';
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
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
        let uids = new \stdClass;
        let uids->p1 = "v1";
        let uids->p2 = "v2";
        let uids = (array) uids;
        return uids;
    }

    public function testIssue828() -> array
    {
        array ret = [];
        var version = "1.0 200 OK", nonNumericString = "OK", floatVersion, intVersion, floatNonNumeric, intNonNumeric;

        let ret[] = version;
        let ret[] = nonNumericString;

        let floatVersion = (double)version,
            intVersion   = (int)version,
            floatNonNumeric = (double)nonNumericString,
            intNonNumeric = (int)nonNumericString;

        let ret[] = floatVersion;
        let ret[] = intVersion;
        let ret[] = floatNonNumeric;
        let ret[] = intNonNumeric;

        return ret;
    }

    /**
     * Reassigning a variable whose first inferred type comes from an
     * `(object)` cast used to make the static type-inference pass leak
     * `StaticTypeInference=object undefined` to stdout during compilation.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1877
     */
    public function testObjectCastThenReassign() -> string
    {
        var data;

        let data = (object) ["key": "value"];
        let data = "reassigned";

        return data;
    }

    /**
     * Every cast target has to accept every source form, not just the handful
     * of literal types the cast operator used to enumerate. Each method below
     * walks one target across: literals, native-typed locals, an `array` local,
     * a compound expression, a method call and a genuinely dynamic parameter.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToInt(var dyn) -> array
    {
        int i = 5;
        long l = 6;
        double d = 5.5;
        bool b = true;
        string s = "12abc";
        char c = 'z';
        array arr = [1, 2];

        return [
            (int) 5,
            (int) 5.5,
            (int) true,
            (int) false,
            (int) null,
            (int) "12abc",
            (int) 'z',
            (int) [1, 2],
            (int) [],
            (int) i,
            (int) l,
            (int) d,
            (int) b,
            (int) s,
            (int) c,
            (int) arr,
            (int) (i + 1),
            (int) dyn,
            (int) this->issue1841Helper()
        ];
    }

    /**
     * `uint`/`ulong` had no branch at all, so every source failed to compile.
     * Only non-negative values are exercised: the C width of `unsigned long`
     * differs between LP64 and LLP64, so wraparound is not portable.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToUnsigned(var dyn) -> array
    {
        int i = 5;
        double d = 5.5;
        string s = "12abc";
        char c = 'z';

        return [
            (uint) 5,
            (uint) 5.5,
            (uint) true,
            (uint) null,
            (uint) "12abc",
            (uint) 'z',
            (uint) i,
            (uint) d,
            (uint) s,
            (uint) c,
            (uint) dyn,
            (ulong) 5,
            (ulong) 5.5,
            (ulong) true,
            (ulong) null,
            (ulong) "12abc",
            (ulong) i,
            (ulong) c,
            (ulong) dyn
        ];
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToLong(var dyn) -> array
    {
        int i = 5;
        long l = 6;
        double d = 5.5;
        bool b = true;
        string s = "12abc";
        char c = 'z';
        array arr = [1, 2];

        return [
            (long) 5,
            (long) 5.5,
            (long) true,
            (long) null,
            (long) "12abc",
            (long) 'z',
            (long) [1, 2],
            (long) i,
            (long) l,
            (long) d,
            (long) b,
            (long) s,
            (long) c,
            (long) arr,
            (long) (i + 1),
            (long) dyn,
            (long) this->issue1841Helper()
        ];
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToFloat(var dyn) -> array
    {
        int i = 5;
        long l = 6;
        double d = 5.5;
        bool b = true;
        string s = "12abc";
        char c = 'z';
        array arr = [1, 2];

        return [
            (float) 5,
            (float) 5.5,
            (float) true,
            (float) false,
            (float) null,
            (float) "5.5abc",
            (float) 'z',
            (float) [1, 2],
            (float) [],
            (float) i,
            (float) l,
            (float) d,
            (float) b,
            (float) s,
            (float) c,
            (float) arr,
            (float) (i + 1),
            (float) dyn,
            (float) this->issue1841Helper()
        ];
    }

    /**
     * A C cast truncates, so `(bool) 0.4` must not go through one: PHP treats
     * every non-zero double as true.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToBool(var dyn) -> array
    {
        int i = 5;
        long l = 6;
        double d = 5.5;
        bool b = true;
        string s = "12abc";
        string zero = "0";
        char c = 'z';
        array arr = [1, 2];

        return [
            (bool) 1,
            (bool) 0,
            (bool) 0.4,
            (bool) 0.0,
            (bool) true,
            (bool) false,
            (bool) null,
            (bool) "0",
            (bool) "",
            (bool) "abc",
            (bool) 'z',
            (bool) [1, 2],
            (bool) [],
            (bool) i,
            (bool) l,
            (bool) d,
            (bool) b,
            (bool) s,
            (bool) zero,
            (bool) c,
            (bool) arr,
            (bool) dyn,
            (bool) this->issue1841Helper()
        ];
    }

    /**
     * Zephir's `char`/`uchar` is a byte, so a cast to it follows `(int)` and
     * boxes as the integer byte value — PHP has no character type.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1629
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToChar(var dyn) -> array
    {
        int i = 5;
        char c = 'z';
        string s = "65";

        return [
            (char) 65,
            (char) 'z',
            (char) 122.9,
            (char) true,
            (char) null,
            (char) "A",
            (char) "65",
            (char) i,
            (char) c,
            (char) s,
            (char) dyn,
            (char) 321,
            (uchar) 65,
            (uchar) 'z',
            (uchar) 200,
            (uchar) null,
            (uchar) i,
            (uchar) c,
            (uchar) dyn
        ];
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToString(var dyn) -> array
    {
        int i = 5;
        long l = 6;
        double d = 5.5;
        bool b = true;
        string s = "12abc";
        char c = 'z';

        return [
            (string) 5,
            (string) 5.0,
            (string) 5.5,
            (string) true,
            (string) false,
            (string) null,
            (string) "abc",
            (string) 'z',
            (string) i,
            (string) l,
            (string) d,
            (string) b,
            (string) s,
            (string) c,
            (string) (i + 1),
            (string) dyn,
            (string) this->issue1841Helper()
        ];
    }

    /**
     * Kept apart from issue1841ToString(): PHP raises "Array to string
     * conversion" here, so the caller has to silence it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToStringFromArray() -> array
    {
        array arr = [1, 2];

        return [
            (string) [1, 2],
            (string) arr
        ];
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToArray(var dyn) -> array
    {
        int i = 5;
        string s = "12abc";
        char c = 'z';
        array arr = [1, 2];

        return [
            (array) 5,
            (array) 5.5,
            (array) true,
            (array) null,
            (array) "abc",
            (array) 'z',
            (array) [1, 2],
            (array) [],
            (array) i,
            (array) s,
            (array) c,
            (array) arr,
            (array) dyn
        ];
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToObject(var dyn) -> array
    {
        int i = 5;
        string s = "abc";
        char c = 'z';

        return [
            (object) 5,
            (object) null,
            (object) [1, 2],
            (object) "x",
            (object) 'z',
            (object) i,
            (object) s,
            (object) c,
            (object) dyn
        ];
    }

    /**
     * `var` accepts anything, so `(var)` is a no-op that has to preserve the
     * value and its type rather than fail with "Cannot cast: X to variable".
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841ToVar(var dyn) -> array
    {
        int i = 5;
        string s = "abc";
        char c = 'z';
        array arr = [1, 2];

        return [
            (var) 5,
            (var) 5.5,
            (var) true,
            (var) null,
            (var) "abc",
            (var) 'z',
            (var) [1, 2],
            (var) i,
            (var) s,
            (var) c,
            (var) arr,
            (var) dyn
        ];
    }

    /**
     * `(array)`/`(object)` lower to kernel conversions that run in place, so
     * casting a variable used to overwrite that variable as a side effect.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841CastKeepsOperandIntact(var value) -> array
    {
        var asArray, asObject;

        let asArray = (array) value;
        let asObject = (object) value;

        return [value, asArray, asObject];
    }

    /**
     * The reported symptoms: a cast (or a parenthesized expression) used as a
     * variable declaration's default value reached the assignment as the AST
     * node type, failing with "Unknown type: cast" / "Unknown type: list".
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function issue1841DeclarationInitializers(int a, int b) -> array
    {
        var x = (int) (a - b);
        float y = (a - b);
        int z = (int) (a * b);
        var s = (string) (a + b);
        var n = (int) (string) a;
        var u = (uint) (a + b);
        var w = (ulong) (a + b);

        return [x, y, z, s, n, u, w];
    }

    private function issue1841Helper() -> int
    {
        return 3;
    }
}
