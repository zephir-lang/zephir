
/**
 * Phalcon\Mvc\Router
 *
 * <p>Phalcon\Mvc\Router is the standard framework router. Routing is the
 * process of taking a URI endpoint (that part of the URI which comes after the base URL) and
 * decomposing it into parameters to determine which module, controller, and
 * action of that controller should receive the request</p>
 *
 *<code>
 *
 *	$router = new Phalcon\Mvc\Router();
 *
 *  $router->add(
 *		"/documentation/{chapter}/{name}.{type:[a-z]+}",
 *		array(
 *			"controller" => "documentation",
 *			"action"     => "show"
 *		)
 *	);
 *
 *	$router->handle();
 *
 *	echo $router->getControllerName();
 *</code>
 *
 */

namespace Test;

abstract class Router
{
	protected _dependencyInjector;

	protected _uriSource;

	protected _namespace = null;

	protected _module = null;

	protected _controller = null;

	protected _action = null;

	protected _params;

	protected _routes;

	protected _matchedRoute;

	protected _matches;

	protected _wasMatched = false;

	protected _defaultNamespace;

	protected _defaultModule;

	protected _defaultController;

	protected _defaultAction;

	protected _defaultParams;

	protected _removeExtraSlashes;

	protected _notFoundPaths;

	const URI_SOURCE_GET_URL = 0;

	const URI_SOURCE_SERVER_REQUEST_URI = 1;

	/**
	 * Phalcon\Mvc\Router constructor
	 *
	 * @param boolean defaultRoutes
	 */
	public function __construct(defaultRoutes=true)
	{
		var routes, paths, actionPattern, route, paramsPattern;

		let routes = [];
		if (defaultRoutes === true) {

			/**
			 * Two routes are added by default to match /:controller/:action and /:controller/:action/:params
			 */
			let paths = ['controller': 1];
			let actionPattern = '#^/([a-zA-Z0-9\_\-]+)[/]{0,1}$#';
			let route = new Phalcon\Mvc\Router\Route(actionPattern, paths);
			let routes[] = route;

			let paths = ['controller': 1, 'action': 2, 'params': 3];
			let paramsPattern = '#^/([a-zA-Z0-9\_\-]+)/([a-zA-Z0-9\.\_]+)(/.*)*$#';
			let route = new Phalcon\Mvc\Router\Route(paramsPattern, paths);
			let routes[] = route;
		}

		let params = [];

		let this->_params = params;
		let this->_routes = routes;
	}

	/**
	 * Sets the dependency injector
	 *
	 * @param Phalcon\DiInterface dependencyInjector
	 */
	public function setDI(dependencyInjector)
	{
		/*$this->_dependencyInjector = $dependencyInjector;*/
	}

	/*
	 * Returns the internal dependency injector
	 *
	 * @return Phalcon\DiInterface
	 */
	public function getDI()
	{
		/*return $this->_dependencyInjector;*/
	}

	/**
	 * Adds a route to the router without any HTTP constraint
	 *
	 *<code>
	 * $router->add('/about', 'About::index');
	 *</code>
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @param string httpMethods
	 * @return Phalcon\Mvc\Router\Route
	 */
	public function add(int pattern, paths=null, httpMethods=null)
	{
		var method;

		let method = 'GET';
		if (method == 1) {

			/**
			 * This is a comment yay!
			 */
			echo method;

			echo method;
		}

	}

}
