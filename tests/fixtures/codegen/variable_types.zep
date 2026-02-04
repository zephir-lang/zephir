namespace TestFixture;

/**
 * Class with various variable types for testing variable declaration
 */
class VariableTypes
{
    public function intVariables()
    {
        int a = 1;
        int b = 2;
        return a + b;
    }

    public function longVariables()
    {
        long x = 1000;
        long y = 2000;
        return x + y;
    }

    public function doubleVariables()
    {
        double pi = 3.14159;
        double e = 2.71828;
        return pi + e;
    }

    public function stringVariables()
    {
        string s1 = "hello";
        string s2 = "world";
        return s1;
    }

    public function boolVariables()
    {
        bool flag1 = true;
        bool flag2 = false;
        return flag1;
    }

    public function varVariables()
    {
        var x = 1;
        var y = "test";
        var z = null;
        return x;
    }

    public function arrayVariables()
    {
        array arr = [];
        return arr;
    }

    public function mixedDeclarations()
    {
        int count = 0;
        string name = "test";
        var data = null;
        array items = [];
        bool flag = true;
        double ratio = 1.5;
        return count;
    }
}
