namespace Stub\Attributes;

/**
 * An interface carries attributes on itself, on a constant and on a method
 * (including its parameters). Interface members reach the compiler through a
 * different grammar path than class members (#2466).
 */
#[Marker("on-an-interface")]
interface Contract
{
    #[Marker]
    const LIMIT = 10;

    #[Marker]
    public function handle(#[Marker] string secret);
}
