
namespace Stub;

class Scope
{
    private static function getStr()
    {
        return "internal_string";
    }

    private static function getDyStr(int g) {
        return "internal_" . g;
    }

    public static function test1()
    {
        var ret = "";
        var k = self::getStr();
        var r = 1;
        if r == 1 {
            var k = "test";
            let ret .= k;
        }
        return ret . k;
    }

    public static function test2()
    {
        int p = 15;
        var k = "";
        var c;
        for c in range(0, 3) {
            int p = 66;
            let k = k . (c * p)."_";
        }
        return [p, k];
    }

    public static function test3()
    {
        var k = "", c;
        for c in range(0, 3) {
            var str = self::getDyStr(c);
            let k .= str;
        }
        return k;
    }
}
