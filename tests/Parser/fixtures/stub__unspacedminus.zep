namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2011
 */
class UnspacedMinus
{
    public function rangeLen(int len) -> array
    {
        var i, out = [];
        for i in range(0, len-1) {
            let out[] = i;
        }
        return out;
    }

    public function glued(int a) -> int
    {
        return a-1;
    }

    public function spaceBefore(int a) -> int
    {
        return a -1;
    }

    public function arrayIndex(array a) -> int
    {
        return a[0]-1;
    }

    public function negativeLiteral() -> int
    {
        return -1;
    }

    public function gluedAdd(int a) -> int
    {
        return a+1;
    }

    public function gluedMul(int a) -> int
    {
        return a*2;
    }

    public function gluedDiv(int a) -> double
    {
        return a/2;
    }

    public function gluedMod(int a) -> int
    {
        return (int) (a%5);
    }

    public function mulByNegative(int a) -> int
    {
        return a*-1;
    }

    public function chained(int a) -> int
    {
        return a*2-1;
    }
}
