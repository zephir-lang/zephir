namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2684
 *
 * `zephir_fast_explode()` and `zephir_fast_explode_str()` accepted only
 * strings and answered anything else with a warning and an empty string.
 * PHP coerces int, float, bool, null and Stringable arguments and throws
 * `TypeError` for the rest; the limit follows `Z_PARAM_LONG` in the same way.
 *
 * One method per path through Zephir\Optimizers\FunctionCall\ExplodeOptimizer.
 */
class Issue2684
{
	public function explodeNoLimit(var delimiter, var source)
	{
		return explode(delimiter, source);
	}

	public function explodeStr(var source)
	{
		return explode(",", source);
	}

	public function explodeLimit(var delimiter, var source, var limit)
	{
		return explode(delimiter, source, limit);
	}

	public function explodeStrLimit(var source, var limit)
	{
		return explode(",", source, limit);
	}

	public function explodeConstLimit(var source)
	{
		return explode(",", source, 2);
	}

	/**
	 * The statement after a throwing explode() must not run.
	 */
	public function stopsAfterThrow(var delimiter, var source)
	{
		var parts;

		let parts = explode(delimiter, source);
		echo "reached";

		return parts;
	}

	/**
	 * Inside a try the throw jumps to the catch.
	 */
	public function catchesInTry(var delimiter, var source)
	{
		var parts, e;

		try {
			let parts = explode(delimiter, source);
			echo "reached";

			return parts;
		} catch \TypeError|\ValueError, e {
			return get_class(e);
		}
	}
}
