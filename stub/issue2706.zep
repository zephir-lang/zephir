namespace Stub;

/**
 * Array keys PHP derives from a non-string offset.
 *
 * PHP turns a `null` offset into the empty string. `unset` and
 * array_key_exists() looked for a one byte NUL key instead, so they missed
 * the element every other access had written. Both also converted float,
 * bool and resource keys and rejected illegal ones on their own terms.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
class Issue2706
{
    /**
     * Set after array_key_exists() returns, so a test can tell whether the
     * method went on after it threw.
     */
    public reached = false;

    public container = [];

    public static shared = [];

    public function write(var key) -> array
    {
        var a = [];

        let a[key] = "written";

        return a;
    }

    public function readBack(var key) -> array
    {
        var a = [];

        let a[key] = "written";

        return [isset a[key], array_key_exists(key, a), a[key]];
    }

    public function unsetKey(var a, var key)
    {
        unset a[key];

        return a;
    }

    public function unsetPath(var a, var key)
    {
        unset a[key]["q"];

        return a;
    }

    public function keyExists(var key, var a) -> bool
    {
        return array_key_exists(key, a);
    }

    public function keyExistsOr(bool left, var key, var a) -> bool
    {
        return left || array_key_exists(key, a);
    }

    /**
     * Each of these reaches a kernel call that can throw: an illegal key,
     * a container that is not an array, or an ArrayAccess method throwing.
     * `reached` stays false only if the method stopped at that call.
     */
    public function writeStops(var a, var key)
    {
        let this->reached = false;
        let a[key] = 1;
        let this->reached = true;
    }

    public function writeMultiStops(var a, var key)
    {
        let this->reached = false;
        let a[key]["x"] = 1;
        let this->reached = true;
    }

    public function appendStops(var a)
    {
        let this->reached = false;
        let a[] = 1;
        let this->reached = true;
    }

    public function readStops(var a, var key)
    {
        var value;

        let this->reached = false;
        let value = a[key];
        let this->reached = true;

        return value;
    }

    public function issetStops(var a, var key) -> bool
    {
        bool found;

        let this->reached = false;
        let found = isset a[key];
        let this->reached = true;

        return found;
    }

    public function fetchStops(var a, var key)
    {
        var value;

        let this->reached = false;
        if fetch value, a[key] {
            let this->reached = true;
        }
        let this->reached = true;

        return value;
    }

    public function emptyStops(var a, var key) -> bool
    {
        bool result;

        let this->reached = false;
        let result = empty a[key];
        let this->reached = true;

        return result;
    }

    public function literalStops(var key, var value) -> array
    {
        var a;

        let this->reached = false;
        let a = [key: value];
        let this->reached = true;

        return a;
    }

    public function propertyWriteStops(var key)
    {
        let this->reached = false;
        let this->container[key] = 1;
        let this->reached = true;
    }

    public function stringOffsetStops(string s, var key)
    {
        var c;

        let this->reached = false;
        let c = s[key];
        let this->reached = true;

        return c;
    }

    public function inArray(var needle, var haystack) -> bool
    {
        return in_array(needle, haystack);
    }

    public function inArrayStops(var needle, var haystack) -> bool
    {
        var found;

        let this->reached = false;
        let found = in_array(needle, haystack);
        let this->reached = true;

        return found;
    }

    /**
     * `[key: value]` with a key that is not a literal, for every kind of
     * value. A dynamic key used to accept only a few value types.
     */
    public function literalVarKey(var k, var v, int iv, double dv, bool bv, string sv) -> array
    {
        return [[k: 1], [k: 1.5], [k: "s"], [k: true], [k: null], [k: v], [k: [1]], [k: iv], [k: dv], [k: bv], [k: sv]];
    }

    public function literalStringKey(string k, var v, int iv, double dv, bool bv, string sv) -> array
    {
        return [[k: 1], [k: 1.5], [k: "s"], [k: true], [k: null], [k: v], [k: [1]], [k: iv], [k: dv], [k: bv], [k: sv]];
    }

    public function literalIntKey(int k, var v, int iv, double dv, bool bv, string sv) -> array
    {
        return [[k: 1], [k: 1.5], [k: "s"], [k: true], [k: null], [k: v], [k: [1]], [k: iv], [k: dv], [k: bv], [k: sv]];
    }

    /**
     * `double` and `bool` keys, held in a typed local or written as a literal.
     * PHP converts them to integer keys, deprecating a fractional float.
     */
    public function keyDoubleWrite(double k, array a)
    {
        let a[k] = 1;
        return a;
    }

    public function keyBoolWrite(bool k, array a)
    {
        let a[k] = 1;
        return a;
    }

    public function keyLiteralDoubleWrite(array a)
    {
        let a[1.5] = 1;
        return a;
    }

    public function keyLiteralTrueWrite(array a)
    {
        let a[true] = 1;
        return a;
    }

    public function keyDoubleWriteNested(double k, array a)
    {
        let a[k]["x"] = 1;
        return a;
    }

    public function keyBoolWriteNested(bool k, array a)
    {
        let a[k]["x"] = 1;
        return a;
    }

    public function keyLiteralDoubleWriteNested(array a)
    {
        let a[1.5]["x"] = 1;
        return a;
    }

    public function keyLiteralTrueWriteNested(array a)
    {
        let a[true]["x"] = 1;
        return a;
    }

    public function keyDoubleAddAssign(double k, array a)
    {
        let a[k] += 1;
        return a;
    }

    public function keyBoolAddAssign(bool k, array a)
    {
        let a[k] += 1;
        return a;
    }

    public function keyLiteralDoubleAddAssign(array a)
    {
        let a[1.5] += 1;
        return a;
    }

    public function keyLiteralTrueAddAssign(array a)
    {
        let a[true] += 1;
        return a;
    }

    public function keyDoubleAppendUnder(double k, array a)
    {
        let a[k][] = 1;
        return a;
    }

    public function keyBoolAppendUnder(bool k, array a)
    {
        let a[k][] = 1;
        return a;
    }

    public function keyLiteralDoubleAppendUnder(array a)
    {
        let a[1.5][] = 1;
        return a;
    }

    public function keyLiteralTrueAppendUnder(array a)
    {
        let a[true][] = 1;
        return a;
    }

    public function keyDoubleRead(double k, array a)
    {
        return a[k];
    }

    public function keyBoolRead(bool k, array a)
    {
        return a[k];
    }

    public function keyLiteralDoubleRead(array a)
    {
        return a[1.5];
    }

    public function keyLiteralTrueRead(array a)
    {
        return a[true];
    }

    public function keyDoubleIsset(double k, array a)
    {
        return isset a[k];
    }

    public function keyBoolIsset(bool k, array a)
    {
        return isset a[k];
    }

    public function keyLiteralDoubleIsset(array a)
    {
        return isset a[1.5];
    }

    public function keyLiteralTrueIsset(array a)
    {
        return isset a[true];
    }

    public function keyDoubleFetch(double k, array a)
    {
        var x = null;
        bool found;

        let found = fetch x, a[k];

        return [found, x];
    }

    public function keyBoolFetch(bool k, array a)
    {
        var x = null;
        bool found;

        let found = fetch x, a[k];

        return [found, x];
    }

    public function keyLiteralDoubleFetch(array a)
    {
        var x = null;
        bool found;

        let found = fetch x, a[1.5];

        return [found, x];
    }

    public function keyLiteralTrueFetch(array a)
    {
        var x = null;
        bool found;

        let found = fetch x, a[true];

        return [found, x];
    }

    public function keyDoubleEmpty(double k, array a)
    {
        return empty a[k];
    }

    public function keyBoolEmpty(bool k, array a)
    {
        return empty a[k];
    }

    public function keyLiteralDoubleEmpty(array a)
    {
        return empty a[1.5];
    }

    public function keyLiteralTrueEmpty(array a)
    {
        return empty a[true];
    }

    public function keyDoubleUnset(double k, array a)
    {
        unset a[k];
        return a;
    }

    public function keyBoolUnset(bool k, array a)
    {
        unset a[k];
        return a;
    }

    public function keyLiteralDoubleUnset(array a)
    {
        unset a[1.5];
        return a;
    }

    public function keyLiteralTrueUnset(array a)
    {
        unset a[true];
        return a;
    }

    public function keyDoubleUnsetNested(double k, array a)
    {
        unset a[k]["x"];
        return a;
    }

    public function keyBoolUnsetNested(bool k, array a)
    {
        unset a[k]["x"];
        return a;
    }

    public function keyLiteralDoubleUnsetNested(array a)
    {
        unset a[1.5]["x"];
        return a;
    }

    public function keyLiteralTrueUnsetNested(array a)
    {
        unset a[true]["x"];
        return a;
    }

    public function keyDoubleLiteral(double k)
    {
        return [k: "v"];
    }

    public function keyBoolLiteral(bool k)
    {
        return [k: "v"];
    }

    public function keyLiteralDoubleLiteral()
    {
        return [1.5: "v"];
    }

    public function keyLiteralTrueLiteral()
    {
        return [true: "v"];
    }

    public function keyDoublePropWrite(double k)
    {
        let this->container = [];
        let this->container[k] = 1;
        return this->container;
    }

    public function keyBoolPropWrite(bool k)
    {
        let this->container = [];
        let this->container[k] = 1;
        return this->container;
    }

    public function keyLiteralDoublePropWrite()
    {
        let this->container = [];
        let this->container[1.5] = 1;
        return this->container;
    }

    public function keyLiteralTruePropWrite()
    {
        let this->container = [];
        let this->container[true] = 1;
        return this->container;
    }

    public function keyDoublePropRead(double k, array a)
    {
        let this->container = a;
        return this->container[k];
    }

    public function keyBoolPropRead(bool k, array a)
    {
        let this->container = a;
        return this->container[k];
    }

    public function keyLiteralDoublePropRead(array a)
    {
        let this->container = a;
        return this->container[1.5];
    }

    public function keyLiteralTruePropRead(array a)
    {
        let this->container = a;
        return this->container[true];
    }

    public function keyDoublePropUnset(double k, array a)
    {
        let this->container = a;
        unset this->container[k];
        return this->container;
    }

    public function keyBoolPropUnset(bool k, array a)
    {
        let this->container = a;
        unset this->container[k];
        return this->container;
    }

    public function keyLiteralDoublePropUnset(array a)
    {
        let this->container = a;
        unset this->container[1.5];
        return this->container;
    }

    public function keyLiteralTruePropUnset(array a)
    {
        let this->container = a;
        unset this->container[true];
        return this->container;
    }

    public function keyDoubleStaticWrite(double k)
    {
        let self::shared = [];
        let self::shared[k] = 1;
        return self::shared;
    }

    public function keyBoolStaticWrite(bool k)
    {
        let self::shared = [];
        let self::shared[k] = 1;
        return self::shared;
    }

    public function keyLiteralDoubleStaticWrite()
    {
        let self::shared = [];
        let self::shared[1.5] = 1;
        return self::shared;
    }

    public function keyLiteralTrueStaticWrite()
    {
        let self::shared = [];
        let self::shared[true] = 1;
        return self::shared;
    }

    public function keyLiteralNullWrite(array a)
    {
        let a[null] = 1;
        return a;
    }

    public function keyLiteralNullWriteNested(array a)
    {
        let a[null]["x"] = 1;
        return a;
    }

    public function keyLiteralNullAddAssign(array a)
    {
        let a[null] += 1;
        return a;
    }

    public function keyLiteralNullAppendUnder(array a)
    {
        let a[null][] = 1;
        return a;
    }

    public function keyLiteralNullRead(array a)
    {
        return a[null];
    }

    public function keyLiteralNullIsset(array a)
    {
        return isset a[null];
    }

    public function keyLiteralNullFetch(array a)
    {
        var x = null;
        bool found;

        let found = fetch x, a[null];

        return [found, x];
    }

    public function keyLiteralNullEmpty(array a)
    {
        return empty a[null];
    }

    public function keyLiteralNullUnset(array a)
    {
        unset a[null];
        return a;
    }

    public function keyLiteralNullUnsetNested(array a)
    {
        unset a[null]["x"];
        return a;
    }

    public function keyLiteralNullLiteral()
    {
        return [null: "v"];
    }

    public function keyLiteralNullPropWrite()
    {
        let this->container = [];
        let this->container[null] = 1;
        return this->container;
    }

    public function keyLiteralNullPropRead(array a)
    {
        let this->container = a;
        return this->container[null];
    }

    public function keyLiteralNullPropUnset(array a)
    {
        let this->container = a;
        unset this->container[null];
        return this->container;
    }

    public function keyLiteralNullStaticWrite()
    {
        let self::shared = [];
        let self::shared[null] = 1;
        return self::shared;
    }

    /**
     * `let a[k][] = v` on a parameter: an `array` parameter's dynamic type is
     * unknown at compile time, which only a `var` container should be checked for.
     */
    public function appendUnderArrayParam(array a, var k) -> array
    {
        let a[k][] = "v";
        return a;
    }

    public function appendUnderVarParam(var a, var k)
    {
        let a[k][] = "v";
        return a;
    }

    public function appendUnderDeepArrayParam(array a) -> array
    {
        let a["x"][1][] = "v";
        return a;
    }

    public function keyExistsStops(var key, var a) -> bool
    {
        var found;

        let this->reached = false;
        let found = array_key_exists(key, a);
        let this->reached = true;

        return found;
    }
}
