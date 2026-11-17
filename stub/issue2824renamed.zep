namespace Stub;

/**
 * Renames the parameters it overrides, which PHP allows.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824Renamed extends Issue2824Pair
{
    public function pair(var first, var second) -> string
    {
        return first . ";" . second;
    }
}
