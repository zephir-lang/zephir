
namespace Stub\Properties;

/**
 * @link https://github.com/zephir-lang/zephir/issues/367
 * @link https://github.com/zephir-lang/zephir/issues/188
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
