namespace Stub;

/**
 * Method bodies whose generated C used to open a Zephir memory frame and never
 * close it, together with the probes that measure what each call retains.
 *
 * Converting a parameter opens the frame; only the end of the body closes it.
 * Every shape below either has no body to end or was believed to have no
 * reachable end, so `ZEPHIR_MM_RESTORE()` was never emitted and the frame, the
 * method globals and the converted parameter stayed allocated for each call.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2716
 */
class Issue2716
{
    protected retained = [];

    /**
     * The reported shape: no body, and an `array` parameter with a default. The
     * default costs a further array_init() on top of the frame itself.
     */
    public function __construct(array options = [])
    {
    }

    public function emptyWithString(string name) -> void
    {
    }

    public function emptyWithArray(array items) -> void
    {
    }

    /**
     * A comment is not a statement, so this body ends nowhere as far as the
     * compiler is concerned, exactly like an empty one.
     */
    public function commentOnlyBody(string name = "x") -> void
    {
        // nothing to do
    }

    /**
     * A `try` with no `catch` clause clears the exception, so the `throw` does
     * not leave the method: control continues past the statement and runs off
     * the end of the body.
     */
    public function swallowedThrow(array items) -> void
    {
        try {
            throw new Exception("swallowed");
        }
    }

    public function constructProbe(int iterations) -> int
    {
        int i;
        var before, after, subject;

        let subject = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2716();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function stringProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->emptyWithString("name");
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function arrayProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->emptyWithArray([]);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function commentProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->commentOnlyBody();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function throwProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->swallowedThrow([]);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The positive control for every probe above: it retains what it produces,
     * so it must report growth. A flat probe reading means nothing unless the
     * measurement is known to work.
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
