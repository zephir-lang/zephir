
namespace Test\Properties;

class ProtectedProperties
{

	/**
	 * This is a protected property with no initial value
	 */
	protected someNull;

	/**
	 * This is a protected property with initial null value
	 */
	protected someNullInitial = null;

	/**
	 * This is a protected property with initial boolean false
	 */
	protected someFalse = false;

	/**
	 * This is a protected property with initial boolean true
	 */
	protected someTrue = true;

	/**
	 * This is a protected property with an initial integer value
	 */
	protected someInteger = 10;

	/**
	 * This is a protected property with an initial double value
	 */
	protected someDouble = 10.25;

	/**
	 * This is a protected property with an initial string value
	 */
	protected someString = "test";

	protected someVar {set, get};

	public function getSomeNull()
	{
		return this->someNull;
	}

	public function setSomeNull(var param)
	{
		let this->someNull = param;
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
