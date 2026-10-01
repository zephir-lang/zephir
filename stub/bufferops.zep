namespace Stub;

/**
 * Drives <Ns>\Buffer from Zephir source, so the kernel fast paths in
 * kernel/array.c are exercised by compiled code rather than from PHP.
 *
 * The buffer parameters are untyped `var` on purpose: Buffer is a hand-written
 * kernel class, so the compiler has no definition to check a `<Buffer>` hint
 * against.
 */
class BufferOps
{
    public function readAt(var buf, int index)
    {
        return buf[index];
    }

    public function writeAt(var buf, int index, var value) -> void
    {
        let buf[index] = value;
    }

    public function has(var buf, int index) -> bool
    {
        return isset buf[index];
    }

    public function size(var buf) -> int
    {
        return count(buf);
    }

    public function sum(var buf) -> double
    {
        double total = 0.0;
        int i = 0, n = 0;

        let n = count(buf);

        while i < n {
            let total += (double) buf[i];
            let i++;
        }

        return total;
    }

    public function scale(var buf, double factor) -> void
    {
        int i = 0, n = 0;

        let n = count(buf);

        while i < n {
            let buf[i] = ((double) buf[i]) * factor;
            let i++;
        }
    }
}
