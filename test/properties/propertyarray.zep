
namespace Test\Properties;

/**
 * @link https://github.com/phalcon/zephir/issues/520
 */
class PropertyArray
{
	/**
	 * This is a public property with an initial empty-array value
	 */
	public someEmptyArray = [];

	/**
	 * This is a public property with an initial array value
	 */
	public someArray = [1, 2, 3, 4];

	public function __construct()
	{
		let this->someArray = [1];
	}
}