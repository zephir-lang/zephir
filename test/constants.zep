
namespace Test;

class Constants extends Test\ConstantsParent
{
	const C1 = null;

	const C2 = false;

	const C3 = true;

	const C4 = 10;

	const C5 = 10.25;

	const C6 = "test";

	public function testReadConstant()
	{
		return ENT_QUOTES;
	}

	public function testReadClassConstant1()
	{
		return Test\Constants::C4;
	}

	public function testReadClassConstant2()
	{
		return self::C4;
	}

	public function testReadClassConstant3()
	{
		return parent::P4;
	}

	public function testPHPVersionEnvConstant()
	{
	    return PHP_VERSION;
	}

	public function testClassMagicConstant()
	{
	    return __CLASS__;
	}

	public function testMethodMagicConstant()
	{
	    return __METHOD__;
	}

	public function testNamespaceMagicConstant()
	{
	    return __NAMESPACE__;
	}
}