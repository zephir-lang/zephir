
/**
 * Arithmetic operations
 */

namespace Test;

class Ternary
{
	public function testTernary1()
	{
		return 100 ? true + 100 : false;
	}

    public function testTernary2(boolean b)
    {
        return b ? "foo" : "bar";
    }

	public function testTernaryComplex1(var a, var y)
	{
		return 100 ? true + 100 : a->y() ? a->x() : isset a[y];
	}

	public function testTernaryComplex2(var a, var y)
	{
		return 5 + (100 ? true + 100 : a->y() ? a->x() : isset a[y]);
	}

	public function testTernaryComplex3(var a)
	{
		//return gettype(typeof a == "resource" ? "unknown": false);
	}

    /**
     * @link https://github.com/phalcon/zephir/issues/297
     */
    public function testTernaryAfterLetVariable()
    {
        var s = 23;
        let s = 1===1 ? 3 : 10;
    }
}
