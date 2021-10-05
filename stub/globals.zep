
namespace Stub;

class Globals
{
	/* Set Default Properties */

	 public function setBoolValueUsingDotNotation(value) -> void
	{
		globals_set("db.my_setting_1", value);
	}

	public function setIntValueUsingDotNotation(value) -> void
	{
		globals_set("db.my_setting_2", value);
	}

	public function setCharValue(value) -> void
	{
		globals_set("my_setting_4", value);
	}

	public function setStringValue(string value) -> void
	{
	    globals_set("my_setting_5", value);
	}

	public function setBoolValue(value) -> void
	{
		globals_set("my_setting_1", value);
	}

	public function setDefaultGlobalsOrmCacheLevel(value) -> void
	{
		globals_set("orm.cache_level", value);
	}

	/* Get Default Properties */

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals1()
	{
		return globals_get("db.my_setting_1");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals2()
	{
		return globals_get("db.my_setting_2");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals3()
	{
		return globals_get("db.my_setting_3");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals4()
	{
		return globals_get("my_setting_1");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals5()
	{
		return globals_get("my_setting_2");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals6()
	{
		return globals_get("my_setting_3");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobals7()
	{
		return globals_get("my_setting_4");
	}

	/**
     * @return mixed
     */
    public function getDefaultGlobals8()
    {
        return globals_get("my_setting_5");
    }

	/**
	 * @return mixed
	 */
	public function getDefaultGlobalsOrmCacheLevel()
	{
		return globals_get("orm.cache_level");
	}
}
