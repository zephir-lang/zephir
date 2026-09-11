namespace Stub;

/**
 * The keys of a persistent array default belong to the module, not to any copy
 * of it. zephir_persist_constant_zval() (kernel/main.c) marks the shared table
 * HASH_FLAG_STATIC_KEYS, and zend_array_dup()'s immutable branch is a raw
 * bucket memcpy that inherits the flag without addref'ing anything. A copy that
 * then loses the flag releases every key, including the ones it only borrowed.
 *
 * A copy loses the flag on the first insert of a NON-interned key. A PHP
 * literal offset is interned at compile time, so only a key built at runtime
 * reaches the defect. Every test against this fixture must build its keys.
 *
 * The shape is Phalcon\Acl\Adapter\Memory's: a "*" key in the default and a
 * runtime component name written over it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2699
 */
class Issue2699
{
    const MAP = ["*": true, "a": 1];

    protected array components = ["*": true];

    protected array nested = ["outer": ["inner": true]];

    public function add(string key) -> void
    {
        let this->components[key] = true;
    }

    public function getComponents() -> array
    {
        return this->components;
    }

    public function getNested() -> array
    {
        return this->nested;
    }
}
