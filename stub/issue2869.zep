namespace Stub;

/**
 * Constants of PHP classes whose values depend on the PHP that compiles the
 * extension, not on the one that ran `zephir generate` (#2869). PHP 8.5
 * renumbered the `PDO::FETCH_*` flags and PHP 8.1 the `FilesystemIterator`
 * flags. Any other constant of a PHP class is read when the code runs.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
class Issue2869
{
    const GROUP = \PDO::FETCH_GROUP;
    const UNIQUE_ASSOC = \PDO::FETCH_ASSOC | \PDO::FETCH_UNIQUE;
    const LIST = [\PDO::FETCH_CLASSTYPE, "late": \PDO::FETCH_PROPS_LATE, "serialize": \PDO::FETCH_SERIALIZE];
    const SYMLINKS = \RecursiveDirectoryIterator::FOLLOW_SYMLINKS;
    const MASK = \FilesystemIterator::OTHER_MODE_MASK;

    public serialize = \PDO::FETCH_SERIALIZE;
    public int mask = \FilesystemIterator::OTHER_MODE_MASK;

    public static function withDefault(int flags = \PDO::FETCH_GROUP) -> int
    {
        return flags;
    }

    public static function grouped(int flags) -> array
    {
        return [\PDO::FETCH_GROUP | flags];
    }

    public static function inverted() -> int
    {
        return ~\PDO::FETCH_GROUP;
    }

    public static function unique() -> int
    {
        return \PDO::FETCH_UNIQUE;
    }

    public static function isGroup(int mode) -> bool
    {
        return (mode & \PDO::FETCH_GROUP) == \PDO::FETCH_GROUP;
    }

    public static function symlinks() -> int
    {
        return \RecursiveDirectoryIterator::FOLLOW_SYMLINKS;
    }

    public static function assoc() -> int
    {
        return \PDO::FETCH_ASSOC;
    }

    public static function props() -> var
    {
        var flags;

        let flags = \ArrayObject::ARRAY_AS_PROPS;

        return flags;
    }
}
