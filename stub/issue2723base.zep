namespace Stub;

/**
 * The parent class `parent::{name}()` and `Issue2723Base::{name}()` resolve
 * against in Stub\Issue2723.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
class Issue2723Base
{
    public static function baseTarget() -> string
    {
        return "base";
    }

    public static function baseWithArgument(var argument) -> string
    {
        return "base argument " . argument;
    }

    public static function baseCalledClass() -> string
    {
        return get_called_class();
    }

    public function baseInstance() -> string
    {
        return "base instance of " . get_class(this);
    }
}
