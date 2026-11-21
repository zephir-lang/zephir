namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2670
 *
 * PHP 8.6 support: kernel paths whose engine API moved in 8.6, each
 * compared against plain PHP by the test.
 */
class Issue2670
{
	/**
	 * Runs unique_key() `count` times inside the extension, so a string
	 * leaked per call shows up in one memory_get_usage() window.
	 */
	public function uniqueKeyLoop(var prefix, var value, long count) -> var
	{
		var key = null;
		long i = 0;

		while i < count {
			let key = unique_key(prefix, value);
			let i++;
		}

		return key;
	}

	public function emptyValue(var obj) -> bool
	{
		return empty obj->value;
	}

	public function issetValue(var obj) -> bool
	{
		return isset obj->value;
	}
}
