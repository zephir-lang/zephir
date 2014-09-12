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
}
