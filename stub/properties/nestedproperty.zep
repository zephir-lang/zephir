namespace Stub\Properties;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2532
 */
class NestedProperty
{
    public bar;

    public function __construct()
    {
        let this->bar = new NestedPropertyTarget();
    }

    public function setData(int value = 0) -> void
    {
        let this->bar->data = value;
    }

    public function setName(string value) -> void
    {
        let this->bar->name = value;
    }

    public function getBar() -> <NestedPropertyTarget>
    {
        return this->bar;
    }
}
