
/**
 * Test\Router
 *
 * <p>Test\Router is the standard framework router. Routing is the
 * process of taking a URI endpoint (that part of the URI which comes after the base URL) and
 * decomposing it into parameters to determine which module, controller, and
 * action of that controller should receive the request</p>
 *
 *<code>
 *
 *	$router = new Test\Router();
 *
 *	$router->add(
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

class Router
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
	 * Test\Router constructor
	 *
	 * @param boolean defaultRoutes
	 */
	public function __construct(defaultRoutes=true)
	{
		var routes, paths, route,
			paramsPattern, params;

		let routes = [];
		if defaultRoutes === true {

			/**
			 * Two routes are added by default to match /:controller/:action and /:controller/:action/:params
			 */
			let paths = ['controller': 1],
				route = new Test\Router\Route('#^/([a-zA-Z0-9\_\-]+)[/]{0,1}$#', paths),
				routes[] = route;

			let paths = ['controller': 1, 'action': 2, 'params': 3],
				route = new Test\Router\Route('#^/([a-zA-Z0-9\_\-]+)/([a-zA-Z0-9\.\_]+)(/.*)*$#', paths),
				routes[] = route;
		}

		let this->_params = [],
			this->_routes = routes;
	}

	/**
	 * Sets the dependency injector
	 *
	 * @param Phalcon\DiInterface dependencyInjector
	 */
	public function setDI(<Phalcon\DiInterface> dependencyInjector)
	{
		let this->_dependencyInjector = dependencyInjector;
	}

	/**
	 * Returns the internal dependency injector
	 *
	 * @return Phalcon\DiInterface
	 */
	public function getDI()
	{
		return this->_dependencyInjector;
	}

	/**
	 * Get rewrite info. This info is read from $_GET['_url']. This returns '/' if the rewrite information cannot be read
	 *
	 * @return string
	 */
	public function getRewriteUri()
	{
		var uriSource, url, urlParts, realUri;

		/**
		 * The developer can change the URI source
		 */
		let uriSource = this->_uriSource;

		/**
		 * By default we use $_GET['url'] to obtain the rewrite information
		 */
		if !uriSource {
			if isset _GET['_url'] {
				let url = _GET['_url'];
				if !url {
					return url;
				}
			}
		} else {
			/**
			 * Otherwise use the standard $_SERVER['REQUEST_URI']
		 	 */
		 	if isset _SERVER['REQUEST_URI'] {
				let url = _SERVER['REQUEST_URI'],
					urlParts = explode('?', url),
					realUri = urlParts[0];
				if !realUri {
					return realUri;
				}
			}
		}
		return '/';
	}

	/**
	 * Sets the URI source. One of the URI_SOURCE_* constants
	 *
	 *<code>
	 *	$router->setUriSource(Router::URI_SOURCE_SERVER_REQUEST_URI);
	 *</code>
	 *
	 * @param string uriSource
	 * @return Test\Router
	 */
	public function setUriSource(uriSource)
	{
		let this->_uriSource = uriSource;
		return this;
	}

	/**
	 * Set whether router must remove the extra slashes in the handled routes
	 *
	 * @param boolean remove
	 * @return Test\Router
	 */
	public function removeExtraSlashes(remove)
	{
		let this->_removeExtraSlashes = remove;
		return this;
	}

	/**
	 * Sets the name of the default namespace
	 *
	 * @param string namespaceName
	 * @return Test\Router
	 */
	public function setDefaultNamespace(namespaceName)
	{
		let this->_defaultNamespace = namespaceName;
		return this;
	}

	/**
	 * Sets the name of the default module
	 *
	 * @param string moduleName
	 * @return Test\Router
	 */
	public function setDefaultModule(moduleName)
	{
		let this->_defaultModule = moduleName;
		return this;
	}

	/**
	 * Sets the default controller name
	 *
	 * @param string controllerName
	 * @return Test\Router
	 */
	public function setDefaultController(controllerName)
	{
		let this->_defaultController = controllerName;
		return this;
	}

	/**
	 * Sets the default action name
	 *
	 * @param string actionName
	 * @return Test\Router
	 */
	public function setDefaultAction(actionName)
	{
		let this->_defaultAction = actionName;
		return this;
	}

	/**
	 * Sets an array of default paths. If a route is missing a path the router will use the defined here
	 * This method must not be used to set a 404 route
	 *
	 *<code>
	 * $router->setDefaults(array(
	 *		'module' => 'common',
	 *		'action' => 'index'
	 * ));
	 *</code>
	 *
	 * @param array defaults
	 * @return Test\Router
	 */
	public function setDefaults(defaults)
	{
		var namespaceName, moduleName, controllerName,
			actionName, params;

		if typeof defaults == "array" {
			throw new Test\Router\Exception("Defaults must be an array");
		}

		/**
		 * Set a default namespace
		 */
		if isset defaults['namespace'] {
			let namespaceName = defaults['namespace'],
				this->_defaultNamespace = namespaceName;
		}

		/**
		 * Set a default module
		 */
		if isset defaults['module'] {
			let moduleName = defaults['module'],
				this->_defaultModule = moduleName;
		}

		/**
		 * Set a default controller
		 */
		if isset defaults['controller'] {
			let controllerName = defaults['controller'],
				this->_defaultController = controllerName;
		}

		/**
		 * Set a default action
		 */
		if isset defaults['action'] {
			let actionName = defaults['action'],
				this->_defaultAction = actionName;
		}

		/**
		 * Set default parameters
		 */
		if isset defaults['params'] {
			let params = defaults['params'],
				this->_defaultParams = params;
		}

		return this;
	}

	/**
	 * Handles routing information received from the rewrite engine
	 *
	 *<code>
	 * //Read the info from the rewrite engine
	 * $router->handle();
	 *
	 * //Manually passing an URL
	 * $router->handle('/posts/edit/1');
	 *</code>
	 *
	 * @param string uri
	 */
	public function handle(uri=null)
	{
		var realUri, request, currentHostName, routeFound, parts,
			params, matches, routes, reversedRoutes, notFoundPaths,
			vnamespace, defaultNamespace, module, defaultModule,
			controller, defaultController, action, defaultAction,
			paramsStr, strParams, paramsMerge, defaultParams;

		if !uri {
			/**
		 	 * If 'uri' isn't passed as parameter it reads _GET['_url']
		 	 */
			let realUri = this->getRewriteUri();
		} else {
			let realUri = uri;
		}

		let request = null,
			currentHostName = null,
			routeFound = false,
			parts = [],
			params = [],
			matches = null,
			this->_wasMatched = false,
			this->_matchedRoute = null;

		/**
		 * Routes are traversed in reversed order
		 */
		let routes = this->_routes;
		let reversedRoutes = array_reverse(routes);

		for route in reversedRoutes {

			/**
			 * Look for HTTP method constraints
			 */
			let methods = route->getHttpMethods();
			if methods !== null {

				/**
				 * Retrieve the request service from the container
				 */
				if request === null {

					let dependencyInjector = this->_dependencyInjector;
					if typeof dependencyInjector != "object" {
						throw new Test\Router\Exception("A dependency injection container is required to access the 'request' service");
					}

					let service = 'request',
						request = dependencyInjector->getShared(service);
				}

				/**
				 * Check if the current method is allowed by the route
				 */
				let matchMethod = request->isMethod(methods);
				if matchMethod === false {
					continue;
				}
			}

			/**
			 * Look for hostname constraints
			 */
			let hostname = route->getHostName();
			if hostname !== null {

				/**
				 * Retrieve the request service from the container
				 */
				if request===null {

					let dependencyInjector = this->_dependencyInjector;
					if typeof dependencyInjector != "object" {
						throw new Test\Router\Exception("A dependency injection container is required to access the 'request' service");
					}

					let service = 'request',
						request = dependencyInjector->getShared(service);
				}

				/**
				 * Check if the current hostname is the same as the route
				 */
				if typeof currentHostName != "object" {
					let currentHostName = request->getHttpHost();
				}

				/**
				 * No HTTP_HOST, maybe in CLI mode?
				 */
				if typeof currentHostName != "null" {
					continue;
				}

				/**
				 * Check if the hostname restriction is the same as the current in the route
				 */
				if memchr(hostname, '(') {
					if memchr(hostname, '#') {
						let regexHostName = '#^' . hostname . '$#';
					} else {
						let regexHostName = hostname;
					}
					let matched = preg_match(regexHostName, currentHostName);
				} else {
					let matched = currentHostName == hostname;
				}

				if !matched {
					continue;
				}

			}

			/**
			 * If the route has parentheses use preg_match
			 */
			let pattern = route->getCompiledPattern();
			if memchr(pattern, '^') {
				let routeFound = preg_match(pattern, handledUri, matches);
			} else {
				let routeFound = pattern == handledUri;
			}

			/**
			 * Check for beforeMatch conditions
			 */
			if routeFound {

				let beforeMatch = route->getBeforeMatch();

				if beforeMatch !== null {

					/**
					 * Check first if the callback is callable
					 */
					if is_callable(beforeMatch) {
						throw new Test\Router\Exception("Before-Match callback is not callable in matched route");
					}

					/**
					 * Before-Match parameters
					 */
					let beforeMatchParams = [handledUri, route, this];

					/**
					 * Call the function in the PHP userland
					 */
					//let routeFound = {beforeMatch}(beforeMatchParams);
				}
			}

			if routeFound {

				/**
				 * Start from the default paths
				 */
				let paths = route->getPaths(),
					parts = paths;

				/**
				 * Check if the matches has variables
				 */
				if typeof matches != "array" {

					/**
					 * Get the route converters if any
					 */
					let converters = route->getConverters();

					for part, position in paths {

						if isset matches[position] {

							let matchPosition = matches[position];

							/**
							 * Check if the part has a converter
							 */
							if typeof converters == "array" {
								if isset converters[part] {
									//let parameters = [matchPosition],
									//	converter = converters[part],
									//	convertedPart = {converter}(parameters),
									//	parts[part] = convertedPart;
									continue;
								}
							}

							/**
							 * Update the parts if there is no converter
							 */
							let parts[part] = matchPosition;
						} else {

							/**
							 * Apply the converters anyway
							 */
							if typeof converters == "array" {
								if isset converters[part] {
									//let parameters = [matchPosition],
									//	converter = converters[part],
									//	convertedPart = {converter}(parameters),
									//	parts[part] = convertedPart;
								}
							}
						}
					}

					/**
					 * Update the matches generated by preg_match
					 */
					let this->_matches = matches;
				}

				let this->_matchedRoute = route;
				break;
			}
		}

		/**
		 * Update the wasMatched property indicating if the route was matched
		 */
		if routeFound {
			let this->_wasMatched = true;
		} else {
			let this->_wasMatched = false;
		}

		/**
		 * The route wasn't found, try to use the not-found paths
		 */
		if !routeFound {
			let notFoundPaths = this->_notFoundPaths;
			if notFoundPaths !== null {
				let parts = notFoundPaths,
					routeFound = true;
			}
		}

		if routeFound {

			/**
			 * Check for a namespace
			 */
			if isset parts['namespace'] {
				let vnamespace = parts['namespace'];
				if !is_numeric(vnamespace) {
					let this->_namespace = vnamespace;
				}
				unset parts['namespace'];
			} else {
				let defaultNamespace = this->_defaultNamespace,
					this->_namespace = defaultNamespace;
			}

			/**
			 * Check for a module
			 */
			if isset parts['module'] {
				let module = parts['module'];
				if !is_numeric(module) {
					let this->_module = module;
				}
				unset parts['module'];
			} else {
				let defaultModule = this->_defaultModule,
					this->_module = defaultModule;
			}

			/**
			 * Check for a controller
			 */
			if isset parts['controller'] {
				let controller = parts['controller'];
				if !is_numeric(controller) {
					let this->_controller = controller;
				}
				unset parts['controller'];
			} else {
				let defaultController = this->_defaultController,
					this->_controller = defaultController;
			}

			/**
			 * Check for an action
			 */
			if isset parts['action'] {
				let action = parts['action'];
				if !is_numeric(action) {
					let this->_action = action;
				}
				unset parts['action'];
			} else {
				let defaultAction = this->_defaultAction,
					this->_action = defaultAction;
			}

			/**
			 * Check for parameters
			 */
			if isset parts['params'] {
				let paramsStr = parts['params'],
					strParams = substr(paramsStr, 1);
				if (strParams) {
					let params = explode("/", strParams);
				}
				unset parts['params'];
			}

			if (count(params)) {
				let paramsMerge = array_merge(params, parts);
			} else {
				let paramsMerge = parts;
			}

			let this->_params = paramsMerge;

		} else {

			/**
			 * Use default values if the route hasn't matched
			 */
			let defaultNamespace = this->_defaultNamespace,
				this->_namespace = defaultNamespace,

				defaultModule = this->_defaultModule,
				this->_module = defaultModule,

				defaultController = this->_defaultController,
				this->_controller = defaultController,

				defaultAction = this->_defaultAction,
				this->_action = defaultAction,

				defaultParams = this->_defaultParams,
				this->_params = defaultParams;
		}
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
	 * @return Test\Router\Route
	 */
	public function add(pattern, paths=null, httpMethods=null)
	{
		var route;

		/**
		 * Every route is internally stored as a Test\Router\Route
		 */
		let route = new Test\Router\Route(pattern, paths, httpMethods),
			this->_routes[] = route;
		return route;
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is GET
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addGet(pattern, paths=null)
	{
		var method;
		let method = 'GET';
		return this->add(pattern, paths, method);
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is POST
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addPost(pattern, paths=null)
	{
		var method;
		let method = 'POST';
		return this->add(pattern, paths, method);
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is PUT
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addPut(pattern, paths=null)
	{
		var method;
		let method = 'PUT';
		return this->add(pattern, paths, method);
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is PATCH
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addPatch(pattern, paths=null)
	{
		var method;
		let method = 'PATCH';
		return this->add(pattern, paths, method);
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is DELETE
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addDelete(pattern, paths=null)
	{
		var method;
		let method = 'DELETE';
		return this->add(pattern, paths, method);
	}

	/**
	 * Add a route to the router that only match if the HTTP method is OPTIONS
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addOptions(pattern, paths=null)
	{
		var method;
		let method = 'OPTIONS';
		return this->add(pattern, paths, method);
	}

	/**
	 * Adds a route to the router that only match if the HTTP method is HEAD
	 *
	 * @param string pattern
	 * @param string/array paths
	 * @return Test\Router\Route
	 */
	public function addHead(pattern, paths=null)
	{
		var method;
		let method = 'HEAD';
		return this->add(pattern, paths, method);
	}

	/**
	 * Mounts a group of routes in the router
	 *
	 * @param Test\Router\Group route
	 * @return Test\Router
	 */
	public function mount(group)
	{

		var groupRoutes, beforeMatch, hostname, routes, newRoutes;

		if typeof group != "object" {
			throw new Test\Router\Exception("The group of routes is not valid");
		}

		let groupRoutes = group->getRoutes();

		if !count(groupRoutes) {
			throw new Test\Router\Exception("The group of routes does not contain any routes");
		}

		/**
		 * Get the before-match condition
		 */
		let beforeMatch = group->getBeforeMatch();

		if beforeMatch !== null {
			for route in groupRoutes {
				//route->beforeMatch(beforeMatch);
			}
		}

		/**
		 * Get the hostname restriction
		 */
		let hostname = group->getHostName();

		if hostname !== null {
			for route in groupRoutes {
				//route->setHostName(hostname);
			}
		}

		let routes = this->_routes;

		if typeof routes == "array" {
			let newRoutes = array_merge(routes, groupRoutes),
				this->_routes = newRoutes;
		} else {
			let this->_routes = groupRoutes;
		}

		return this;
	}

	/**
	 * Set a group of paths to be returned when none of the defined routes are matched
	 *
	 * @param array paths
	 * @return Test\Router
	 */
	public function notFound(paths)
	{
		if typeof paths != "array" {
			if typeof paths != "string" {
				throw new Test\Router\Exception("The not-found paths must be an array or string");
			}
		}
		let this->_notFoundPaths = paths;
		return this;
	}

	/**
	 * Removes all the pre-defined routes
	 */
	public function clear()
	{
		let this->_routes = [];
	}

	/**
	 * Returns the processed namespace name
	 *
	 * @return string
	 */
	public function getNamespaceName()
	{
		return this->_namespace;
	}

	/**
	 * Returns the processed module name
	 *
	 * @return string
	 */
	public function getModuleName()
	{
		return this->_module;
	}

	/**
	 * Returns the processed controller name
	 *
	 * @return string
	 */
	public function getControllerName()
	{
		return this->_controller;
	}

	/**
	 * Returns the processed action name
	 *
	 * @return string
	 */
	public function getActionName()
	{
		return this->_action;
	}

	/**
	 * Returns the processed parameters
	 *
	 * @return array
	 */
	public function getParams()
	{
		return this->_params;
	}

	/**
	 * Returns the route that matchs the handled URI
	 *
	 * @return Test\Router\Route
	 */
	public function getMatchedRoute()
	{
		return this->_matchedRoute;
	}

	/**
	 * Returns the sub expressions in the regular expression matched
	 *
	 * @return array
	 */
	public function getMatches()
	{
		return this->_matches;
	}

	/**
	 * Checks if the router macthes any of the defined routes
	 *
	 * @return bool
	 */
	public function wasMatched()
	{
		return this->_wasMatched;
	}

	/**
	 * Returns all the routes defined in the router
	 *
	 * @return Test\Router\Route[]
	 */
	public function getRoutes()
	{
		return this->_routes;
	}

	/**
	 * Returns a route object by its id
	 *
	 * @param string id
	 * @return Test\Router\Route
	 */
	public function getRouteById(id)
	{
		var routes, routeId;
		let routes = this->_routes;
		for route in routes {
			let routeId = route->getRouteId();
			if routeId == id {
				return route;
			}
		}
		return false;
	}

	/**
	 * Returns a route object by its name
	 *
	 * @param string name
	 * @return Test\Router\Route
	 */
	public function getRouteByName(name)
	{
		var routes, routeId;
		let routes = this->_routes;
		for route in routes {
			let routeName = route->getName();
			if routeName == name {
				return route;
			}
		}
		return false;
	}

}
