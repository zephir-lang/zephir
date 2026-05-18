namespace Stub;

class Issue820
{
    public static function testIterator(var obj)
    {
        var k, v, arr = [];

        for k, v in iterator(obj) {
            let arr[k] = v;
        }

        return arr;
    }
}
