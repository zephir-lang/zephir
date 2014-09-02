
/**
 *
 */

namespace Test\Oo;

class OoParams
{
	public function createThisClassWithoutWriteCurrentNamespace()
	{
		return new OoParams();
	}

	public function createOtherClassWithoutWriteCurrentNamespace()
	{
		return new OoDynamicA();
	}

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

	public function setAgeDefault(int age=25)
	{
		return age;
	}

	public function setAverageDefault(double average=25)
	{
		return average;
	}

	public function setNameDefault(string name="peter")
	{
		return name;
	}

	public function setEnabledDefault(boolean enabled=false)
	{
		return enabled;
	}

	public function setListDefault(array someList=[1, 2, 3])
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

	public function setObject(object obj)
	{
	    return obj;
	}

	public function setCallable(callable obj)
	{
	    return obj;
	}

	public function setResource(resource obj)
	{
	    return obj;
	}

	public function setObjectClassCast(<Param> parameter)
	{
		return parameter;
	}
}
