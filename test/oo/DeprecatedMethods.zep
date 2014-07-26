namespace Test\Oo;

class DeprecatedMethods
{
    public deprecated function deprecatedMethod()
    {
        return true;
    }

    public function normalMethod()
    {
        return this->privateDepricatedMethod();
    }

    private deprecated function privateDepricatedMethod()
    {
        return true;
    }
}
