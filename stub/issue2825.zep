namespace Stub;

use DateTime;
use ArrayObject as Renamed;
use \Countable;

/**
 * `X::class` is the name PHP resolves at compile time: as written, through
 * `use` aliases matched in any case, without a leading backslash and without
 * loading the class. `parent::class` is the `extends` name as written.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
class Issue2825 extends \ArrayObject
{
    const GLOBAL_NAME = DateTime::class;

    const ALIAS_NAME = Renamed::class;

    const FOLDED_NAME = DateTime::class . "!";

    public propDefault = DateTime::class;

    public static staticDefault = Renamed::class;

    public static function imported() -> string
    {
        return DateTime::class;
    }

    public static function aliased() -> string
    {
        return Renamed::class;
    }

    public static function aliasInOtherCase() -> string
    {
        return datetime::class;
    }

    public static function qualified() -> string
    {
        return \DateTime::class;
    }

    public static function importedWithBackslash() -> string
    {
        return Countable::class;
    }

    public static function keptAsWritten() -> string
    {
        return \datetime::class;
    }

    public static function unknown() -> string
    {
        return NotLoaded::class;
    }

    public static function unknownQualified() -> string
    {
        return \Foo\bar::class;
    }

    public static function concatenated() -> string
    {
        return DateTime::class . "!";
    }

    public function parentName() -> string
    {
        return parent::class;
    }

    public static function newAliasInOtherCase() -> var
    {
        return new datetime("2020-01-01");
    }
}
