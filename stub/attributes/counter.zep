namespace Stub\Attributes;

/**
 * Attributes on trait members. Zephir merges traits at compile time, so the
 * using class re-instantiates these members from the same raw AST nodes and
 * therefore carries their attributes too, which is what PHP does natively
 * (#2466).
 */
#[Marker("on-a-trait")]
trait Counter
{
    #[Marker("trait-property")]
    public int hits = 0;

    #[Marker("trait-method")]
    public function bump() -> int
    {
        let this->hits = this->hits + 1;

        return this->hits;
    }
}
