namespace Stub;

/**
 * `__get()` answers every property a read cannot reach: a parent's private
 * property, and an undeclared one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
class Issue2784Magic extends Issue2784Base
{
    public function __get(string name)
    {
        return "__get(" . name . ")";
    }

    public function readParentPrivate()
    {
        var value;

        let value = this->priv;

        return value;
    }

    public function returnParentPrivate()
    {
        return this->priv;
    }

    public function returnVirtual()
    {
        return this->virtual;
    }
}
