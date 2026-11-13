namespace Stub;

/**
 * `fetch` of a property is PHP's `isset()` followed by a read, except that a
 * property which exists and holds null is found. In an `isset`, `empty` or
 * `fetch` chain every container is fetched silently and every key or
 * property name is read as PHP reads it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2832
 */
class Issue2832
{
    public static function fetchName(var o, var n) -> array
    {
        var c;

        if fetch c, o->{n} {
            return [true, c];
        }

        return [false, c];
    }

    public static function fetchLiteral(var o) -> array
    {
        var c;

        if fetch c, o->x {
            return [true, c];
        }

        return [false, c];
    }

    public static function fetchString(var o) -> array
    {
        var c;

        if fetch c, o->{"x"} {
            return [true, c];
        }

        return [false, c];
    }

    public static function fetchEscaped(var o) -> array
    {
        var c;

        if fetch c, o->{"a\"b\\c"} {
            return [true, c];
        }

        return [false, c];
    }

    public static function issetEscaped(var o) -> bool
    {
        return isset o->{"a\"b\\c"};
    }

    public static function emptyEscaped(var o) -> bool
    {
        return empty o->{"a\"b\\c"};
    }

    public static function readEscaped(var o)
    {
        return o->{"a\"b\\c"};
    }

    public static function writeEscaped(var o, var value) -> void
    {
        let o->{"a\"b\\c"} = value;
    }

    public static function fetchIntName(var o, int n) -> array
    {
        var c;

        if fetch c, o->{n} {
            return [true, c];
        }

        return [false, c];
    }

    public static function fetchStatement(var o, var n)
    {
        var c;

        fetch c, o->{n};

        return c;
    }

    public static function fetchThenMark(var o, var n, var target)
    {
        var c;

        if fetch c, o->{n} {
            let target->marked = true;

            return c;
        }

        let target->marked = true;

        return null;
    }

    public static function fetchSelf(var o, var n) -> array
    {
        if fetch o, o->{n} {
            return [true, o];
        }

        return [false, o];
    }

    public static function fetchIntoName(var o, var n) -> array
    {
        if fetch n, o->{n} {
            return [true, n];
        }

        return [false, n];
    }

    public static function fetchSelfArray(var a, var k) -> array
    {
        if fetch a, a[k] {
            return [true, a];
        }

        return [false, a];
    }

    public static function fetchIntoKey(var a, var k) -> array
    {
        if fetch k, a[k] {
            return [true, k];
        }

        return [false, k];
    }

    public static function issetKey(var a, var o) -> bool
    {
        return isset a[o->k];
    }

    public static function fetchKey(var a, var o) -> array
    {
        var c;

        if fetch c, a[o->k] {
            return [true, c];
        }

        return [false, c];
    }

    public static function emptyKey(var a, var o) -> bool
    {
        return empty a[o->k];
    }

    public static function issetDeep(var a) -> bool
    {
        return isset a[1][2][3];
    }

    public static function fetchDeep(var a) -> array
    {
        var c;

        if fetch c, a[1][2][3] {
            return [true, c];
        }

        return [false, c];
    }

    public static function emptyDeep(var a) -> bool
    {
        return empty a[1][2][3];
    }

    public static function issetPropDeep(var o) -> bool
    {
        return isset o->x->y->z;
    }

    public static function fetchPropDeep(var o) -> array
    {
        var c;

        if fetch c, o->x->y->z {
            return [true, c];
        }

        return [false, c];
    }

    public static function issetDynamicDeep(var o, var n) -> bool
    {
        return isset o->x->{n}->z;
    }

    public static function issetPropArray(var o) -> bool
    {
        return isset o->p["k"]["j"];
    }
}
