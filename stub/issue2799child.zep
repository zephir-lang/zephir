namespace Stub;

/**
 * Redeclares one of its parent's array defaults and adds its own.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
class Issue2799Child extends Issue2799
{
    protected literal = ["child"];

    public extra = ["x": []];
}
