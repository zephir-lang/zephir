namespace Stub;

/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/2546
 *
 * `continue` inside a `for ... in` loop over a PHP Iterator/Traversable was
 * not calling next() on the iterator. The generated while(1) placed next()
 * after the user body, so C `continue` jumped past it, producing an infinite
 * loop on any element that triggered the continue.
 */
class Issue2546
{
    /**
     * Collect items from a Traversable, skipping any equal to `skip`.
     * `continue` must advance the iterator; without the fix it loops forever.
     */
    public function collectExcluding(var items, var skip) -> array
    {
        var item;
        var result = [];

        for item in items {
            if item === skip {
                continue;
            }
            let result[] = item;
        }

        return result;
    }

    /**
     * Collect items until `stop` is encountered; verifies `break` still works.
     */
    public function collectUntil(var items, var stop) -> array
    {
        var item;
        var result = [];

        for item in items {
            if item === stop {
                break;
            }
            let result[] = item;
        }

        return result;
    }

    /**
     * Collect key-value pairs, skipping those whose key equals `skipKey`.
     * Exercises the key+value path with continue.
     */
    public function collectKeysExcluding(var items, var skipKey) -> array
    {
        var k, v;
        var result = [];

        for k, v in items {
            if k === skipKey {
                continue;
            }
            let result[k] = v;
        }

        return result;
    }
}
