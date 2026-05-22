namespace Stub;

/**
 * Workloads for the PHPBench suite under tests/Benchmark/.
 *
 * Each method runs a tight internal loop so PHPBench's per-iteration cost
 * (revs/iterations setup) stays small relative to the work being measured.
 * Pairs one-to-one with a pure-PHP equivalent in the benchmark class so the
 * reported metric is the Zephir-over-PHP speedup margin.
 */
class Bench
{
    public propA = 0;
    public propB = "hello";

    public function __construct()
    {
        let this->propA = 42;
        let this->propB = "hello";
    }

    /**
     * For-over-array baseline. The compiler skips the IS_ARRAY branch wrapper
     * since `arr` is known to be array (see issue #1878).
     */
    public function forOverArray(array! arr) -> long
    {
        var v;
        long sum = 0;
        for v in arr {
            let sum += (long) v;
        }
        return sum;
    }

    /**
     * For-over-Iterator. The compiler skips the IS_ARRAY branch wrapper
     * since the iterand is known to be an object (issue #1878).
     */
    public function forOverIterator() -> long
    {
        var v, iter;
        long sum = 0;
        let iter = new \ArrayIterator([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
        for v in iter {
            let sum += (long) v;
        }
        return sum;
    }

    /**
     * Object property read in a hot loop. Exercises zephir_read_property.
     */
    public function propertyReadLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += (long) this->propA;
            let i++;
        }
        return sum;
    }

    /**
     * Array fetch by string key in a hot loop. Exercises
     * zephir_array_fetch_string against precomputed-length char* keys.
     */
    public function arrayFetchStringLoop(array! arr, long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += (long) arr["a"] + (long) arr["b"];
            let i++;
        }
        return sum;
    }

    /**
     * Method dispatch in a hot loop. Exercises zephir_call_method on `this`.
     */
    public function methodDispatchLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += this->returnConst();
            let i++;
        }
        return sum;
    }

    public function returnConst() -> long
    {
        return 7;
    }

    /**
     * String concatenation in a hot loop. Exercises ZEPHIR_CONCAT_*.
     */
    public function stringConcatLoop(string! base, long n) -> string
    {
        var out;
        long i = 0;
        let out = "";
        while i < n {
            let out = out . base;
            let i++;
        }
        return out;
    }

    /**
     * Object instantiation + immediate method call.
     */
    public function newInstanceLoop(long n) -> long
    {
        var obj;
        long i = 0, sum = 0;
        while i < n {
            let obj = new Bench();
            let sum += obj->returnConst();
            let i++;
        }
        return sum;
    }
}
