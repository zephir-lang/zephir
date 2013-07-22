
/**
 * Test\Router\Route
 *
 * This class represents every route added to the router
 */

namespace Test\Router;

class Route
{

	protected _pattern = null;

	protected _compiledPattern = null;

	protected _paths = null;

	protected _methods = null;

	protected _hostname = null;

	protected _converters = null;

	protected _id;

	protected _name;

	protected _beforeMatch;

	/**
	 * Phalcon\Mvc\Router\Route constructor
	 *
	 * @param string pattern
	 * @param array paths
	 * @param array|string httpMethods
	 */
	public function __construct(pattern, paths=null, httpMethods=null)
	{

	}

}
