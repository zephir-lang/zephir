
/**
 * A read-only subscript read treats the value as borrowed from the container.
 *
 * That holds for a native array: the kernel hands back a ZVAL_COPY_VALUE() and
 * the hash table keeps the value alive. An ArrayAccess object synthesises the
 * value inside offsetGet() and owns nothing afterwards, so the Z_TRY_DELREF_P()
 * the read-only branch used to apply dropped it to refcount 0 with no owner.
 *
 * The reads under test are shaped so the emitter used to promote the target to
 * read-only: an untyped container parameter, a call on an earlier line than the
 * read (the throw is there for that alone), and a target assigned exactly once.
 * On the fix they emit `, 0)` and plain `PH_NOISY`; before it they emitted
 *
 *     zephir_array_isset_string_fetch(&value, container, SL("namespace"), 1)
 *     zephir_array_fetch_string(&value, container, SL("namespace"), PH_NOISY | PH_READONLY, ...)
 *
 * so if a change to those heuristics stops the promotion firing on the buggy
 * kernel, these methods stop testing anything. nativeArrayStaysReadOnly() is
 * the other side: its container is provably an array, so it must keep the
 * shortcut, and Zephir\Test\CodeGen\ReadOnlyArrayAccessTest pins that in C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */

namespace Stub;

class Issue2682
{
    protected retained = [];

    /**
     * `fetch` family, kernel/array.c zephir_array_isset_string_fetch().
     *
     * The container's offsetGet() returns an object whose destructor reports
     * back through the container, so the test can tell "released too early"
     * from "released once, on time" without reading the freed slot.
     *
     * Released early, the copy holds the only reference and dropping it runs
     * the destructor before this method returns true. Owned correctly, the
     * refcount runs 1 -> 2 -> 1 and the destructor waits for the memory frame.
     */
    public function fetchReleasedEarly(container) -> bool
    {
        var value, copy;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        if fetch value, container["namespace"] {
            let copy = value;
            let copy = null;

            return container->destroyed;
        }

        return true;
    }

    /**
     * The same read with nothing copied out of it: with the value released and
     * no owner left, the allocation is simply lost.
     */
    public function fetchOnly(container) -> bool
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        if fetch value, container["namespace"] {
            return true;
        }

        return false;
    }

    /**
     * Array-access family, kernel/array.c zephir_array_fetch_string().
     */
    public function readReleasedEarly(container) -> bool
    {
        var value, copy;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let value = container["namespace"];
        let copy = value;
        let copy = null;

        return container->destroyed;
    }

    public function plainRead(container) -> bool
    {
        var value;

        if typeof container === "null" {
            throw new \RuntimeException("A container is required");
        }

        let value = container["namespace"];

        return typeof value !== "null";
    }

    /**
     * Both families, `iterations` times. The container's offsetGet() returns a
     * fresh 64 byte string, so an unreleased result shows up as growth.
     * Nothing in the loop retains anything.
     */
    public function leakProbe(var container, int iterations) -> int
    {
        int i;
        var before, after, sink;

        let sink = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let sink = this->fetchOnly(container);
            let sink = this->plainRead(container);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The positive control for the probe above: it retains what it allocates,
     * so it must report growth. Without it a flat reading would also be what a
     * probe that cannot measure at all reports.
     */
    public function leakProbeControl(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let this->retained[] = str_repeat("x", 64);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The read-only shortcut has to survive for a container that provably holds
     * a native array, which is where it pays for itself. Assigned from a literal
     * and never aliased, so the generated C must keep PH_READONLY here.
     */
    public function nativeArrayStaysReadOnly(int mode) -> string
    {
        var table, value;

        if mode < 0 {
            throw new \RuntimeException("Not reachable, it only moves the last call line");
        }

        let table = ["namespace": "kept"];
        let value = table["namespace"];

        return value;
    }
}
