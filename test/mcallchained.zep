
/**
 * Chained method calls
 */

namespace Test;

class McallChained
{
	private temp;

	public function testMethod1()
	{
		return this;
	}

	protected function testMethod2()
	{
		return this;
	}

	private function testMethod3()
	{
		return this;
	}

	public function testChained1()
	{
		return this->testMethod1()
				   ->testMethod2();
	}

	public function testChained2()
	{
		return this->testMethod1()
				   ->testMethod3()
				   ->testMethod2();
	}

	public function testChained3()
	{
		return this->testMethod3()
				   ->testMethod2()
				   ->testMethod1();
	}

	public function testChained4()
	{
		let this->temp = this;
		return this->temp->testMethod1()->testMethod2();
	}
}
