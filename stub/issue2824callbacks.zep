namespace Stub;

/**
 * A callback is resolved where it is written, so a private method of the
 * class is callable from any of its methods.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824Callbacks
{
    private function secret(var a, var b = "") -> string
    {
        return "secret:" . a . b;
    }

    internal function viaInternal() -> string
    {
        return call_user_func_array([this, "secret"], ["x"]);
    }

    internal function viaInternalFunc() -> string
    {
        return call_user_func([this, "secret"], "y");
    }

    public function fromInternal() -> string
    {
        return this->viaInternal() . "|" . this->viaInternalFunc();
    }

    public function fromMethod() -> string
    {
        return call_user_func([this, "secret"], "z", "!");
    }

    public function fromMethodArray() -> string
    {
        return call_user_func_array([this, "secret"], ["w", "?"]);
    }

    public function fromMethodNamed() -> string
    {
        return call_user_func_array([this, "secret"], ["b": "2", "a": "1"]);
    }

    public function fromGenerator()
    {
        yield call_user_func([this, "secret"], "g");
    }

    public function notCallable(var f)
    {
        return call_user_func(f);
    }

    public function notCallableWithArgs(var f)
    {
        return call_user_func(f, 1);
    }

    public function notCallableArray(var f)
    {
        return call_user_func_array(f, []);
    }

    public function notArray(var f, var args)
    {
        return call_user_func_array(f, args);
    }
}
