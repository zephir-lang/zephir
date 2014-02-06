
namespace Test\Properties;

class StaticProtectedProperties
{

	/**
	 * This is a protected property with no initial value
	 */
	protected static someNull;

	/**
	 * This is a protected property with initial null value
	 */
	protected static someNullInitial = null;

	/**
	 * This is a protected property with initial boolean false
	 */
	protected static someFalse = false;

	/**
	 * This is a protected property with initial boolean true
	 */
	protected static someTrue = true;

	/**
	 * This is a protected property with an initial integer value
	 */
	protected static someInteger = 10;

	/**
	 * This is a protected property with an initial double value
	 */
	protected static someDouble = 10.25;

	/**
	 * This is a protected property with an initial string value
	 */
	protected static someString = "test";

	protected static someVar {set, get};

	public static function getSomeNull()
	{
		return self::someNull;
	}

	public static function setSomeNull(var param)
	{
		let self::someNull = param;
	}

	public static function getSomeNullInitial()
	{
		return self::someNullInitial;
	}

	public static function getSomeFalse()
	{
		return self::someFalse;
	}

	public static function getSomeTrue()
	{
		return self::someTrue;
	}

	public static function getSomeInteger()
	{
		return self::someInteger;
	}

	public static function getSomeDouble()
	{
		return self::someDouble;
	}

	public static function getSomeString()
	{
		return self::someString;
	}

}
