namespace Stub;

/**
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824Child extends Issue2824Pair
{
    public function pair(var a, var b) -> string
    {
        return "child:" . parent::pair(b: b, a: a);
    }

    public static function parentPair() -> string
    {
        return parent::spair(b: "2", a: "1");
    }
}
