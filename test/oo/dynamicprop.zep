/**
 *
 */

namespace Test\Oo;

class DynamicProp
{
    protected test;

	public function setPropertyInt(string! property)
	{
		let this->{property} = 10;
	}

	public function setPropertyBool(string! property)
	{
		let this->{property} = true;
	}

	public function setPropertyString(string! property)
	{
		let this->{property} = "string";
	}

	public function setPropertyChar(string! property)
	{
	    char a = 'A';
		let this->{property} = a;
	}

	public function setPropertyUChar(string! property)
	{
	    uchar a = 'A';
		let this->{property} = a;
	}

	public function getProperty(string! property)
	{
        return this->{property};
	}
}
