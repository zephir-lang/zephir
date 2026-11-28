namespace Stub;

/**
 * An \ArrayObject with an array default is still created by \ArrayObject,
 * which allocates the storage its methods use.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
class Issue2799ArrayObject extends \ArrayObject
{
    protected extra = [];

    public function getExtra()
    {
        return this->extra;
    }
}
