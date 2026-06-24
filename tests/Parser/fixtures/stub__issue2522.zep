namespace Stub;

class Issue2522
{
    /**
     * Issue #2522 ask: inline closure init must compile. Comparing against
     * the two-step form (which has worked for years) for parity.
     */
    public function inlineClosureIsCallable() -> bool
    {
        var call = function () {
            return 42;
        };

        return is_callable(call);
    }

    public function inlineClosureReturnsClosureInstance() -> bool
    {
        var call = function () {
            return 42;
        };

        return (call instanceof \Closure);
    }

    public function inlineFnKeywordIsCallable() -> bool
    {
        var call = fn () {
            return 42;
        };

        return is_callable(call);
    }

    public function twoStepClosureIsCallable() -> bool
    {
        var call;
        let call = function () {
            return 42;
        };

        return is_callable(call);
    }
}
