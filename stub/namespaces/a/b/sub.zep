namespace Stub\Namespaces\A\B;

class Sub
{
    public value;

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2686
     */
    public function issue2686ClosureNew()
    {
        return function () {
            return new Sub();
        };
    }

    public function issue2686ArrowInstanceOf()
    {
        return n => n instanceof Sub;
    }
}
