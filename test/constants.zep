namespace Test;

class Constants extends ConstantsParent
{
	const C1 = null;

	const C2 = false;

	const C3 = true;

	const C4 = 10;

	const C5 = 10.25;

	const C6 = "test";

	const className = __CLASS__;

	const STD_PROP_LIST = \ArrayObject::STD_PROP_LIST;

	/**
	 * Test property addSlashes for constants
	 */
	const ANNOTATION_REGEX = '/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/';
	const PARAMETER_REGEX = '/(\w+)\s*=\s*(\[[^\]]*\]|"[^"]*"|[^,)]*)\s*(?:,|$)/';

	protected propertyC1 = self::C1 {get};
	protected propertyC2 = self::C2 {get};
	protected propertyC3 = self::C3 {get};
	protected propertyC4 = self::C4 {get};
	protected propertyC5 = self::C5 {get};
	protected propertyC6 = self::C6 {get};

	protected propertyC7 = self::ANNOTATION_REGEX {get};
	protected propertyC8 = self::PARAMETER_REGEX {get};

	public function testReadConstant()
	{
		return ENT_QUOTES;
	}

	public function testReadClassConstant1()
	{
		return Constants::C4;
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

	public function testFunctionMagicConstant()
	{
	    return __FUNCTION__;
	}

	public function testNamespaceMagicConstant()
	{
	    return __NAMESPACE__;
	}

	public function testDirConstant()
	{
	    return __DIR__;
	}

	public function testPHPVersionEnvConstantInExpValue()
	{
	    var a;
	    let a = PHP_VERSION;
	    return a;
	}
}
