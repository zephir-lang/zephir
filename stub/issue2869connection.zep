namespace Stub;

/**
 * `self::` and `parent::` read the PDO flags a Zephir subclass inherits.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
class Issue2869Connection extends \PDO
{
    public static function unique() -> int
    {
        return self::FETCH_UNIQUE;
    }

    public static function group() -> int
    {
        return parent::FETCH_GROUP;
    }

    public static function assoc() -> int
    {
        return self::FETCH_ASSOC;
    }
}
