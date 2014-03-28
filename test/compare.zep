namespace Test;

class Compare
{
    const PI = 3.14;

    public function isLessInt(int a, int b)
    {
        return a < b;
    }

    public function isLessDouble(double a, double b)
    {
        return a < b;
    }

    public function isLessThenPi(double a)
    {
        return a < self::PI;
    }

    public function isMoreThenPi(double a)
    {
        return self::PI < a;
    }
}