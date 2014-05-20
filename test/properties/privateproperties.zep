
namespace Test\Properties;

class PrivateProperties
{

	/**
	 * This is a private property with no initial value
	 */
	private someNull;

	/**
	 * This is a private property with initial null value
	 */
	private someNullInitial = null;

	/**
	 * This is a private property with initial boolean false
	 */
	private someFalse = false;

	/**
	 * This is a private property with initial boolean true
	 */
	private someTrue = true;

	/**
	 * This is a private property with an initial integer value
	 */
	private someInteger = 10;

	/**
	 * This is a private property with an initial double value
	 */
	private someDouble = 10.25;

	/**
	 * This is a private property with an initial string value
	 */
	private someString = "test";

	public function getSomeNull()
	{
		return this->someNull;
	}

	public function getSomeNullInitial()
	{
		return this->someNullInitial;
	}

	public function getSomeFalse()
	{
		return this->someFalse;
	}

	public function getSomeTrue()
	{
		return this->someTrue;
	}

	public function getSomeInteger()
	{
		return this->someInteger;
	}

	public function getSomeDouble()
	{
		return this->someDouble;
	}

	public function getSomeString()
	{
		return this->someString;
	}

}
