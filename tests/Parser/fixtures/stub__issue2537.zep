namespace Stub;

/**
 * Regression coverage for PR #2537 / issue #2505 follow-up: the parent
 * declares a `-> <static>` method, the child (in issue2537child.zep)
 * overrides the same signature. Under the prior codegen the `<static>`
 * return type was emitted as the literal lowercase string `static`
 * inside `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX`. PHP then ran the
 * engine's arginfo class-name resolver on the override, which has no
 * `static` class entry to point at, and aborted MINIT with:
 *
 *     Fatal error: static must be registered before Stub\Issue2537Child
 *
 * The fix emits a type-mask form (`ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX`
 * with `MAY_BE_STATIC`) for `<static>`, which the engine treats as a
 * built-in type and resolves without touching the class table.
 *
 * cphalcon hit this on its fluent-setter methods (`-> <static>`
 * overridden by component subclasses).
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
class Issue2537
{
    protected tag = "parent";

    public function make() -> <static>
    {
        return this;
    }

    public function makeNullable() -> <static> | null
    {
        return this;
    }

    public function getTag() -> string
    {
        return this->tag;
    }
}
