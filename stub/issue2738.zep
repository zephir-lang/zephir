namespace Stub;

/**
 * `Attribute::*` flags whose values depend on the PHP that compiles the
 * extension, not on the one that ran `zephir generate` (#2738). PHP 8.5
 * renumbered `TARGET_ALL` and `IS_REPEATABLE`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2738
 */
#[\Attribute(\Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE)]
class Issue2738
{
    const TARGETS = \Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE;
    const LIST = [\Attribute::TARGET_CLASS, "all": \Attribute::TARGET_ALL];

    public function withDefault(int flags = \Attribute::IS_REPEATABLE) -> int
    {
        return flags;
    }

    public function returned() -> int
    {
        return \Attribute::TARGET_ALL;
    }

    public function combined() -> int
    {
        return \Attribute::TARGET_ALL | \Attribute::IS_REPEATABLE;
    }

    public function described() -> string
    {
        return "flags=" . \Attribute::TARGET_ALL;
    }
}
