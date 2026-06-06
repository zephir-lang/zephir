
namespace Stub\Properties;

class StaticPublicProperties
{

	/**
	 * This is a public property with no initial value
	 */
	public static someNull;

	/**
	 * This is a public property with initial null value
	 */
	public static someNullInitial = null;

	/**
	 * This is a public property with initial boolean false
	 */
	public static someFalse = false;

	/**
	 * This is a public property with initial boolean true
	 */
	public static someTrue = true;

	/**
	 * This is a public property with an initial integer value
	 */
	public static someInteger = 10;

	/**
	 * This is a public property with an initial double value
	 */
	public static someDouble = 10.25;

	/**
	 * This is a public property with an initial string value
	 */
	public static someString = "test";

	public static someAdd = 0;
	public static someSub = 0;

	public static function setSomeString(val)
	{
		let self::someString = val;
	}

	public static function testAddAndSub()
	{
		let self::someAdd += 1;
		let self::someSub -= 1;
	}

	public static function testAddAndSub2()
	{
		let self::someAdd += "2string";
		let self::someSub -= "2string";
	}

	public static function testAddAndSub3()
	{
		let self::someAdd += 1.0;
		let self::someSub -= 1.0;
	}

	public static function testAddAndSub4()
	{
		var v;
		let v = 1;
		let self::someAdd += v;
		let self::someSub -= v;
	}

	public static function testAddAndSub5()
	{
		var v;
		let v = "1";
		let self::someAdd += v;
		let self::someSub -= v;
	}
}
