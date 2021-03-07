namespace Stub\Oo\Scopes;

class PrivateScopeTester extends HasPrivateMethod implements ScopeTesterInterface
{
    public function run() -> string
    {
        return this->callPrivateMethod();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2057
     */
    public function setPropertyObj(var obj, string property, var value) -> var
    {
        let obj->{property} = value;

        return obj->{property};
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2057
     */
    public function setPropertyNew(string className, string property, var value) -> var
    {
        var obj;

        let obj = create_instance(className);
        let obj->{property} = value;

        return obj;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2057
     */
    public function getObjVars(var obj) -> var
    {
        return get_object_vars(obj);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2057
     */
    public function getNewVars(string className) -> var
    {
        return this->getObjVars(create_instance(className));
    }
}
