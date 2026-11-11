namespace Stub;

/**
 * A subclass may read a protected property of its parent's instances.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 */
class Issue2831Child extends Issue2831Base
{
    public static function readParentProtected(var o)
    {
        return o->prot;
    }
}
