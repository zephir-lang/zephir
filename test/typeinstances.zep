
/**
 * Cast tests
 */

namespace Test;

class TypeInstances
{
	public function testInstanceOfString1()
	{
		return new string(24);
	}

    public function testInstanceOfString2()
	{
		return new string(-24);
	}

    public function testInstanceOfString3()
	{
		return new string(0);
	}    
}
