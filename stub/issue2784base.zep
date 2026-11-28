namespace Stub;

/**
 * Properties a class outside this hierarchy cannot reach.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
class Issue2784Base
{
    protected prot = "Issue2784Base::prot";

    private priv = "Issue2784Base::priv";

    /**
     * `unset this->priv` reaches the private property of this class on an
     * instance of a subclass as well.
     */
    public function unsetOwnPrivate() -> bool
    {
        unset this->priv;

        return isset this->priv;
    }

    /**
     * A closure reaches what the class it is written in reaches.
     */
    public static function closureReadingPrivate()
    {
        return function (<Issue2784Base> o) {
            return o->priv;
        };
    }
}
