
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

	public function setDefaultGlobalsOrmCachePrefix(string value) -> void
	{
	    globals_set("orm.cache_prefix", value);
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

	/**
     * @return mixed
     */
    public function getDefaultGlobalsOrmCachePrefix()
    {
        return globals_get("orm.cache_prefix");
    }

	/* Globals that had no accessor at all before #2449 */

	public function setUIntValue(value) -> void
	{
		globals_set("my_setting_6", value);
	}

	public function setIntValue(value) -> void
	{
		globals_set("my_setting_2", value);
	}

	public function setDoubleValue(value) -> void
	{
		globals_set("my_setting_3", value);
	}

	/**
	 * @return mixed
	 */
	public function getUIntValue()
	{
		return globals_get("my_setting_6");
	}

	/**
	 * @return mixed
	 */
	public function getTestSetting1()
	{
		return globals_get("test_setting_1");
	}

	/**
	 * @return mixed
	 */
	public function getDefaultGlobalsOrmCacheEnable()
	{
		return globals_get("orm.cache_enable");
	}

	/**
	 * @return mixed
	 */
	public function getTestIniVariable()
	{
		return globals_get("extension.test_ini_variable");
	}

	/**
	 * @return mixed
	 */
	public function getModuleSetting()
	{
		return globals_get("module_setting");
	}

	public function setModuleSetting(value) -> void
	{
		globals_set("module_setting", value);
	}
}
