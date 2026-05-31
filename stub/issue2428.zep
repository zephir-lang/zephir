namespace Stub;

/**
 * Union return types — every member must survive into the compiled
 * extension's arginfo so PHP enforces and reflects the full union, exactly
 * like a hand-written PHP union return type.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2428
 */
class Issue2428
{
    public function scalarUnion(bool flag) -> int | string
    {
        return flag ? 1 : "x";
    }

    public function scalarUnionNullable(int n) -> int | string | null
    {
        return n;
    }

    public function classUnion(bool flag) -> <\stdClass> | <\ArrayObject>
    {
        return flag ? new \stdClass() : new \ArrayObject();
    }

    public function classUnionNullable(int n) -> <\stdClass> | <\ArrayObject> | null
    {
        return n > 0 ? new \stdClass() : null;
    }

    public function classOrScalar(bool flag) -> <\stdClass> | int
    {
        return flag ? new \stdClass() : 5;
    }

    public function arrayOrObject(bool flag) -> array | object
    {
        return flag ? [] : new \stdClass();
    }

    /**
     * Returns whatever it is given. Used to drive negative tests: the body
     * checker cannot see the dynamic value's type, so an out-of-union value
     * can reach the boundary at runtime.
     */
    public function dynamicReturn(var value) -> int | string
    {
        return value;
    }
}
