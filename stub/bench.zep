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
     * For-over-Iterator with `continue`, the exact path fixed in issue #2546.
     * The first-iteration guard (zend_bool flag) introduced by the fix adds one
     * extra conditional per iteration; this subject quantifies that overhead
     * against the plain-iterator path and against pure-PHP foreach+continue.
     * Skips every even element (5 of 10 trigger continue).
     */
    public function forOverIteratorWithContinue() -> long
    {
        var v, iter;
        long sum = 0, iv = 0;
        let iter = new \ArrayIterator([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
        for v in iter {
            let iv = (long) v;
            if iv % 2 == 0 {
                continue;
            }
            let sum += iv;
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

    /**
     * Variadic callee (issue #2025). Sums every trailing argument, exercising
     * the zephir_get_args_from() collection emitted for `...` parameters.
     */
    public function variadicSum(...numbers) -> long
    {
        var v;
        long sum = 0;
        for v in numbers {
            let sum += (long) v;
        }
        return sum;
    }

    /**
     * Hot loop calling the variadic method with a 5-argument tail. Measures
     * the per-call cost of collecting the trailing arguments into an array.
     */
    public function variadicSumNarrowLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += this->variadicSum(1, 2, 3, 4, 5);
            let i++;
        }
        return sum;
    }

    /**
     * Same as variadicSumNarrowLoop but with a 15-argument tail, so the report
     * shows how the collection cost scales with the number of variadic args.
     */
    public function variadicSumWideLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += this->variadicSum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
            let i++;
        }
        return sum;
    }

    /**
     * The pre-variadic idiom that `...` replaces: a fixed-arity method reading
     * its arguments through func_get_args(). Paired with variadicSum so the
     * report contrasts the `...` operator against the old workaround.
     */
    public function funcGetArgsSum(a, b, c, d, e) -> long
    {
        var args, v;
        long sum = 0;
        let args = func_get_args();
        for v in args {
            let sum += (long) v;
        }
        return sum;
    }

    public function funcGetArgsSumLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += this->funcGetArgsSum(1, 2, 3, 4, 5);
            let i++;
        }
        return sum;
    }

    /**
     * Call-dispatch micro-workloads for the FastCall investigation (#1510).
     * Each loops a trivial callee so the measured cost is dominated by the
     * call machinery (cache lookup + fcic setup + zend_call_function), not the
     * callee body. Three dispatch flavours exercise distinct kernel paths.
     */
    final public function finalReturnConst() -> long
    {
        return 7;
    }

    public static function staticReturnConst() -> long
    {
        return 7;
    }

    public function finalDispatchLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += this->finalReturnConst();
            let i++;
        }
        return sum;
    }

    public function staticDispatchLoop(long n) -> long
    {
        long i = 0, sum = 0;
        while i < n {
            let sum += self::staticReturnConst();
            let i++;
        }
        return sum;
    }

    /**
     * Generator (yield) workloads — issue #1849. Each producer is paired
     * with an array producer of identical shape so the benchmarks compare
     * lazy suspension against eager materialization, plus an Iterator relay.
     */
    public function generatorRange(long n)
    {
        long i = 1;
        while i <= n {
            yield i;
            let i = i + 1;
        }
    }

    public function arrayRange(long n) -> array
    {
        array result = [];
        long i = 1;
        while i <= n {
            let result[] = i;
            let i = i + 1;
        }
        return result;
    }

    /**
     * Generator relaying an array: exercises the suspension-safe
     * HashPosition/snapshot for-in codegen (one suspension per element).
     */
    public function generatorOverArray(array items)
    {
        var v;
        for v in items {
            yield v;
        }
    }

    /**
     * Produce and consume entirely inside the extension: the C-to-C cost of
     * one suspension+resume per element, no PHP userland in the loop.
     */
    public function sumViaGenerator(long n) -> long
    {
        var v;
        long total = 0;
        for v in this->generatorRange(n) {
            let total += (long) v;
        }
        return total;
    }

    public function sumViaArray(long n) -> long
    {
        var v;
        long total = 0;
        for v in this->arrayRange(n) {
            let total += (long) v;
        }
        return total;
    }

    /**
     * `for i in 0..n` over the `..` range operator (#2433). Compiles to an
     * integer counting loop; previously it materialised an n-element array.
     * Paired with sumRangeFn (explicit range() call) and the pure-PHP baseline
     * so the report shows the operator now matches range() and pure PHP.
     */
    public function sumRangeOperator(long n) -> long
    {
        long i = 0, total = 0;
        for i in 0..n {
            let total += i;
        }
        return total;
    }

    public function sumRangeFn(long n) -> long
    {
        long i = 0, total = 0;
        for i in range(0, n) {
            let total += i;
        }
        return total;
    }
}
