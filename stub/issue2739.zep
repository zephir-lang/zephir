namespace Stub;

/**
 * `time()` must read the clock PHP `time()` reads (#2739). C `time(NULL)`
 * lags `gettimeofday()` for a few milliseconds after each second boundary.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2739
 */
class Issue2739
{
    public function now() -> int
    {
        return time();
    }

    public function notBefore(int timestamp) -> bool
    {
        return timestamp <= time();
    }
}
