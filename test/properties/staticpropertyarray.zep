
namespace Test\Properties;

/**
 * @link https://github.com/phalcon/zephir/issues/367
 * @link https://github.com/phalcon/zephir/issues/188
 */
class StaticPropertyArray
{
	/**
	 * This is a public property with an initial empty-array value
	 */
	public static someEmptyArray = [];

	/**
	 * This is a public property with an initial array value
	 */
	public static someArray = [1, 2, 3, 4];
}
