namespace Stub;

/**
 * One method call site, for the test that runs it in two requests whose
 * classes may share an address.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2790
 */
class Issue2790
{
    public function callWho(var o) -> string
    {
        return o->who();
    }
}
