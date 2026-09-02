
/**
 * kernel/array.c's ArrayAccess branches got three lifetimes wrong.
 *
 * zephir_array_isset_string_fetch() released the zend_string it built for the
 * offset between the offsetExists() and the offsetGet() call, so offsetGet()
 * was handed a freed string. The failure is not a crash: the very next
 * allocation is ZVAL_STRINGL(&method, "offsetget", 9) inside
 * zephir_call_class_method_aparams(), which lands in the same ZEND_MM bin and
 * pops the slot the offset just vacated, so the object silently receives the
 * key "offsetget".
 *
 * All six isset helpers additionally dropped the offsetExists() return value
 * on the floor, and none of them held a reference to the container across the
 * two userland calls. PHP's own zend_std_read_dimension() owns the offset, the
 * container and the offsetExists() result; these helpers now do the same.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */

namespace Stub;

class Issue2656
{
    protected container;
    protected retained = [];

    public function setContainer(var container) -> void
    {
        let this->container = container;
    }

    /**
     * The reported shape. `let value = null` is a second mutation on purpose:
     * it keeps the fetch off the read-only path, so this method measures the
     * offset lifetime and nothing else.
     *
     * The key is nine characters on purpose. _ZSTR_STRUCT_SIZE(9) is 34, the
     * same 40-byte bin as "offsetget", which is what makes the corruption
     * deterministic on every supported PHP version. A shorter key only
     * reproduces on 8.4+ and would pass silently on 8.0 to 8.2.
     */
    public function fetchLiteral(var container)
    {
        var value;

        let value = null;
        if fetch value, container["cache.key"] {
            return value;
        }

        return false;
    }

    public function fetchLiteralMissing(var container)
    {
        var value;

        let value = null;
        if fetch value, container["absent.k1"] {
            return value;
        }

        return false;
    }

    /**
     * empty() on an array-access reaches the same helper through
     * zephir_isempty_dim_string().
     */
    public function emptyLiteral(var container) -> bool
    {
        return empty container["cache.key"];
    }

    public function issetLiteral(var container) -> bool
    {
        return isset container["cache.key"];
    }

    public function fetchLong(var container)
    {
        var value;

        let value = null;
        if fetch value, container[7] {
            return value;
        }

        return false;
    }

    public function issetLong(var container) -> bool
    {
        return isset container[7];
    }

    public function fetchDynamic(var container, var key)
    {
        var value;

        let value = null;
        if fetch value, container[key] {
            return value;
        }

        return false;
    }

    public function issetDynamic(var container, var key) -> bool
    {
        return isset container[key];
    }

    /**
     * The container is read from a property, which the emitter hands over
     * borrowed. A userland offsetExists() that clears that property drops the
     * last reference, so without an owned reference across both calls the
     * helper reads a freed object.
     */
    public function fetchThroughProperty()
    {
        var value;

        let value = null;
        if fetch value, this->container["cache.key"] {
            return value;
        }

        return false;
    }

    /**
     * Every isset helper, `iterations` times. The recorder's offsetExists()
     * returns a fresh heap string, so an unreleased offsetExists() result
     * shows up as growth. Nothing in the loop retains anything.
     */
    public function leakProbe(var container, var key, int iterations) -> int
    {
        int i;
        var before, after, sink;

        let sink = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let sink = this->fetchLiteral(container);
            let sink = this->fetchLiteralMissing(container);
            let sink = this->fetchLong(container);
            let sink = this->fetchDynamic(container, key);
            let sink = this->issetLiteral(container);
            let sink = this->issetLong(container);
            let sink = this->issetDynamic(container, key);
            let sink = this->emptyLiteral(container);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The positive control for every probe in this class: it retains what it
     * allocates, so it must report growth. A control that replaces what it
     * retains reads zero and proves nothing.
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
     * `unset obj->literal` routes through kernel/object.c
     * zephir_unset_property(), which built a zend_string for the name and
     * never released it, leaking on every call.
     */
    public function unsetPropertyProbe(int iterations) -> int
    {
        int i;
        var before, after, obj;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let obj = new \stdClass();
            let obj->probe = 1;
            unset(obj->probe);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * `throw` of a non-object routes through kernel/exception.c
     * zephir_throw_exception_debug(), which wraps the value in an Exception
     * and addrefs it for zend_throw_exception_object(). Exception::__construct
     * rejects an array, so the throw is skipped and both the instance and the
     * extra reference used to leak.
     */
    public function throwArrayProbe(int iterations) -> int
    {
        int i;
        var before, after, bad;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let bad = [1, 2];
            try {
                throw bad;
            } catch \Throwable {
            }
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }
}
