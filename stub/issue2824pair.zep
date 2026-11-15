namespace Stub;

/**
 * Named arguments are bound by the callee's parameter names at run time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824Pair
{
    public function pair(var a, var b) -> string
    {
        return a . "," . b;
    }

    public static function spair(var a, var b) -> string
    {
        return a . "," . b;
    }

    public function withDefault(var a = "x", var b = "y") -> string
    {
        return a . "," . b;
    }

    public function withCallable(callable | null c = null, var b = "y") -> string
    {
        return (c === null ? "null" : "set") . "," . b;
    }

    public function withObject(object | null o = null, var b = "y") -> string
    {
        return (o === null ? "null" : "set") . "," . b;
    }

    public function withInt(int a = 5, int b = 6) -> string
    {
        return a . "," . b;
    }

    public function variadic(var a, var ...rest) -> array
    {
        return [a, rest];
    }

    public function thisPair() -> string
    {
        return this->pair(b: "2", a: "1");
    }

    public static function selfPair() -> string
    {
        return self::spair(b: "2", a: "1");
    }

    public static function staticPair() -> string
    {
        return static::spair(b: "2", a: "1");
    }
}
