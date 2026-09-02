namespace Stub;

/**
 * A local whose only assignment sits inside a conditional keeps the ZVAL_UNDEF
 * it receives at declaration whenever that branch does not run, and that zval
 * reaches userland: var_dump() renders it as UNKNOWN:0, which is not a PHP
 * value. PHP evaluates an unset variable as null.
 *
 * This is the conditional half of #2654, which covered a local that is never
 * assigned at all.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
class Issue2679
{
    protected retained = [];

    /**
     * The first shape from the issue: one assignment, inside an if.
     */
    public function conditional(bool flag)
    {
        var x;

        if flag {
            let x = "set";
        }

        return x;
    }

    /**
     * Guard: `fetch` looks conditional but is not. Every fetch helper writes
     * its target on both paths, the value on a hit and ZVAL_NULL on a miss
     * (kernel/array.c zephir_array_isset_*_fetch, kernel/object.c
     * zephir_fetch_property*), so the target is always a valid zval.
     *
     * It also must not be touched: a read-only fetch hands back a borrowed
     * value with no reference taken, so registering the slot with the memory
     * frame would free a reference this method never owned.
     */
    public function fetchCase(array data) -> array
    {
        var v;
        bool found = false;

        if fetch v, data["k"] {
            let found = true;
        }

        return [found, v];
    }

    /**
     * Guard: the fetch target is read only where the fetch succeeded, so it is
     * definitely assigned at every read and needs no initialization.
     */
    public function fetchGuarded(array data) -> string
    {
        var v;

        if fetch v, data["k"] {
            return (string) v;
        }

        return "missing";
    }

    /**
     * Guard: same as above, as a standalone statement.
     */
    public function fetchStatement(array data)
    {
        var v;

        fetch v, data["k"];

        return v;
    }

    /**
     * Guard: both arms assign, so the variable is definitely assigned after
     * the if and nothing has to be added.
     */
    public function exhaustiveIfElse(bool flag)
    {
        var x;

        if flag {
            let x = "yes";
        } else {
            let x = "no";
        }

        return x;
    }

    /**
     * Two conditional writes. The compiler's own `conditional-initialization`
     * warning misses this one because it only fires on a single init branch.
     */
    public function multiBranch(bool a, bool b)
    {
        var x;

        if a {
            let x = "a";
        }

        if b {
            let x = "b";
        }

        return x;
    }

    /**
     * Guard: the conditional write is followed by an unconditional one.
     */
    public function conditionalThenReassign(bool flag)
    {
        var x;

        if flag {
            let x = "first";
        }

        let x = "second";

        return x;
    }

    /**
     * A loop body may run zero times.
     */
    public function loopOnly(array items)
    {
        var x, item;

        for item in items {
            let x = item;
        }

        return x;
    }

    public function whileOnly(int n)
    {
        var x;
        int i = 0;

        while i < n {
            let x = "iter" . i;
            let i++;
        }

        return x;
    }

    /**
     * Guard: a `default` clause makes the switch exhaustive.
     */
    public function switchWithDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;

            default:
                let x = "other";
                break;
        }

        return x;
    }

    public function switchWithoutDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;

            case 2:
                let x = "two";
                break;
        }

        return x;
    }

    /**
     * An exception can leave the try block before the assignment runs.
     */
    public function tryOnly(bool boom)
    {
        var x;

        try {
            if boom {
                throw new \RuntimeException("boom");
            }

            let x = "ok";
        } catch \RuntimeException {
            let boom = true;
        }

        return x;
    }

    /**
     * The read happens before the only assignment.
     */
    public function readBeforeWrite() -> string
    {
        var x, t;

        let t = typeof x;
        let x = "set";

        return t . "/" . typeof x;
    }

    public function stringLocal(bool flag)
    {
        string s;

        if flag {
            let s = "set";
        }

        return s;
    }

    public function arrayLocal(bool flag)
    {
        array a;

        if flag {
            let a = [1, 2, 3];
        }

        return a;
    }

    /**
     * The conditionally assigned local is handed to a function rather than
     * returned, so the null has to be real and not only visible to var_dump().
     */
    public function passAsArgument(bool flag) -> string
    {
        var x;

        if flag {
            let x = "set";
        }

        return gettype(x);
    }

    /**
     * A generator step is a resumable function whose locals are saved and
     * restored across yield points, so Method::compile() rewrites its
     * first-init macros to their N forms. The registration added at the
     * declaration has to survive that rewrite intact.
     */
    public function conditionalInGenerator(bool flag)
    {
        var x;

        if flag {
            let x = "set";
        }

        yield x;
        yield "after";
    }

    /**
     * A capture reads the local where the closure is created, so an unassigned
     * one has to arrive in the closure as null.
     */
    public function conditionalCapturedByClosure(bool flag) -> string
    {
        var x, fnc;

        if flag {
            let x = "set";
        }

        let fnc = function() use (x) {
            return gettype(x);
        };

        return call_user_func(fnc);
    }

    /**
     * Allocates a heap string inside the conditional branch and drops it on
     * return, so a slot that stopped being registered with the memory frame
     * shows up as growth in leakProbe().
     */
    public function conditionalHeap(bool flag) -> string
    {
        var x;

        if flag {
            let x = str_repeat("x", 256);
        }

        return typeof x;
    }

    /**
     * Exercises every conditionally assigned local `iterations` times and
     * reports how much memory the run retained. Nothing is allocated inside
     * the measured window, so a non-zero result is a leak.
     */
    public function leakProbe(int iterations) -> int
    {
        int i;
        var before, after, sink;

        let sink = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let sink = this->conditional(true);
            let sink = this->conditional(false);
            let sink = this->conditionalHeap(true);
            let sink = this->conditionalHeap(false);
            let sink = this->multiBranch(true, true);
            let sink = this->loopOnly([1, 2]);
            let sink = this->whileOnly(2);
            let sink = this->switchWithoutDefault(1);
            let sink = this->tryOnly(false);
            let sink = this->stringLocal(true);
            let sink = this->arrayLocal(true);
            let sink = this->passAsArgument(true);
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
            let this->retained[] = this->conditionalHeap(true);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }
}
