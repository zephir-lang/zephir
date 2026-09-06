/**
 * The write context of a by-reference call argument.
 *
 * PHP fetches such an argument through its storage slot: `ZEND_FETCH_OBJ_W`
 * hands the VM an `IS_INDIRECT` to the property itself, `ZEND_FETCH_DIM_W`
 * separates the container before looking the element up (creating it when it is
 * missing), and `ZEND_SEND_REF` turns that slot into a reference. Zephir read a
 * borrowed copy of the property instead, which is a different thing in four
 * observable ways and a use-after-free in a fifth.
 *
 * Each method below is one of them, and Extension\Issue2691Test compares every
 * one against the same code run as plain PHP.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */

namespace Stub;

class Issue2691
{
    protected prefixes = [];

    protected retained = [];

    protected rows = [];

    protected value = null;

    protected static staticPrefixes = [];

    public readonly array readonlyRows;

    public function __construct()
    {
        let this->readonlyRows = ["a": ["seed"]];
    }

    /**
     * The container is a property, and an array returned by an earlier call is
     * still held. PHP separates the property's array on the second call, so the
     * array handed out by the first stops tracking it. Borrowing the property
     * left both pointing at one table, so a later push showed through the copy.
     */
    public function pushAndReturn(string key, string value) -> array
    {
        if !isset this->prefixes[key] {
            let this->prefixes[key] = [];
        }

        array_push(this->prefixes[key], value);

        return this->prefixes;
    }

    /**
     * The property is named at runtime. `ZEND_FETCH_OBJ_W` does not care that
     * the name is a variable rather than a literal, so neither may this: the
     * write still has to reach the property and not a copy of it.
     */
    public function pushIntoDynamic(string name, string key, string value) -> array
    {
        if !isset this->prefixes[key] {
            let this->prefixes[key] = [];
        }

        array_push(this->{name}[key], value);

        return this->prefixes;
    }

    /**
     * The whole dynamically named property as the by-reference argument.
     */
    public function shiftDynamic(string name) -> var
    {
        return array_shift(this->{name});
    }

    /**
     * The name written as a literal in braces, which the compiler resolves the
     * same way it resolves an ordinary property access.
     */
    public function pushIntoBracedLiteral(string key, string value) -> array
    {
        if !isset this->prefixes[key] {
            let this->prefixes[key] = [];
        }

        array_push(this->{"prefixes"}[key], value);

        return this->prefixes;
    }

    /**
     * There is no slot to hand out for a readonly property either, so the fetch
     * falls back to reading in write mode and the engine raises its own
     * `Cannot indirectly modify readonly property`. Letting it speak keeps that
     * message, and the version it arrived in, out of the kernel.
     */
    public function pushIntoReadonly(string key, string value) -> bool
    {
        array_push(this->readonlyRows[key], value);

        return true;
    }

    public function getReadonlyRows() -> array
    {
        return this->readonlyRows;
    }

    /**
     * A read of an element an earlier write context turned into a reference.
     *
     * PHP's read context dereferences, `ZEND_FETCH_DIM_R`'s ZVAL_COPY_DEREF(),
     * so what comes back is the value and the copy is a copy. Handing back the
     * reference instead would make every later write to that copy show through
     * the property.
     */
    public function pushThenCopy(string key, string value) -> array
    {
        var copy, fetched;

        if !isset this->prefixes[key] {
            let this->prefixes[key] = [];
        }

        array_push(this->prefixes[key], value);

        let copy = this->prefixes[key];
        let copy[] = "copy only";

        if !fetch fetched, this->prefixes[key] {
            let fetched = null;
        }

        let fetched[] = "fetched only";

        return ["property": this->prefixes[key], "copy": copy, "fetched": fetched];
    }

    /**
     * The same construct on a static property, `ZEND_FETCH_STATIC_PROP_W`. Its
     * slot lives in the class rather than in an object, and everything else
     * about the write context is the same.
     */
    public static function pushStatic(string key, string value) -> array
    {
        if !isset self::staticPrefixes[key] {
            let self::staticPrefixes[key] = [];
        }

        array_push(self::staticPrefixes[key], value);

        return self::staticPrefixes;
    }

    public static function resetStatic() -> void
    {
        let self::staticPrefixes = [];
    }

    /**
     * The same construct with a local container that someone else also holds.
     * PHP separates it and the write lands; the borrowed fetch could only turn
     * the element into a reference while the table was unshared, so the caller
     * got an owned copy and the write reached nothing.
     */
    public function pushIntoLocalShared(string key, string value) -> array
    {
        var container, alias;

        let container = [];
        let container[key] = [];
        let alias = container;

        array_push(container[key], value);

        return ["container": container, "alias": alias];
    }

    /**
     * The whole property is the by-reference argument, with no subscript. This
     * one never went through the write context at all: the property was read
     * borrowed and ZEPHIR_MAKE_REF() moved the borrow into a reference, so
     * array_shift()'s own separation ran GC_TRY_DELREF() on a reference the
     * zval never took. Wrong values, and a bad free under USE_ZEND_ALLOC=0.
     */
    public function shiftRows() -> var
    {
        return array_shift(this->rows);
    }

    /**
     * A by-reference callee that replaces its argument rather than mutating it
     * in place. settype() destroys the old value first, which on a borrowed
     * property frees the array the property still points at.
     */
    public function retypeValue(string type) -> bool
    {
        return settype(this->value, type);
    }

    /**
     * The element does not exist yet. A write context is a lookup-or-create in
     * PHP, with no diagnostic; the read context Zephir used raises "Undefined
     * index" and leaves the container alone.
     */
    public function pushMissingKey(string key, string value) -> array
    {
        array_push(this->prefixes[key], value);

        return this->prefixes;
    }

    /**
     * The slot is not reachable for a magic property, so the fetch falls back
     * to reading an owned copy, exactly as PHP's `zend_fetch_property_address()`
     * does, and says so with the same notice. The holder is untyped so the
     * compiler cannot settle the property at compile time, which is also the
     * only way to write this: a declared property is required otherwise.
     */
    public function pushIntoOverloaded(var holder, string key, string value) -> bool
    {
        array_push(holder->missing[key], value);

        return true;
    }

    public function setRows(var rows) -> void
    {
        let this->rows = rows;
    }

    public function getRows() -> var
    {
        return this->rows;
    }

    public function setValue(var value) -> void
    {
        let this->value = value;
    }

    public function getValue() -> var
    {
        return this->value;
    }

    public function getPrefixes() -> array
    {
        return this->prefixes;
    }

    /**
     * The write context run `iterations` times over a property whose value is
     * replaced every round, so the size of what is reachable does not change
     * and anything the fetch fails to release shows up as growth.
     * leakProbeControl() below is the positive control: a flat reading is what
     * a probe that cannot measure anything reports too.
     */
    public function writeLeakProbe(int iterations) -> int
    {
        int i;
        var before, after, sink;

        let sink = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let this->rows = ["seed", "next"];
            let sink = this->shiftRows();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

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
}
