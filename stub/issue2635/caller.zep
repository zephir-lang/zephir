namespace Stub\Issue2635;

/**
 * Calling an inherited interface method on a variable typed as the child
 * interface, rather than on `this`.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2635
 */
class Caller
{
    public function call(<Outer> obj) -> string
    {
        return obj->go();
    }
}
