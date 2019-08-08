
namespace Test\Properties;

/**
 * @link https://github.com/phalcon/zephir/issues/367
 * @link https://github.com/phalcon/zephir/issues/188
 */
class StaticPropertyReference
{
	/**
	 * This is a public property that will be referenced by variable
	 */
	public static reference;

	public static function setValue(val)
	{
		let self::reference = val;
	}
}
