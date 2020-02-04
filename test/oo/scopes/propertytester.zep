namespace Test\Oo\Scopes;

class PropertyTester
{
    public function setPropertyObj(var obj, string property, var value)
    {
        let obj->{property} = value;

        return obj->{property};
    }

    public function setPropertyNew(string className, string property, var value)
    {
        var obj;

        let obj = new {className}();
        let obj->{property} = value;

        return obj;
    }

    public function getObjVars(var obj)
    {
        return get_object_vars(obj);
    }

    public function getNewVars(string className)
    {
        var obj;
        let obj = new {className}();

        return get_object_vars(obj);
    }
}
