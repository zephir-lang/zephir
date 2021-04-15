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
}
