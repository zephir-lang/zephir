namespace Test\BuiltIn;

class ArrayMethods
{
    public function getReversed(string str)
    {
        var arr;
        let arr = [0, 1, 2, 3, 4, 5];
        return arr->rev();
    }
}