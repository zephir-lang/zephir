
/**
 * Unset statement tests
 */

namespace Test;

class Unsettest
{
	protected property;

	public function has(var key)
	{
		return isset this->property[key];
	}

	public function addValueToProperty(var key, var value)
	{
		let this->property[key] = value;
	}

	public function testUnsetValueFromProperty(var key)
	{
		unset(this->property[key]);
	}

	public function testUnsetFromArray(var arrayParameter)
	{
		unset(arrayParameter[0]);
		return arrayParameter;
	}

	public function testUnsetFromArrayByIndexVar(var arrayParameter, var index)
	{
		unset(arrayParameter[index]);
		return arrayParameter;
	}

	public function testUnsetTypedArray(array arr, string key)
	{
        unset(arr[key]);
        return arr;
	}
}
