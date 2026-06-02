namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1628
 */
class Issue1628 extends Issue1628Abstract
{
    public function mustImplement() -> string
    {
        return "implemented";
    }
}
