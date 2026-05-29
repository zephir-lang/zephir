namespace Stub;

/**
 * Regression coverage for issue #2167: a `[ClassName, "method"]` array
 * passed as a PHP callback (e.g. to array_reduce) fails PHP's callable
 * validation when the method is `protected` or `private`, because Zephir
 * methods are ZEND_INTERNAL_FUNCTION and PHP's scope-walker
 * (`zend_get_executed_scope()`) skips them when looking for the calling
 * class. Same root cause as #2321.
 *
 * The supported workaround is to wrap the callback in a Zephir closure
 * that delegates via a fully-qualified static-call expression. Closures
 * auto-bind to the enclosing class scope, so the delegating call passes
 * PHP's visibility check.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2167
 */
class Issue2167
{
    protected a;

    public function __construct(array data)
    {
        let this->a = data;
    }

    /**
     * The broken pattern from the issue: a [class, "method"] array passed
     * directly as a callback. PHP rejects this at the callable-validation
     * step before the callback ever runs. Kept here so the regression
     * test can lock down the documented failure mode.
     */
    public function reduceWithArrayCallable() -> string
    {
        return (string) array_reduce(
            this->a,
            ["Stub\\Issue2167", "implodeRow"],
            ""
        );
    }

    /**
     * The supported workaround: a closure that calls the protected
     * static method via its fully-qualified absolute class name. The
     * closure is implicitly bound to the enclosing class, so visibility
     * checks see `Stub\Issue2167` as the caller.
     */
    public function reduceWithClosureWrapper() -> string
    {
        return (string) array_reduce(
            this->a,
            function (carry, row) {
                return \Stub\Issue2167::implodeRow(carry, row);
            },
            ""
        );
    }

    protected static function implodeRow(const string carry, const array row) -> string
    {
        return carry . "[ " . implode(" ", row) . " ]";
    }
}
