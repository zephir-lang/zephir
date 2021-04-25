namespace Stub;

class ArrayAccessTest
{
    protected data;
    protected assigedFromMethod;
    protected unsetData = [
        "key_a": "marcin",
        "key_b": "paula",
        3: "long value"
        //3.14: "double value", // Syntax error
        //false: "bool value", // Syntax error
        //null: "null value"  // Syntax error
    ];

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

	public function unsetByKeyFromArray(string! key, array data) -> array
	{
	    unset data[key];

	    return data;
	}

	public function unsetByKeyFromProperty(string! key, array dataFromProperty) -> array
	{
	    let this->assigedFromMethod = dataFromProperty;

	    unset this->assigedFromMethod[key];

	    return this->assigedFromMethod;
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

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public static function issue1086Strict(array! params)
    {
        let params["test2"] = 1234;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public static function issue1086WontNullArrayAfterPassViaStaticWithStrictParams() -> array
    {
        array params;

        let params = [];
        let params["test"] = 123;

        self::issue1086Strict(params);

        return params;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public static function issue1086NotStrictParams(array params)
    {
        let params["test2"] = 1234;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1086
     */
    public static function issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams() -> array
    {
        array params;

        let params = [];
        let params["test"] = 123;

        self::issue1086NotStrictParams(params);

        return params;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1259
     */
    public function issue1259UnsetKeyFromArrayInternalVariable() -> array
    {
        array ret = [];
        array unsetData = ["key_a": "marcin", "key_b": "paula"];

        let ret[] = unsetData;
        unset(unsetData["key_a"]);
        let ret[] = unsetData;

        return ret;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1259
     */
    public function issue1259UnsetStringKeyFromArrayProperty() -> array
    {
        array ret = [];

        let ret[] = this->unsetData;
        unset(this->unsetData["key_a"]);
        let ret[] = this->unsetData;

        return ret;
    }

    public function issue1259UnsetLongKeyFromArrayProperty() -> array
    {
        array ret = [];

        let ret[] = this->unsetData;
        unset(this->unsetData[3]);
        let ret[] = this->unsetData;

        return ret;
    }

    /*
    public function issue1259UnsetDoubleKeyFromArrayProperty() -> array
    {
        array ret = [];

        let ret[] = this->unsetData;
        unset(this->unsetData[3.14]);
        let ret[] = this->unsetData;

        return ret;
    }
    */

    /*
    public function issue1259UnsetBoolKeyFromArrayProperty() -> array
    {
        array ret = [];

        let ret[] = this->unsetData;
        unset(this->unsetData[false]);
        let ret[] = this->unsetData;

        return ret;
    }
    */

    /*
    public function issue1259UnsetNullKeyFromArrayProperty() -> array
    {
        array ret = [];

        let ret[] = this->unsetData;
        unset(this->unsetData[null]);
        let ret[] = this->unsetData;

        return ret;
    }
    */
}
