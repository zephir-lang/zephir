
/**
 * Variable declaration
 */

namespace Test;

class DeclareTest
{
    public function testStringDeclare1()
    {
        var a = "/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/";
        return a;
    }

    public function testStringDeclare2()
    {
        var a = "/(\w+)\s*=\s*(\[[^\]]*\]|\"[^\"]*\"|[^,)]*)\s*(?:,|$)/";
        return a;
    }

	public function testDeclare1()
	{
		int a = 1;
		return a;
	}

	public function testDeclare2()
	{
		uint a = 1;
		return a;
	}

	public function testDeclare3()
	{
		double a = 1.0;
		return a;
	}

	public function testDeclare4()
	{
		float a = 1.0;
		return a;
	}

	public function testDeclare5()
	{
		char a = 'A';
		return a;
	}

	public function testDeclare6()
	{
		uchar a = 'A';
		return a;
	}

	public function testDeclare7()
	{
		long a = 1;
		return a;
	}

	public function testDeclare8()
	{
		boolean a = true;
		return a;
	}

	public function testDeclare9()
	{
		boolean a = false;
		return a;
	}

	public function testDeclare10()
	{
		var a = 10;
		return a;
	}

	public function testDeclare11()
	{
		var a = 10.5;
		return a;
	}

	public function testDeclare12()
	{
		var a = false;
		return a;
	}

	public function testDeclare13()
	{
		var a = true;
		return a;
	}

	public function testDeclare14()
	{
		var a = "hello";
		return a;
	}

	public function testDeclare15()
	{
		var a = null;
		return a;
	}
}