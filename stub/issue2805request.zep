namespace Stub;

/**
 * Only scalar defaults, so the engine allocates the static members of this
 * class on their first access in each request.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
class Issue2805Request
{
    public static counter = "";
    public static pad = 0;

    public static function bump() -> string
    {
        let self::counter .= "x";

        return self::counter;
    }
}
