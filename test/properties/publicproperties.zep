
namespace Test\Properties;

class PublicProperties
{
    const TEST_CONST = 123;

	/**
	 * This is a public property with no initial value
	 */
	public someNull;

	/**
	 * This is a public property with initial null value
	 */
	public someNullInitial = null;

	/**
	 * This is a public property with initial boolean false
	 */
	public someFalse = false;

	/**
	 * This is a public property with initial boolean true
	 */
	public someTrue = true;

	/**
	 * This is a public property with an initial integer value
	 */
	public someInteger = 10;

	/**
	 * This is a public property with an initial double value
	 */
	public someDouble = 10.25;

	/**
	 * This is a public property with an initial string value
	 */
	public someString = "test";

	/**
	 * This is a public property with an initial empty-array value
	 */
	public someEmptyArray = [];

	/**
	 * This is a public property with an initial array value
	 */
	public someArray = [1, 2, 3, 4];

	public testPropertyFromSelfConstantValue = self::TEST_CONST;

	public testPropertyFromClassNameConstantValue = PublicProperties::TEST_CONST;
}