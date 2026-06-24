namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2469
 *
 * Reproducer for "Cannot pass literal char into a function expecting char".
 * Crashes Zephir compilation prior to the fix.
 */
final class Issue2469
{
    public function peek(char ch) -> char
    {
        return ch;
    }

    public function callWithLiteral() -> char
    {
        return this->peek('a');
    }

    public function callWithVariable() -> char
    {
        char chTmp;
        let chTmp = '>';
        return this->peek(chTmp);
    }

    public function callWithLiteralUchar() -> char
    {
        return this->peek('Z');
    }
}
