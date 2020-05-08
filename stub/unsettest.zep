
/**
 * Unset statement tests
 */

namespace Stub;

class Unsettest
{
	protected property {get};

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

	public function testUnsetProperty()
	{
		unset(this->property);
		return this->property;
	}

	public function testStdClassUnset()
	{
		var simpleObject;
		let simpleObject = new \StdClass();

		let simpleObject->property1 = 12345;
		let simpleObject->property2 = "test";
		let simpleObject->property3 = 12345;

		unset(simpleObject->property2);
		return simpleObject;
	}

	public function testUnsetTypedArray(array arr, string key)
	{
        unset(arr[key]);
        return arr;
	}
}
