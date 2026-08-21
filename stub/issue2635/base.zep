namespace Stub\Issue2635;

/**
 * `go()` is inherited by Outer from Inner, `extra()` is declared by Outer
 * itself. Both have to resolve from an abstract class implementing Outer.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2635
 */
abstract class Base implements Outer
{
    public function run() -> string
    {
        return this->go() . "/" . this->extra();
    }
}
