namespace Stub\Types;

/**
 * Union-typed parameters are checked and coerced on entry exactly like a
 * PHP union parameter.
 */
class UnionParams
{
    public function number(int|float value)
    {
        return value;
    }

    public function scalar(string|bool value)
    {
        return value;
    }

    public function floatOrString(double|string value)
    {
        return value;
    }

    public function intOrFalse(int|false value)
    {
        return value;
    }

    public function objectOrArray(<MayBe>|array value)
    {
        return value;
    }

    public function classes(<MayBe>|<\ArrayObject> value)
    {
        return value;
    }

    public function second(string label, int|float value)
    {
        return value;
    }

    public function optional(int|string value = 5)
    {
        return value;
    }

    public function nullDefault(int|string value = null)
    {
        return value;
    }

    public static function staticNumber(int|float value)
    {
        return value;
    }
}
