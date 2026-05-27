
namespace Stub;

/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/808
 *
 * unset(obj->{variable}) was not supported and threw:
 *   CompilerException: Cannot use expression type: property-dynamic-access in "unset"
 *
 * Fix: UnsetStatement must resolve the dynamic property name from the symbol
 * table and emit zephir_unset_property_zval() instead of rejecting the node.
 */
class Issue808
{
    /**
     * Unset a stdClass property by a variable holding the property name.
     * After the unset the property must no longer exist (isset returns false).
     */
    public function testDynamicUnset(var propertyName) -> bool
    {
        var obj;
        let obj = new \stdClass();
        let obj->{propertyName} = "value";
        unset(obj->{propertyName});
        return isset(obj->{propertyName});
    }

    /**
     * Only the targeted property is removed; others survive.
     */
    public function testDynamicUnsetSelective(var toRemove) -> var
    {
        var obj;
        let obj = new \stdClass();
        let obj->keep = "keep_value";
        let obj->remove = "remove_value";
        unset(obj->{toRemove});
        return obj;
    }

    /**
     * Unset using a string literal in braces: unset(obj->{"name"}).
     * This exercises property-string-access in UnsetStatement.
     * The post-unset check uses regular property-access (not brace syntax)
     * because isset(obj->{"name"}) has a separate known limitation.
     */
    public function testDynamicUnsetStringKey() -> bool
    {
        var obj;
        let obj = new \stdClass();
        let obj->name = "value";
        unset(obj->{"name"});
        return isset(obj->name);
    }
}
