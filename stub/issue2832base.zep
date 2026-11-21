namespace Stub;

/**
 * `fetch` from inside the class that declares the properties, where PHP's
 * `isset()` can see them.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2832
 */
class Issue2832Base
{
    protected prot = "prot";

    private priv = "priv";

    protected nul = null;

    public static function fetchName(var o, var n) -> array
    {
        var c;

        if fetch c, o->{n} {
            return [true, c];
        }

        return [false, c];
    }
}
