namespace Stub;

/**
 * Property reads as PHP's `$o->x`: `__get()` without asking `__isset()`
 * first, a warning for an undefined property or a non-object, an Error for
 * an inaccessible or uninitialized one, and a stop when the read throws.
 * `empty` and `isset` keep PHP's silent fetch.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 * @see https://github.com/zephir-lang/zephir/issues/2784
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
class Issue2831
{
    public static function read(var o)
    {
        return o->x;
    }

    public static function readDynamic(var o, var name)
    {
        return o->{name};
    }

    public static function readProtected(var o)
    {
        return o->prot;
    }

    public static function readPrivate(var o)
    {
        return o->priv;
    }

    public static function readTyped(var o)
    {
        return o->typed;
    }

    public static function readInCondition(var o) -> string
    {
        if o->x {
            return "yes";
        }

        return "no";
    }

    public static function isEmpty(var o) -> bool
    {
        return empty o->x;
    }

    public static function isEmptyDynamic(var o, var name) -> bool
    {
        return empty o->{name};
    }

    public static function issetOrEmpty(var o, bool askIsset) -> bool
    {
        if askIsset {
            return isset o->x;
        }

        return empty o->x;
    }

    public static function issetDim(var o) -> bool
    {
        return isset o->x["k"];
    }

    public static function issetDynamic(var o, var name) -> bool
    {
        return isset o->{name};
    }

    public static function issetNested(var o) -> bool
    {
        return isset o->x->y;
    }

    public static function isEmptyNested(var o) -> bool
    {
        return empty o->x->y;
    }

    public static function issetThenMark(var o, var target) -> bool
    {
        bool result;

        let result = isset o->x;
        let target->marked = true;

        return result;
    }

    public static function isEmptyThenMark(var o, var target) -> bool
    {
        bool result;

        let result = empty o->x;
        let target->marked = true;

        return result;
    }

    public static function addAssign(var o)
    {
        let o->c += 1;

        return o->c;
    }

    public static function addAssignDynamic(var o, var name)
    {
        let o->{name} += 1;

        return o->{name};
    }

    public static function increment(var o)
    {
        let o->c++;

        return o->c;
    }

    public static function readThenMark(var o, var target)
    {
        var value;

        let value = o->x;
        let target->marked = true;

        return value;
    }

    public static function readProtectedThenMark(var o, var target)
    {
        var value;

        let value = o->prot;
        let target->marked = true;

        return value;
    }
}
