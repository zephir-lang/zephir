namespace Stub;

/**
 * Static property accesses run many times in one call, so every one of them
 * is answered from its cache record after the first.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
class Issue2805
{
    public static items = [];
    public static num = 0;
    public static flag = null;
    protected static text = "";

    public static function hits(int n) -> array
    {
        int i = 0;
        var total = 0, k;

        let self::items = [];
        let self::num = 0;
        let self::text = "";

        while i < n {
            let self::num += 2;
            let self::num -= 1;
            let total = total + self::num;
            let self::items[] = i;
            let k = "k" . (i % 3);
            let self::items[k] = i;
            let self::items[k] += 1;
            let self::text .= "t";
            let i++;
        }

        unset self::items["k0"];

        return [
            self::num,
            total,
            count(self::items),
            self::items["k1"],
            isset self::items["k1"],
            isset self::items["k0"],
            isset self::flag,
            strlen(self::text)
        ];
    }

    /**
     * Reads a property __get() answers, as an owned copy, as a condition and
     * as an argument, which used to leak the value __get() returned.
     */
    public static function readMagic(var o, int n) -> int
    {
        int i = 0, c = 0;
        var v;

        while i < n {
            let v = o->bag;
            if o->bag {
                let c++;
            }
            let c += count(o->bag);
            let i++;
        }

        return c + count(v);
    }
}
