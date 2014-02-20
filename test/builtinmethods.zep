
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

    public function stringMethodIndex(string str, string needle)
    {
        return str->index(needle);
    }

    public function stringMethodIndexWithPosition(string str, string needle, int position)
    {
        return str->index(needle, position);
    }

	public function stringMethodTrim1()
	{
		return " hello \t\n"->trim();
	}

	public function arrayMethodJoin1(array a)
	{
		return a->join("|");
	}

	public function arrayMethodReverse1(array a)
	{
		return a->$reverse();
	}

}
