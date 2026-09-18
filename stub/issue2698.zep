namespace Stub;

/**
 * Separating an array property must hand the separated array back to exactly
 * one owner.
 *
 * kernel/object.c's separation idiom is
 *
 *     ZVAL_DUP(&new_zv, &tmp);          // a fresh array at refcount 1
 *     ZVAL_COPY_VALUE(&tmp, &new_zv);   // tmp now holds that one reference
 *
 * and the zval_ptr_dtor(&tmp) at the end of each function is what releases it.
 * Three of the six sites dropped the reference with Z_TRY_DELREF() first, so
 * that dtor decremented zero instead of freeing, and the separated array plus
 * everything in it was retained for the life of the request.
 *
 * Every property here is TYPED on purpose. A typed default is persistent and
 * non-refcounted (issues #2608 and #2651), which is the only shape that reaches
 * the broken branch. An untyped `protected items = []` is declared null and
 * filled by the per-instance initializer, so it is already refcount 1 and
 * updates in place.
 *
 * The probes build a fresh instance per iteration because the defect fires only
 * while the property still holds its declared default; after the first write it
 * holds a refcount 1 array and later writes never separate.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2698
 */
class Issue2698
{
    /** let this->items[key] = value -> zephir_update_property_array() */
    protected array items = ["seed": 1];

    /** let this->bag[] = value -> zephir_update_property_array_append() */
    protected array bag = ["seed"];

    /** let this->matrix[a][b] = value -> zephir_update_property_array_multi() */
    protected array matrix = ["row": ["col": 1]];

    /** unset this->removable[key] -> zephir_unset_property_array() */
    protected array removable = ["a": 1, "b": 2];

    /** Untyped, so it is refcounted from the start: the control's sink. */
    protected retained = [];

    public function writeOffset(string key, var value) -> void
    {
        let this->items[key] = value;
    }

    public function appendValue(var value) -> void
    {
        let this->bag[] = value;
    }

    public function writeNested(string outer, string inner, var value) -> void
    {
        let this->matrix[outer][inner] = value;
    }

    public function removeOffset(string key) -> void
    {
        unset this->removable[key];
    }

    public function getItems() -> array
    {
        return this->items;
    }

    public function getBag() -> array
    {
        return this->bag;
    }

    public function getMatrix() -> array
    {
        return this->matrix;
    }

    public function getRemovable() -> array
    {
        return this->removable;
    }

    /**
     * zephir_update_property_array(). Nothing in the loop retains anything, so
     * a reading that scales with `iterations` is the separated array leaking.
     */
    public function writeProbe(int iterations) -> int
    {
        int i;
        var before, after, subject;

        let subject = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2698();
            subject->writeOffset("added", 1);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * zephir_update_property_array_append(). This site was already correct: it
     * puts the reference back with Z_ADDREF(). The probe pins that, so a later
     * cleanup cannot break it.
     */
    public function appendProbe(int iterations) -> int
    {
        int i;
        var before, after, subject;

        let subject = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2698();
            subject->appendValue(1);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * zephir_update_property_array_multi(). Same six lines as writeProbe()'s
     * site, and no fixture reached it before this one.
     */
    public function nestedProbe(int iterations) -> int
    {
        int i;
        var before, after, subject;

        let subject = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2698();
            subject->writeNested("row", "added", 1);
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * zephir_unset_property_array(). This one never released the separated
     * array at all: it has no zval_ptr_dtor() of its own.
     */
    public function unsetProbe(int iterations) -> int
    {
        int i;
        var before, after, subject;

        let subject = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2698();
            subject->removeOffset("a");
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * zephir_unset_property_array() again, but through its REFCOUNTED branch:
     * the first unset leaves the property holding a refcount 1 array, handing
     * it to a local raises that to 2, and the second unset separates again.
     * That branch never put the dropped reference back with Z_ADDREF() the way
     * zephir_update_property_array()'s does, so it leaked as well.
     */
    public function unsetSharedProbe(int iterations) -> int
    {
        int i;
        var before, after, subject, held;

        let subject = null;
        let held = null;
        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let subject = new Issue2698();
            subject->removeOffset("a");
            let held = subject->getRemovable();
            subject->removeOffset("b");
            let held = null;
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The positive control for every probe above: it retains what it allocates,
     * so it must report growth. A control that replaces what it retains reads
     * zero and proves nothing.
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
}
