namespace Stub;

/**
 * Properties a reader outside the class cannot reach, or not yet.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 */
class Issue2831Base
{
    protected prot = "prot";

    private priv = "priv";

    public int typed;

    /**
     * A read of `this` that borrows the property instead of copying it.
     */
    public function readOwnTyped()
    {
        var value;

        let value = this->typed;

        return value;
    }

    public function readOwnTypedInCondition() -> string
    {
        if this->typed {
            return "yes";
        }

        return "no";
    }

    /**
     * Another instance of the same class may be read from here.
     */
    public static function readOtherProtected(var o)
    {
        return o->prot;
    }
}
