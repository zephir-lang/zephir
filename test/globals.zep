
/**
 * Extension Globals
 */

namespace Test;

class Globals
{

	public function getDefaultGlobals1()
	{
		return globals_get("test.my_setting_1");
	}

	public function getDefaultGlobals2()
	{
		return globals_get("test.my_setting_2");
	}

	public function getDefaultGlobals3()
	{
		return globals_get("test.my_setting_3");
	}

	public function getDefaultGlobals4()
	{
		return globals_get("my_setting_1");
	}

	public function getDefaultGlobals5()
	{
		return globals_get("my_setting_2");
	}

	public function getDefaultGlobals6()
	{
		return globals_get("my_setting_3");
	}
}
