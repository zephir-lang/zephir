namespace Stub;

/**
 * A dynamic name the class does not declare falls back to `__call()` when the
 * call has a compatible `$this`, and to `__callStatic()` otherwise.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
class Issue2723Magic
{
    public function __call(string name, array arguments) -> string
    {
        return "__call " . name;
    }

    public static function __callStatic(string name, array arguments) -> string
    {
        return "__callStatic " . name;
    }

    public function selfDynamic(var name)
    {
        return self::{name}();
    }

    public static function selfDynamicStatic(var name)
    {
        return self::{name}();
    }
}
