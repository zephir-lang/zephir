namespace Test;

class Operator
{
    public function testIdentical(param1, param2)
    {
        return param1 === param2;
    }

    public function testExponential(a, b, c)
    {
        return a ** b ** c;
    }

    public function testExponential2()
    {
        return 3 ** 4;
    }

    public function testExponential3(int a, int b)
    {
        return a ** b;
    }
}
