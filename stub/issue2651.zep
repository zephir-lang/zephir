namespace Stub;

/**
 * A persistent array default (typed property or class constant) must behave like
 * a PHP one: shared until written, copied on first write. Before #2651 the
 * persistent table was refcount 1, so copy-on-write never separated it and a
 * userland write landed in the single table every instance points at.
 *
 * The string keys and string values are deliberate: they are what makes
 * zend_array_dup()'s immutable branch (a raw bucket memcpy) observable.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2651
 */
class Issue2651
{
    const MAP = ["a": "first", "b": "second"];

    protected array headers = [];

    protected array preset = ["x": "one", "y": "two"];

    protected array nested = ["outer": ["inner": "deep"]];

    public function toArray() -> array
    {
        return this->headers;
    }

    public function getPreset() -> array
    {
        return this->preset;
    }

    public function getNested() -> array
    {
        return this->nested;
    }

    /**
     * An internal write must separate too, and must not disturb other instances.
     */
    public function addHeader(string key, string value) -> void
    {
        let this->headers[key] = value;
    }
}
