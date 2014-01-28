
/**
 *
 */

namespace Test\Oo;

class OoParams
{

	public function setAge(int age)
	{
		return age;
	}

	public function setAverage(double average)
	{
		return average;
	}

	public function setName(string name)
	{
		return name;
	}

	public function setEnabled(boolean enabled)
	{
		return enabled;
	}

	public function setList(array someList)
	{
		return someList;
	}

	public function setStrictAge(int! age)
	{
		return age;
	}

	public function setStrictAverage(double! average)
	{
		return average;
	}

	public function setStrictName(string! name)
	{
		return name;
	}

	public function setStrictEnabled(boolean enabled)
	{
		return enabled;
	}

	public function setStrictList(array! someList)
	{
		return someList;
	}

	public function setConstAge(const int age)
	{
		return age;
	}

	public function setConstAverage(const double average)
	{
		return average;
	}

}
