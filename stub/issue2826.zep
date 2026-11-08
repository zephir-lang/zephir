namespace Stub;

/**
 * `fetch` and `empty` read an element, so they must leave the array they read
 * exactly as it was, as PHP's own reads do.
 *
 * An empty array that is not reference counted, such as `[]` inside a PHP
 * constant array or inside a Zephir array constant, used to be replaced in the
 * source by a new array. That write lands in OPcache shared memory or in a
 * table that outlives the request.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2826
 */
class Issue2826
{
    const PARAMS = ["key": [], 0: []];

    public function fetchDynamic(var params, var key) -> bool
    {
        var value;

        return fetch value, params[key];
    }

    public function fetchLong(var params) -> bool
    {
        var value;

        return fetch value, params[0];
    }

    public function fetchString(var params) -> bool
    {
        var value;

        return fetch value, params["key"];
    }

    public function emptyDynamic(var params, var key) -> bool
    {
        return empty params[key];
    }

    public function emptyLong(var params) -> bool
    {
        return empty params[0];
    }

    public function emptyString(var params) -> bool
    {
        return empty params["key"];
    }

    public function assignFetched(var params)
    {
        var value;

        if fetch value, params["key"] {
            let value["y"] = 1;
        }

        return value;
    }

    public function appendFetched(var params)
    {
        var value;

        if fetch value, params["key"] {
            let value[] = 1;
        }

        return value;
    }

    public function unsetFetched(var params)
    {
        var value;

        if fetch value, params["key"] {
            unset value["y"];
        }

        return value;
    }

    public function assignNestedFetched(var params)
    {
        var value;

        if fetch value, params["key"] {
            let value["a"]["b"] = 1;
        }

        return value;
    }
}
