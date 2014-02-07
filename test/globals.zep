
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

}
