namespace Stub;

/**
 * `unset` of an offset reached through anything but a plain `this->prop`.
 *
 * Only `unset this->prop[k]` went through zephir_unset_property_array(), which
 * separates the array and writes it back. A brace property name, a nested
 * offset and a static property all took the generic branch, which fetched the
 * container into a temporary and unset the key there, so the key survived.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2705
 */
class Issue2705
{
    /**
     * Whatever a test wants to unset an offset on.
     */
    public container;

    /**
     * The static counterpart of `container`.
     */
    public static staticContainer;

    /**
     * Typed, so the default is persistent and the first unset really has to
     * separate from it.
     */
    protected array defaults = ["k": 1, "a": ["b": 1, "c": 2]];

    protected static staticDefaults = ["k": 1, "a": ["b": 1, "c": 2]];

    public function unsetBraceLiteral() -> void
    {
        unset this->{"container"}["k"];
    }

    public function unsetBraceName(var name) -> void
    {
        unset this->{name}["k"];
    }

    public function unsetBraceNameAndKey(var name, var key) -> void
    {
        unset this->{name}[key];
    }

    public function unsetBraceNameLongKey(var name) -> void
    {
        unset this->{name}[3];
    }

    public function unsetNested() -> void
    {
        unset this->container["a"]["b"];
    }

    public function unsetNestedByVar(var outer, var inner) -> void
    {
        unset this->container[outer][inner];
    }

    public function unsetDeep() -> void
    {
        unset this->container["a"]["b"]["c"];
    }

    public function unsetBraceLiteralNested() -> void
    {
        unset this->{"container"}["a"]["b"];
    }

    public function unsetBraceNameNested(var name) -> void
    {
        unset this->{name}["a"]["b"];
    }

    /**
     * The assignment must not run when the unset threw.
     */
    public function unsetNestedThenAssign() -> void
    {
        unset this->container["a"]["b"];
        let this->container = "reached";
    }

    public function unsetPropertyOffsetThenAssign() -> void
    {
        unset this->container["k"];
        let this->container = "reached";
    }

    public function unsetLocalOffsetThenAssign(var local) -> void
    {
        unset local["k"];
        let this->container = "reached";
    }

    public function unsetPropertyThenAssign(var target) -> void
    {
        unset target->item;
        let this->container = "reached";
    }

    public function unsetNamedPropertyThenAssign(var target, var name) -> void
    {
        unset target->{name};
        let this->container = "reached";
    }

    public function unsetLocalNested(var container)
    {
        unset container["a"]["b"];

        return container;
    }

    public function unsetLocalDeep(var container)
    {
        unset container["a"]["b"]["c"];

        return container;
    }

    public function unsetStatic() -> void
    {
        unset self::staticContainer["k"];
    }

    public function unsetStaticNested() -> void
    {
        unset self::staticContainer["a"]["b"];
    }

    /**
     * A copy taken before the unsets must keep both keys, and the copy taken
     * after must not alias the property: both only hold if every unset
     * separated rather than wrote through a shared array or a reference.
     */
    public function snapshotDefaults() -> array
    {
        array ret = [];
        var after;

        let ret[] = this->defaults;
        unset this->{"defaults"}["k"];
        unset this->defaults["a"]["b"];
        let after = this->defaults;
        let after["a"]["c"] = 9;
        let ret[] = this->defaults;
        let ret[] = after;

        return ret;
    }

    public function snapshotStaticDefaults() -> array
    {
        array ret = [];

        let ret[] = self::staticDefaults;
        unset self::staticDefaults["k"];
        unset self::staticDefaults["a"]["b"];
        let ret[] = self::staticDefaults;

        return ret;
    }

    public function nestedProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let this->container = ["a": ["b": 1, "c": 2]];
            unset this->{"container"}["a"]["b"];
            unset this->container["a"]["c"];
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function staticProbe(int iterations) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let self::staticContainer = ["k": 1, "a": ["b": 1]];
            unset self::staticContainer["k"];
            unset self::staticContainer["a"]["b"];
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }

    public function braceNameProbe(int iterations, var name) -> int
    {
        int i;
        var before, after;

        let before = memory_get_usage();

        let i = 0;
        while i < iterations {
            let this->container = ["k": 1, "j": 2];
            unset this->{name}["k"];
            let i++;
        }

        let after = memory_get_usage();

        return after - before;
    }
}
