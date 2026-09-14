namespace Stub;

/**
 * `unset this->prop[offset]`.
 *
 * Two codegen defects and one kernel gap met here. The literal offset was
 * written into a temp borrowed from a read-only property fetch, so the
 * zend_string it came to own was never released; the property branch fell
 * through into the generic array-access branch, so the unset was emitted
 * twice and an ArrayAccess property received two offsetUnset() calls; and
 * the kernel stayed silent on containers PHP reports an error for.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2702
 */
class Issue2702
{
    /**
     * Typed, so the default is persistent and non-refcounted and the first
     * unset really separates. That keeps #2698 and #2699 under this fixture
     * as well.
     */
    protected array removable = ["a": 1, "b": 2, 3: "three"];

    protected data = ["key_a": "a", "key_b": "b", 3: "three"];

    protected retained = [];

    /**
     * Public so a test can bind a PHP reference to it and prove the unset
     * reaches the referenced array rather than a copy of it.
     */
    public shared = ["k": 1, "j": 2];

    /**
     * Whatever a test wants to unset an offset on: an array, an ArrayAccess
     * object, a plain object, a string, a scalar, null.
     */
    public container;

    public function getData() -> array
    {
        return this->data;
    }

    public function getRemovable() -> array
    {
        return this->removable;
    }

    public function removeLiteral() -> void
    {
        unset this->data["key_a"];
    }

    public function removeLongLiteral() -> void
    {
        unset this->data[3];
    }

    public function removeByVar(var key) -> void
    {
        unset this->data[key];
    }

    /**
     * A native `string` local is held as a zend_string plus a `_zv` zval, so
     * the offset is passed through rather than boxed.
     */
    public function removeByNativeString(string key) -> void
    {
        unset this->data[key];
    }

    /**
     * A native `int` local is not a zval at all. Passing it straight to
     * zephir_unset_property_array() did not compile, which is why no fixture
     * reached this shape before.
     */
    public function removeByNativeInt(int key) -> void
    {
        unset this->data[key];
    }

    public function removeFromTypedProperty(var key) -> void
    {
        unset this->removable[key];
    }

    public function removeFromShared() -> void
    {
        unset this->shared["k"];
    }

    public function unsetContainerLiteral() -> void
    {
        unset this->container["k"];
    }

    public function unsetContainerLongLiteral() -> void
    {
        unset this->container[3];
    }

    public function unsetContainerByVar(var key) -> void
    {
        unset this->container[key];
    }

    /**
     * The same statement against a local rather than a property. This is the
     * shape almost every unset in the wild has: it reaches the kernel helpers
     * directly instead of through zephir_unset_property_array(), so it is
     * where reporting the container errors PHP reports has the widest reach.
     */
    public function unsetLocalLiteral(var container)
    {
        unset container["k"];

        return container;
    }

    public function unsetLocalLongLiteral(var container)
    {
        unset container[3];

        return container;
    }

    public function unsetLocalByVar(var container, var key)
    {
        unset container[key];

        return container;
    }

    /**
     * The snapshot shape of the issue1259 fixtures: the copy taken before the
     * unset must keep the key, which is only true if the unset separated.
     */
    public function snapshotAroundLiteralUnset() -> array
    {
        array ret = [];

        let ret[] = this->removable;
        unset this->removable["a"];
        let ret[] = this->removable;

        return ret;
    }

    public function literalProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->removeLiteral();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function longLiteralProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            this->removeLongLiteral();
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function localArrayProbe(int iterations) -> int
    {
        int i;
        var before, after;
        array local = [];

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            unset local["key_a"];
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    /**
     * The instrument has to be able to report growth, or a flat reading from
     * the probes above means nothing.
     */
    public function growthProbe(int iterations) -> int
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
