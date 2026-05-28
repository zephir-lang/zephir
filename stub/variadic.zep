namespace Stub;

/**
 * Free (non-method) variadic function — the exact form from issue #2025:
 * `function f(...params)`. Exercises the PHP_FUNCTION code path.
 */
function variadicSum(...numbers) -> int
{
    int total = 0;
    var n;

    for n in numbers {
        let total += n;
    }

    return total;
}

class Variadic
{
    /**
     * Variadic only, untyped: collects every argument into an array.
     */
    public function collect(...args) -> array
    {
        return args;
    }

    /**
     * Fixed required parameter followed by a variadic one.
     */
    public function sum(int first, ...rest) -> int
    {
        int total = first;
        var n;

        for n in rest {
            let total += n;
        }

        return total;
    }

    /**
     * Counts only the variadic (trailing) arguments.
     */
    public function countExtra(var required, ...rest) -> int
    {
        return count(rest);
    }

    /**
     * Typed variadic parameter.
     */
    public function joinStrings(string glue, string ...parts) -> string
    {
        return implode(glue, parts);
    }

    /**
     * Required + optional fixed parameter followed by a variadic one
     * (the example from issue #2025).
     */
    public function describe(int req, int opt = 0, ...params) -> array
    {
        return [req, opt, count(params)];
    }

    /**
     * Static variadic method — exercises the static-context code path.
     */
    public static function staticCollect(...args) -> array
    {
        return args;
    }

    /**
     * Array-typed variadic: each argument is an array; returns the total
     * number of elements across all of them.
     */
    public function totalLength(array ...rows) -> int
    {
        int total = 0;
        var row;

        for row in rows {
            let total += count(row);
        }

        return total;
    }

    /**
     * Collects objects and returns their class names. Exercises a variadic
     * that carries object values.
     */
    public function classNames(...objects) -> array
    {
        array names = [];
        var obj;

        for obj in objects {
            let names[] = get_class(obj);
        }

        return names;
    }
}
