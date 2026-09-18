namespace Stub\Attributes;

/**
 * A top-level function carrying attributes on itself and on a parameter.
 *
 * A function has no class initializer to write into, so these are attached from
 * MINIT next to the class inits; the engine registers a module's functions
 * before MINIT runs, so `CG(function_table)` already holds them (#2466).
 */
#[Marker("on-a-function", number: 4 + 3)]
function tagged(#[Marker("on-a-function-parameter")] int a) -> int
{
    return a;
}

class Tools
{
    public function callTagged(int a) -> int
    {
        return tagged(a);
    }
}
