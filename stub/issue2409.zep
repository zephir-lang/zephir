namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2409
 */
class Issue2409
{
    protected static someVar;

    public static function setEmpty() -> void
    {
        let self::someVar = "";
    }

    public static function setNonEmpty() -> void
    {
        let self::someVar = "123";
    }

    public static function setNull() -> void
    {
        let self::someVar = null;
    }

    public static function setInt() -> void
    {
        let self::someVar = 42;
    }

    public static function setNegativeInt() -> void
    {
        let self::someVar = -7;
    }

    public static function setDouble() -> void
    {
        let self::someVar = 3.14;
    }

    public static function setBoolTrue() -> void
    {
        let self::someVar = true;
    }

    public static function setBoolFalse() -> void
    {
        let self::someVar = false;
    }

    public static function setChar() -> void
    {
        char ch = 'A';
        let self::someVar = ch;
    }

    public static function setEmptyArray() -> void
    {
        let self::someVar = [];
    }

    public static function setArray() -> void
    {
        let self::someVar = [1, 2, 3];
    }

    public static function setFromStringVariable() -> void
    {
        string tmp = "hello";
        let self::someVar = tmp;
    }

    public static function setFromIntVariable() -> void
    {
        int tmp = 99;
        let self::someVar = tmp;
    }

    public static function setFromDoubleVariable() -> void
    {
        double tmp = 2.5;
        let self::someVar = tmp;
    }

    public static function setFromBoolVariable() -> void
    {
        bool tmp = true;
        let self::someVar = tmp;
    }

    public static function getVar()
    {
        return self::someVar;
    }
}
