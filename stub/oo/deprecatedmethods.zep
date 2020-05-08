namespace Stub\Oo;

class DeprecatedMethods
{
    public deprecated function publicDeprecated()
    {
        return true;
    }

    public function normalMethod()
    {
        return this->privateDeprecated();
    }

    private deprecated function privateDeprecated()
    {
        return true;
    }
}
