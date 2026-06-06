
namespace Stub\Properties;

/**
 * Stub\Properties\ProtectedProperties
 */
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

    /**
     * @var null|mixed
     */
	protected someVar {set, get};

    /**
     * This is a property to test default value on extends
     * @var array
     */
    protected someArrayVar = [] { set, get };

    /**
     * @return null|mixed
     */
	public function getSomeNull()
	{
		return this->someNull;
	}

    /**
     * @return void
     */
	public function setSomeNull(var param)
	{
		let this->someNull = param;
	}

    /**
     * @return null
     */
	public function getSomeNullInitial()
	{
		return this->someNullInitial;
	}

    /**
     * @return bool
     */
	public function getSomeFalse()
	{
		return this->someFalse;
	}

    /**
     * @return bool
     */
	public function getSomeTrue()
	{
		return this->someTrue;
	}

    /**
     * @return int
     */
	public function getSomeInteger()
	{
		return this->someInteger;
	}

    /**
     * @return float
     */
	public function getSomeDouble()
	{
		return this->someDouble;
	}

    /**
     * @return string
     */
	public function getSomeString()
	{
		return this->someString;
	}
}
