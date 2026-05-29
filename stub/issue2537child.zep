namespace Stub;

/**
 * Child class that overrides every `<static>`-returning method declared
 * in `Issue2537`. The override is what makes PHP run the arginfo
 * class-name resolution on the child during `MINIT`. See `Issue2537`
 * for the full failure-mode rationale.
 *
 * Also exercises `-> <parent>` and `-> <self>` on a child that actually
 * has a parent class.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 */
class Issue2537Child extends Issue2537
{
    protected tag = "child";

    public function make() -> <static>
    {
        return this;
    }

    public function makeNullable() -> <static> | null
    {
        return this;
    }

    public function makeViaParent() -> <parent>
    {
        return new Issue2537();
    }

    public function makeViaSelf() -> <self>
    {
        return this;
    }
}
