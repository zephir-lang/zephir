namespace Stub\Issue2391;

/**
 * Shortcuts on a static property read and write the static property.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
class StaticAccessors
{
    protected static plain = 5 { get, set };

    /** @var int */
    protected static count = 1 { get, set };

    /** @var bool */
    protected static flag { get };

    protected static label = "static" { toString };

    public static function plainValue()
    {
        return self::plain;
    }

    public static function countValue()
    {
        return self::count;
    }
}
