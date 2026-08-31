namespace Stub;

/**
 * A declared variable that is never assigned keeps the ZVAL_UNDEF it receives
 * at declaration, and that zval reaches userland unchanged: var_dump() renders
 * it as UNKNOWN:0, which is not a PHP value. PHP evaluates an unset variable as
 * null.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2654
 */
class Issue2654
{
    protected retained = [];

    public function readVar()
    {
        var x;

        return x;
    }

    public function readString()
    {
        string s;

        return s;
    }

    public function readArray()
    {
        array a;

        return a;
    }

    public function readInt() -> int
    {
        int i;

        return i;
    }

    /**
     * Any read is affected, not only `return`: an unassigned local handed to a
     * function has to arrive as null too.
     */
    public function passAsArgument() -> string
    {
        var x;

        return gettype(x);
    }

    /**
     * The same value captured by a closure.
     */
    public function captureInClosure()
    {
        var x, capture;

        let capture = function () use (x) {
            return x;
        };

        return call_user_func(capture);
    }

    public function emptyOnUnassigned() -> bool
    {
        var x;

        return empty x;
    }

    /**
     * `-null` is 0 in PHP. Reaching zephir_negate() with an IS_UNDEF zval falls
     * through to convert_scalar_to_number(), which asserts.
     */
    public function negateUnassigned()
    {
        var x;

        return -x;
    }

    public function concatUnassigned() -> string
    {
        var x;

        return "tail" . x;
    }

    /**
     * A generator step saves and restores its locals across every resume.
     */
    public function yieldUnassigned()
    {
        var x;

        yield x;
    }

    /**
     * Assigned locals must keep their existing behaviour: this one is the
     * control for the whole fix.
     */
    public function writeVar()
    {
        var y;

        let y = "assigned";

        return y;
    }

    /**
     * Omitting the argument routes `b` through its `b_sub` companion, whose
     * IS_UNDEF is what makes ZEPHIR_CPY_WRT register it with the memory frame.
     */
    public function optionalParam(var b = null) -> string
    {
        let b = "prefix" . b;

        return b;
    }

    /**
     * Reads an unassigned local `iterations` times and reports how much memory
     * the run retained. Nothing is allocated inside the measured window, so a
     * non-zero result is a leak.
     */
    public function leakProbe(int iterations) -> int
    {
        int i;
        var before, after, sink;

        let sink = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let sink = this->readVar();
            let sink = this->readString();
            let sink = this->readArray();
            let sink = this->passAsArgument();
            let sink = this->captureInClosure();
            let sink = this->concatUnassigned();
            let sink = this->optionalParam();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The positive control for leakProbe(): it retains every value it produces,
     * so it must report growth. A control that replaces what it retains reads
     * zero and proves nothing.
     */
    public function leakProbeControl(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let this->retained[] = "retained-" . i;
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }
}
