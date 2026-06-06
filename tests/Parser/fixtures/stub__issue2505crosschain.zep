namespace Stub;

/**
 * Regression coverage for the cross-class chained `<static>` resolution.
 *
 * A method declared `-> <static>` on `Issue2505` is called via a LOCAL
 * variable (not `this`), and the result is immediately chained. Before
 * the fix, `MethodCall.php` resolved `<static>` to the LEXICAL class of
 * the call site (i.e. `Stub\Issue2505CrossChain`) instead of the
 * receiver's class (`Stub\Issue2505`). Compile-time method lookup for
 * `checkPrivate()` then ran against `Issue2505CrossChain` — which has
 * no such method — and the build aborted with:
 *
 *     Class 'Stub\Issue2505CrossChain' does not implement method: 'checkPrivate'
 *
 * After the fix, `static`/`self`/`parent` resolve against the receiver's
 * class definition (the one already used to look up the called method),
 * so the chained lookup correctly finds `Issue2505::checkPrivate()`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
class Issue2505CrossChain
{
    public function crossClassChain(<Issue2505> instance) -> bool
    {
        return instance->makeStatic()->checkPrivate();
    }

    public function crossClassChainSelf(<Issue2505> instance) -> bool
    {
        return instance->makeSelf()->checkPrivate();
    }
}