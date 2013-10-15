
/**
 * Method calls
 */

namespace Test;

class Mcall
{
	public function testMethod1()
	{
		return "hello public";
	}

	protected function testMethod2()
	{
		return "hello protected";
	}

	private function testMethod3()
	{
		return "hello private";
	}

	public function testMethod4(var a, var b)
	{
		return a + b;
	}

	protected function testMethod5(var a, var b)
	{
		return a + b;
	}

	private function testMethod6(var a, var b)
	{
		return a + b;
	}

	public function testCall1()
	{
		return this->testMethod1();
	}

	public function testCall2()
	{
		return this->testMethod2();
	}

	public function testCall3()
	{
		return this->testMethod3();
	}

	public function testCall4(var a, var b)
	{
		return this->testMethod4(a, b);
	}

	public function testCall5(var a, var b)
	{
		return this->testMethod5(a, b);
	}

	public function testCall6(var a, var b)
	{
		return this->testMethod6(a, b);
	}

	public function testCall7(var a, var b)
	{
		return this->testMethod4(a, b) + this->testMethod4(a, b);
	}

	public function testCall8(var a, var b)
	{
		return this->testMethod5(a, b) + this->testMethod5(a, b);
	}

	public function testCall9(var a, var b)
	{
		return this->testMethod6(a, b) + this->testMethod5(a, b);
	}

	public function testCall10()
	{
		return this->{"testMethod1"}();
	}

	public function testCall11()
	{
		return this->{"testMethod2"}();
	}

	public function testCall12()
	{
		return this->{"testMethod3"}();
	}

	public function testCall13(var a, var b)
	{
		return this->{"testMethod4"}(a, b);
	}

	public function testCall14(var a, var b)
	{
		return this->{"testMethod5"}(a, b);
	}

	public function testCall15(var a, var b)
	{
		return this->{"testMethod6"}(a, b);
	}

}
