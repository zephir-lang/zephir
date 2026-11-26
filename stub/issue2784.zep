namespace Stub;

/**
 * The issue's `POther`: reads, writes and unsets a property from outside the
 * class that declares it. PHP throws an Error for an inaccessible property,
 * warns for an undefined one, and hands an inaccessible one to `__unset()`
 * when the class has it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
class Issue2784
{
    public function readProt(var o)
    {
        return o->prot;
    }

    public function readPriv(var o)
    {
        return o->priv;
    }

    public function readUndefined(var o)
    {
        return o->nope;
    }

    public function writeProt(var o)
    {
        let o->prot = "changed";

        return "written";
    }

    public static function unsetProt(var o)
    {
        unset o->prot;

        return "unset";
    }

    public static function unsetPriv(var o)
    {
        unset o->priv;

        return "unset";
    }

    public static function unsetNamed(var o, var name)
    {
        unset o->{name};

        return "unset";
    }

    public static function unsetProtThenMark(var o, var target) -> void
    {
        unset o->prot;

        let target->marked = true;
    }

    public static function unsetOffset(var o) -> void
    {
        unset o->x["k"];
    }

    public static function writeProtOffset(var o) -> void
    {
        let o->prot["k"] = 1;
    }

    public static function pushIntoProt(var o) -> void
    {
        array_push(o->prot, 1);
    }

    public static function readMagicProtected(<Issue2784Magic> o)
    {
        return o->prot;
    }

    public static function readMagicPrivate(<Issue2784Magic> o)
    {
        return o->priv;
    }

    public static function readMagicUndeclared(<Issue2784Magic> o)
    {
        return o->virtual;
    }

    public static function issetPrivateOffset(<Issue2784Base> o) -> bool
    {
        return isset o->priv["k"];
    }

    public static function emptyPropertyOfProtected(<Issue2784Base> o) -> bool
    {
        return empty o->prot->x;
    }

    public static function fetchPrivateOffset(<Issue2784Base> o)
    {
        var value;

        if fetch value, o->priv["k"] {
            return value;
        }

        return "none";
    }
}

/**
 * A function has no class: like PHP's own, it reaches no protected or private
 * property, whichever class calls it.
 */
function issue2784_read(var o)
{
    return o->prot;
}

function issue2784_write(var o)
{
    let o->prot = "changed";

    return "written";
}

function issue2784_unset(var o)
{
    unset o->prot;

    return "unset";
}

function issue2784_isset(var o) -> bool
{
    return isset o->prot;
}
