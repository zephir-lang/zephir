namespace Stub;

class ArraySearch
{
	/** @issue https://github.com/phalcon/zephir/issues/1609 */
	public function simpleSearch(var needle, array haystack, bool strict = false) -> var
	{
		return array_search(needle, haystack, strict);
	}

	/** @issue https://github.com/phalcon/zephir/issues/1609 */
	public function searchUsingArrayInsideZephir() -> var
	{
		array itoA64 = [".", "/", "0", "1", "2", "3", "4", "value", "OtherValue"];
		var pos = array_search("value", itoA64);

		return pos;
	}
}
