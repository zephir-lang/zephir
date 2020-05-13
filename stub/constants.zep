namespace Stub;

class Constants extends ConstantsParent
{
	const C1 = null;

	const C2 = false;

	const C3 = true;

	const C4 = 10;

	const C5 = 10.25;

	const C5_SELF = self::C5;

	const C5_BY_NAME = Constants::C5;

	const C6 = "test";

	const className = __CLASS__;

	const STD_PROP_LIST = \ArrayObject::STD_PROP_LIST;

	/** Test Issue 1571 */
	const DEFAULT_PATH_DELIMITER  = ".";
	const PROPERTY_WITH_VARS = "$SOME/CSRF/KEY$";

	/**
	* Test property addSlashes for constants
	*/
	const ANNOTATION_REGEX = '/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/';
	const PARAMETER_REGEX = '/(\w+)\s*=\s*(\[[^\]]*\]|"[^"]*"|[^,)]*)\s*(?:,|$)/';

	protected propWsVarsAssigned = "$SOME/CSRF/KEY$";
	protected propWsVarsGet = self::PROPERTY_WITH_VARS {get};

	protected propertyC1 = self::C1 {get};
	protected propertyC2 = self::C2 {get};
	protected propertyC3 = self::C3 {get};
	protected propertyC4 = self::C4 {get};
	protected propertyC5 = self::C5 {get};
	protected propertyC6 = self::C6 {get};

	protected propertyC7 = self::ANNOTATION_REGEX {get};
	protected propertyC8 = self::PARAMETER_REGEX {get};

	// Do not modify annotation bellow
	// See:
	// https://github.com/phalcon/php-zephir-parser/issues/13
	// https://github.com/phalcon/cphalcon/pull/11212/files

	/**
	 * @var \Phalcon\Cache\FrontendInterface
	 */
	protected propertyC9 = "some-value" {get};

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

	/**
	 * Test Delimiters as String Constants
	 *
	 * @link https://github.com/phalcon/zephir/issues/1571
	 */
	public function testStringDelimiterConstantDoubleQuoted()
	{
		var delimiter;
		let delimiter = Constants::DEFAULT_PATH_DELIMITER;

		return delimiter;
	}

	public function testStringConstantWithVars()
	{
		var property;
		let property = Constants::PROPERTY_WITH_VARS;

		return property;
	}

	public function testStringPropertyWithVarsAssigned()
	{
		return this->propWsVarsAssigned;
	}

	public function testStringPropertyWithVarsGet()
	{
		return this->propWsVarsGet;
	}
}
