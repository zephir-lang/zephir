
/**
 * X
 */

namespace Test;

class Cast
{
    /** To int cast */

	public function testIntCastFromFloat()
	{
		return (int) 5.0;
	}

	public function testIntCastFromBoolean()
	{
		return (int) true;
	}

    /** To boolean cast */

    public function testBooleanCastFromIntTrue1()
    {
        return (boolean) 1;
    }

    public function testBooleanCastFromIntTrue2()
    {
        return (boolean) 1000;
    }

    public function testBooleanCastFromIntFalse()
    {
        return (boolean) 0;
    }

    public function testBooleanCastFromObject()
    {
        var simpleObject;
        let simpleObject = new \StdClass();
        return (boolean) simpleObject;
    }

    public function testBooleanCastFromEmptyArray()
    {
        var a = [];
        return (boolean) a;
    }

    public function testBooleanCastFromArray()
    {
        var a;
        let a = [1, 2, 3, 4];
        return (boolean) a;
    }

    public function testBooleanCaseFromNull()
    {
        var a = null;
        return (boolean) a;
    }
}
