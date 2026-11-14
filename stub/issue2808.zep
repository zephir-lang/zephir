namespace Stub;

/**
 * Compound assignments, `++`, `--` and plain assignments of a property as
 * PHP's ASSIGN_OBJ_OP, PRE_INC_OBJ and ASSIGN_OBJ: one read-write fetch that
 * creates an undefined property before it warns, operates in place, and an
 * Error for a non-object. A write that throws stops the method.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
class Issue2808
{
    public static function add(var o, var v)
    {
        let o->x += v;
    }

    public static function sub(var o, var v)
    {
        let o->x -= v;
    }

    public static function mul(var o, var v)
    {
        let o->x *= v;
    }

    public static function div(var o, var v)
    {
        let o->x /= v;
    }

    public static function mod(var o, var v)
    {
        let o->x %= v;
    }

    public static function concat(var o, var v)
    {
        let o->x .= v;
    }

    public static function bitwiseAnd(var o, var v)
    {
        let o->x &= v;
    }

    public static function bitwiseOr(var o, var v)
    {
        let o->x |= v;
    }

    public static function bitwiseXor(var o, var v)
    {
        let o->x ^= v;
    }

    public static function shiftLeft(var o, var v)
    {
        let o->x <<= v;
    }

    public static function shiftRight(var o, var v)
    {
        let o->x >>= v;
    }

    public static function addOne(var o)
    {
        let o->x += 1;
    }

    public static function addDouble(var o)
    {
        let o->x += 1.5;
    }

    public static function concatLiteral(var o)
    {
        let o->x .= "s";
    }

    public static function concatChar(var o)
    {
        char c = 'A';

        let o->x .= c;
    }

    public static function concatCharLiteral(var o)
    {
        let o->x .= 'B';
    }

    public static function addChar(var o)
    {
        char c = 'A';

        let o->x += c;
    }

    public static function addCall(var o, var v)
    {
        let o->x += self::identity(v);
    }

    public static function addDynamic(var o, var name, var v)
    {
        let o->{name} += v;
    }

    public static function concatDynamic(var o, var name, var v)
    {
        let o->{name} .= v;
    }

    public static function addNativeName(var o)
    {
        int name = 7;

        let o->{name} += 1;
    }

    public static function addString(var o, var v)
    {
        let o->{"x"} += v;
    }

    public static function concatString(var o, var v)
    {
        let o->{"x"} .= v;
    }

    public static function increment(var o)
    {
        let o->x++;
    }

    public static function decrement(var o)
    {
        let o->x--;
    }

    public static function assign(var o, var v)
    {
        let o->x = v;
    }

    public static function assignDynamic(var o, var name, var v)
    {
        let o->{name} = v;
    }

    public static function assignThenMark(var o, var v, var target)
    {
        let o->x = v;
        let target->marked = true;
    }

    public static function assignDynamicThenMark(var o, var name, var v, var target)
    {
        let o->{name} = v;
        let target->marked = true;
    }

    public static function addThenMark(var o, var v, var target)
    {
        let o->x += v;
        let target->marked = true;
    }

    public static function incrementThenMark(var o, var target)
    {
        let o->x++;
        let target->marked = true;
    }

    public static function identity(var v)
    {
        return v;
    }
}
