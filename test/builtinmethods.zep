
/**
 * OO operations
 */

namespace Test;

class BuiltInMethods
{
	public function stringMethodLength1()
	{
		return "hello"->length();
	}

	public function stringMethodLength2()
	{
		return ("hello")->length();
	}

	public function stringMethodLength3()
	{
		return ("hello" . "hello")->length();
	}

	public function stringMethodLength4(string a)
	{
		return a->length();
	}

	public function stringMethodLength5(string a)
	{
		return ("hello" . a)->length();
	}

	public function stringMethodTrim1()
	{
		return " hello \t\n"->trim();
	}

	public function arrayMethodJoin1(array a)
	{
		return a->join("|");
	}

}
