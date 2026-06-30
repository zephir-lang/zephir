
namespace Stub\Oo;

class PropertyAccess
{
    protected a;

    protected b;

    private privateVariable;

    public function __construct()
    {
        var test, test1;
        let test = new \stdClass;
        let test->test = ["1", 2, 3, 5, 6.00];
        let test->test2 = test->test[0];
        let test->test3 = test->test[1];
        let test->test3 = [test->test[1], 1, 2, 3];

        let this->a = test;
        let this->b = this->a->test2;

        let test1 = this->a;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1851
     */
    public function setPrivatevariable(value) -> <PropertyAccess>
    {
        let this->privateVariable = value;

        return this;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1851
     */
    public function getPrivatevariable() -> var
    {
        return this->privateVariable;
    }
}
