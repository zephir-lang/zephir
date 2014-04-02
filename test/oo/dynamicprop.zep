/**
 *
 */

namespace Test\Oo;

class DynamicProp
{
    public test;

    public function setPropertyInt(string! property)
    {
        let this->{property} = 10;
    }

    public function setPropertyBoolTrue(string! property)
    {
        let this->{property} = true;
    }

    public function setPropertyBoolFalse(string! property)
    {
        let this->{property} = false;
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

    public function setPropertyNull(string! property)
    {
        let this->{property} = null;
    }
}
