namespace TestFixture;

/**
 * Class with typed parameters for testing parameter handling
 */
class TypedParams
{
    public function intParam(int x) -> int
    {
        return x + 1;
    }

    public function stringParam(string s) -> string
    {
        return s;
    }

    public function arrayParam(array arr) -> array
    {
        return arr;
    }

    public function mixedParams(int a, string b, var c)
    {
        return a;
    }
}
