namespace Stub;

/**
 * `self::` and `parent::` read the `FilesystemIterator` flags a Zephir
 * subclass of `RecursiveDirectoryIterator` inherits.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
class Issue2869Directory extends \RecursiveDirectoryIterator
{
    public static function symlinks() -> int
    {
        return self::FOLLOW_SYMLINKS;
    }

    public static function mask() -> int
    {
        return parent::OTHER_MODE_MASK;
    }

    public static function skipDots() -> int
    {
        return self::SKIP_DOTS;
    }
}
