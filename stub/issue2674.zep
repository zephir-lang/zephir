namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2674
 *
 * `zephir_fast_explode()` and `zephir_fast_explode_str()` handed every limit
 * straight to `php_explode()`, which PHP itself calls only when `limit > 1`.
 * A negative limit belongs to `php_explode_negative_limit()` and a limit of 0
 * or 1 returns the whole subject as a single element, so every limit <= 1
 * produced the `limit == 2` result. An empty separator is a `ValueError` in
 * PHP; here it spun forever, because `zend_memnstr()` answers a zero-length
 * needle with the haystack instead of NULL.
 *
 * One method per path through Zephir\Optimizers\FunctionCall\ExplodeOptimizer.
 */
class Issue2674
{
	/**
	 * Literal separator, runtime limit: `zephir_fast_explode_str()`.
	 */
	public function explodeStrLimit(var source, var limit)
	{
		return explode(",", source, limit);
	}

	/**
	 * Runtime separator, runtime limit: `zephir_fast_explode()`.
	 */
	public function explodeLimit(var delimiter, var source, var limit)
	{
		return explode(delimiter, source, limit);
	}

	/**
	 * No limit argument at all, so the optimizer supplies `ZEND_LONG_MAX`.
	 */
	public function explodeNoLimit(var delimiter, var source)
	{
		return explode(delimiter, source);
	}

	/**
	 * Literal separator and a compile-time constant limit, which the
	 * optimizer emits as a C literal instead of `zephir_get_intval()`.
	 */
	public function explodeConstLimit(var source)
	{
		return explode(",", source, -2);
	}
}
