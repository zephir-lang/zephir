namespace Stub;

class ArrayAccessTest
{
    protected data;

	public static function exits()
	{
		var arr;

		let arr = new ArrayAccessObj();
		return isset arr["one"];
	}

	public static function get()
	{
		var arr;

		let arr = new ArrayAccessObj();
		return arr["two"];
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/645
     */
    public function issue645() -> array
    {
        let this->{"data"} = [];

        return this->{"data"};
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1155
     */
    public function issue1155() -> bool
    {
        array arr;
        var s;

        let arr = ["0", "false", ""];
        let s = "true";

        return in_array(s, arr);
    }

    public function issue1094Test1(const array items = null) -> bool
    {
        bool isItemsNULL;

        // This syntax do not exist in Zephir...
        //let isItemsNULL = null === items;

        return isItemsNULL;
    }

    public function issue1094Test2(const array items = null) -> bool
    {
        bool isItemsNULL;

        let isItemsNULL = items === null;

        return isItemsNULL;
    }

    public function issue1094Test3(const array items = null) -> bool
    {
        bool isItemsNULL;

        let isItemsNULL = is_null(items);

        return isItemsNULL;
    }

    public static function issue1086Strict(array! params)
    {
        let params["test2"] = 1234;
    }

    public static function issue1086WontNullArrayAfterPassViaStaticWithStrictParams() -> array
    {
        array params;

        let params = [];
        let params["test"] = 123;

        self::issue1086Strict(params);

        return params;
    }

    public static function issue1086NotStrictParams(array params)
    {
        let params["test2"] = 1234;
    }

    public static function issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams() -> array
    {
        array params;

        let params = [];
        let params["test"] = 123;

        self::issue1086NotStrictParams(params);

        return params;
    }
}
