namespace Stub;

/**
 * A subclass may unset a protected property of its parent's instances.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
class Issue2784Child extends Issue2784Base
{
    public static function unsetParentProtected(var o) -> bool
    {
        unset o->prot;

        return isset o->prot;
    }
}
