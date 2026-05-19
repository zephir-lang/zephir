namespace Stub;

class Issue2505
{
    public function makeSelf() -> <self>
    {
        return new self();
    }

    public static function makeSelfStatic() -> <self>
    {
        return new self();
    }

    public function makeStatic() -> <static>
    {
        return new static();
    }

    public static function makeStaticFromStatic() -> <static>
    {
        return new static();
    }

    /**
     * Chained call on a <static> return: the COMPILE-TIME type tracker must
     * resolve the receiver of `checkPrivate()` to the lexical class. Without
     * the fix, this triggered:
     *   Warning: Class "Stub\static" does not exist at compile time.
     * Phalcon hit this once they started using `-> <static>` for fluent
     * setters. The presence of these methods in the compiled stub is the
     * regression assertion — if the fix were missing, generate would emit
     * the false-positive `nonexistent-class` warning.
     */
    public function chainedStatic() -> bool
    {
        return this->makeStatic()->checkPrivate();
    }

    public function chainedSelf() -> bool
    {
        return this->makeSelf()->checkPrivate();
    }

    public function checkPrivate() -> bool
    {
        return true;
    }
}
