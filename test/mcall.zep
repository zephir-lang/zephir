
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

	private function testMethod7() -> <\stdClass>
	{
		return new \stdClass();
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

	public function testCall16(var c, var d)
	{
		return this->testMethod4(a: c, b: d);
	}

	public function testCall17(var c, var d)
	{
		return this->testMethod4(b: c, a: d);
	}

	public function testCall18()
	{
		return this->testMethod7();
	}

	public function testMethod19(long a, long b)
	{
		return a + b;
	}

	public function testCall20(long k, var p)
	{
		long i, j = 0;
		for i in range(1, k) {
			let j += this->testMethod19(p, p);
		}
		return j;
	}

	private function testMethod21(long a, long b)
	{
		return a + b;
	}

	public function testCall22(long k, var p)
	{
		long i, j = 0;
		for i in range(1, k) {
			let j += this->testMethod21(p, p);
		}
		return j;
	}

	public function optionalRequereString(string! param)
	{
		return param;
	}

	public function optionalParameterString(string param = "test string")
	{
		return param;
	}

	public function optionalParameterStringNull(string param = null)
	{
		return param;
	}

	public function optionalParameterInt(int param = 2)
	{
		return param;
	}

	public function optionalParameterVar(var param = null)
	{
		return param;
	}

	public function optionalParameterBoolTrue(bool param = true)
	{
		return param;
	}

	public function optionalParameterBoolFalse(bool param = false)
	{
		return param;
	}

	public function optionalParameterBoolean(boolean! start = true)
	{
		return start;
	}

	public function optionalParameterBooleanNull(bool! value = null)
	{
		return value;
	}

	/**
	 * @link https://github.com/phalcon/zephir/issues/142
	 */
	public function arrayParamWithDefaultEmptyArray(array driverOptions = [])
	{
		return driverOptions;
	}

	public function arrayParamWithDefaultNullValue(array driverOptions = null)
	{
		return driverOptions;
	}

	public function arrayParam(array driverOptions)
	{
		return driverOptions;
	}

	public function objectParamCastStdClass(<\StdClass> param)
	{
		return param;
	}

	public function objectParamCastOoParam(<Oo\Param> param)
	{
		return param;
	}

	private function bb()
	{
		return "hello";
	}

	public function testCallablePass()
	{
		var a;
		let a = new Oo\OoParams();

		return a->setCallable(function() {
			return 5;
		});
	}

	public function testCallableArrayThisMethodPass()
	{
		var a;
		let a = new Oo\OoParams();

		return a->setCallable([this, "bb"]);
	}

	public function aa()
	{
		var a;
		let a = new self;
		return a->bb();
	}
}
