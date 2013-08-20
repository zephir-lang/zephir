
namespace Test\Router;

/**
 * Test\Router\Route
 *
 * This class represents every route added to the router
 */
class Route
{

	protected _pattern;

	protected _compiledPattern;

	protected _paths;

	protected _methods;

	protected _hostname;

	protected _converters;

	protected _id;

	protected _name;

	protected _beforeMatch;

	/**
	 * Test\Router\Route constructor
	 *
	 * @param string pattern
	 * @param array paths
	 * @param array|string httpMethods
	 */
	public function __construct(pattern, paths=null, httpMethods=null)
	{
		// Configure the route (extract parameters, paths, etc)
		this->reConfigure(pattern, paths);

		// Update the HTTP method constraints
		let this->_methods = httpMethods;
	}

	/**
	 * Replaces placeholders from pattern returning a valid PCRE regular expression
	 *
	 * @param string pattern
	 * @return string
	 */
	public function compilePattern(pattern)
	{
		var compiledPattern, idPattern;

		let compiledPattern = pattern;

		// If a pattern contains ':', maybe there are placeholders to replace
		if memstr(pattern, ":") {

			// This is a pattern for valid identifiers
			let idPattern = "/([a-zA-Z0-9\_\-]+)";

			// Replace the module part
			if memstr(pattern, "/:module") {
				let compiledPattern = str_replace("/:module", idPattern, compiledPattern);
			}

			// Replace the controller placeholder
			if memstr(pattern, "/:controller") {
				let compiledPattern = str_replace("/:controller", idPattern, compiledPattern);
			}

			// Replace the namespace placeholder
			if memstr(pattern, "/:namespace") {
				let compiledPattern = str_replace("/:namespace", idPattern, compiledPattern);
			}

			// Replace the action placeholder
			if memstr(pattern, "/:action") {
				let compiledPattern = str_replace("/:action", idPattern, compiledPattern);
			}

			// Replace the params placeholder
			if memstr(pattern, "/:params") {
				let compiledPattern = str_replace("/:params", "(/.*)*", compiledPattern);
			}

			// Replace the int placeholder
			if memstr(pattern, "/:int") {
				let compiledPattern = str_replace("/:int", "/([0-9]+)", compiledPattern);
			}
		}

		// Check if the pattern has parantheses in order to add the regex delimiters
		if memstr(compiledPattern, "(") {
			return "#^" . compiledPattern . "$#";
		}

		// Square brackets are also checked
		if memstr(compiledPattern, "[") {
			return "#^" . compiledPattern . "$#";
		}

		return compiledPattern;
	}

	/**
	 * Set one or more HTTP methods that constraint the matching of the route
	 *
	 *<code>
	 * $route->via('GET');
	 * $route->via(array('GET', 'POST'));
	 *</code>
	 *
	 * @param string|array httpMethods
	 * @return Test\Router\Route
	 */
	public function via(httpMethods)
	{
		let this->_methods = httpMethods;
		return this;
	}

	/**
	 * Extracts parameters from a string
	 *
	 * @param string pattern
	 */
	public function extractNamedParams(string pattern)
	{

		char ch;
		var variable, regexp, tmp, matches;
		int cursor, marker, bracketCount = 0, parenthesesCount = 0;
		int intermediate = 0, length, numberMatches = 0, foundPattern;
		int variableLength, regexpLength, cursorVar;
		boolean notValid;
		string route, item;

		if strlen(pattern) <= 0 {
			return false;
		}

		for cursor, ch in pattern {

			if parenthesesCount == 0 {
				if ch == '{' {
					if bracketCount == 0 {
						let marker = cursor + 1,
							intermediate = 0,
							notValid = false,
							matches = [];
					}
					let bracketCount++;
				} else {
					if ch == '}' {
						let bracketCount--;
						if intermediate > 0 {
							if bracketCount == 0 {

								let numberMatches++,
									variable = null,
									length = cursor - marker,
									item = (string) substr(pattern, marker, length);

								for cursorVar, ch in item {

									if ch == '\0' {
										break;
									}

									if cursorVar == 0 && !((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
										let notValid = true;
										break;
									}

									if (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <='9') || ch == '-' || ch == '_' || ch ==  ':' {
										if ch == ':' {
											let regexpLength = length - cursorVar - 1,
												variableLength = cursorVar - marker,
												variable = estrndup(marker, variableLength),
												regexp = estrndup(cursorVar + 1, regexpLength);
											break;
										}
									} else {
										let notValid = true;
										break;
									}

								}

								if !notValid {

									let tmp = numberMatches;

									if variable {
										if regexpLength > 0 {

											let foundPattern = 0;
											/*for (k = 0; k < regexp_length; k++) {
												if (regexp[k] == '\0') {
													break;
												}
												if (!found_pattern) {
													if (regexp[k] == '(') {
													found_pattern = 1;
													}
												} else {
													if (regexp[k] == ')') {
														found_pattern = 2;
														break;
													}
												}
											}

											if (found_pattern != 2) {
												smart_str_appendc(&route_str, '(');
												smart_str_appendl(&route_str, regexp, regexp_length);
												smart_str_appendc(&route_str, ')');
											} else {
												smart_str_appendl(&route_str, regexp, regexp_length);
											}
											zend_hash_update(Z_ARRVAL_P(matches), variable, variable_length + 1, &tmp, sizeof(zval *), NULL);*/
										}
									} else {
										let route .= "([^/]*)",
											matches[item] = tmp;
									}
								} else {
									let route .= '{',
										route .= item,
										route .= '}';
								}
								continue;
							}
						}
					}
				}
			}

			if bracketCount == 0 {
				if ch == '(' {
					let parenthesesCount++;
				} else {
					if ch == ')' {
						let parenthesesCount--;
						if parenthesesCount == 0 {
							let numberMatches++;
						}
					}
				}
			}

			if bracketCount > 0 {
				let intermediate++;
			} else {
				let route .= ch;
			}
		}

		return route;
	}

	/**
	 * Reconfigure the route adding a new pattern and a set of paths
	 *
	 * @param string pattern
	 * @param array paths
	 */
	public function reConfigure(pattern, paths=null)
	{
		var moduleName, controllerName, actionName,
			parts, routePaths, realClassName, namespaceName,
			pcrePattern, compiledPattern, reversed, extracted;

		if typeof pattern != "string" {
			throw new Test\Router\Exception("The pattern must be string");
		}

		if paths !== null {
			if typeof paths == "string" {

				let moduleName = null,
					controllerName = null,
					actionName = null;

				// Explode the short paths using the :: separator
				let parts = explode("::", paths);

				// Create the array paths dynamically
				switch count(parts) {
					case 3:
						let moduleName = parts[0],
							controllerName = parts[1],
							actionName = parts[2];
						break;
					case 2:
						let controllerName = parts[0],
							actionName = parts[1];
						break;
					case 1:
						let controllerName = parts[0];
						break;
				}

				let routePaths = [];

				// Process module name
				if moduleName !== null {
					let routePaths["module"] = moduleName;
				}

				// Process controller name
				if controllerName !== null {

					// Check if we need to obtain the namespace
					if memstr(controllerName, "\\") {

						// Extract the real class name from the namespaced class
						let realClassName = get_class_ns(controllerName);

						// Extract the namespace from the namespaced class
						let namespaceName = get_ns_class(controllerName);

						// Update the namespace
						if namespaceName {
							let routePaths["namespace"] = namespaceName;
						}
					} else {
						let realClassName = controllerName;
					}

					// Always pass the controller to lowercase
					let routePaths["controller"] = uncamelize(realClassName);
				}

				// Process action name
				if actionName !== null {
					let routePaths["action"] = actionName;
				}
			} else {
				let routePaths = paths;
			}
		} else {
			let routePaths = [];
		}

		if typeof routePaths !== "array" {
			throw new Test\Router\Exception("The route contains invalid paths");
		}

		// If the route starts with '#' we assume that it is a regular expression
		if !starts_with(pattern, "#") {

			if memstr(pattern, "{") {
				// The route has named parameters so we need to extract them
				//let extracted = this->extractNamedParams(pattern),
				//	pcrePattern = extracted[0],
				//	routePaths = extracted[1];
				let pcrePattern = pattern;
			} else {
				let pcrePattern = pattern;
			}

			// Transform the route's pattern to a regular expression
			let compiledPattern = this->compilePattern(pcrePattern);
		} else {
			let compiledPattern = pattern;
		}

		// Update the original pattern
		let this->_pattern = pattern;

		// Update the compiled pattern
		let this->_compiledPattern = compiledPattern;

		//Update the route's paths
		let this->_paths = routePaths;
	}

	/**
	 * Returns the route's name
	 *
	 * @return string
	 */
	public function getName()
	{
		return this->_name;
	}

	/**
	 * Sets the route's name
	 *
	 *<code>
	 * $router->add('/about', array(
	 *     'controller' => 'about'
	 * ))->setName('about');
	 *</code>
	 *
	 * @param string name
	 * @return Test\Router\Route
	 */
	public function setName(name)
	{
		let this->_name = name;
		return this;
	}

	/**
	 * Sets a callback that is called if the route is matched.
	 * The developer can implement any arbitrary conditions here
	 * If the callback returns false the route is treaded as not matched
	 *
	 * @param callback callback
	 * @return Test\Router\Route
	 */
	public function beforeMatch(callback)
	{
		let this->_beforeMatch = callback;
		return this;
	}

	/**
	 * Returns the 'before match' callback if any
	 *
	 * @return mixed
	 */
	public function getBeforeMatch()
	{
		return this->_beforeMatch;
	}

	/**
	 * Returns the route's id
	 *
	 * @return string
	 */
	public function getRouteId()
	{
		return this->_id;
	}

	/**
	 * Returns the route's pattern
	 *
	 * @return string
	 */
	public function getPattern()
	{
		return this->_pattern;
	}

	/**
	 * Returns the route's compiled pattern
	 *
	 * @return string
	 */
	public function getCompiledPattern()
	{
		return this->_compiledPattern;
	}

	/**
	 * Returns the paths
	 *
	 * @return array
	 */
	public function getPaths()
	{
		return this->_paths;
	}

	/**
	 * Returns the paths using positions as keys and names as values
	 *
	 * @return array
	 */
	public function getReversedPaths()
	{
		var reversed, path, position;

		let reversed = [];
		for path, position in this->_paths {
			let reversed[position] = path;
		}
		return reversed;
	}

	/**
	 * Sets a set of HTTP methods that constraint the matching of the route (alias of via)
	 *
	 *<code>
	 * $route->setHttpMethods('GET');
	 * $route->setHttpMethods(array('GET', 'POST'));
	 *</code>
	 *
	 * @param string|array httpMethods
	 * @return Test\Router\Route
	 */
	public function setHttpMethods(httpMethods)
	{
		let this->_methods = httpMethods;
		return this;
	}

	/**
	 * Returns the HTTP methods that constraint matching the route
	 *
	 * @return string|array
	 */
	public function getHttpMethods()
	{
		return this->_methods;
	}

	/**
	 * Sets a hostname restriction to the route
	 *
	 *<code>
	 * $route->setHostname('localhost');
	 *</code>
	 *
	 * @param string|array httpMethods
	 * @return Test\Router\Route
	 */
	public function setHostname(hostname)
	{
		let this->_hostname = hostname;
		return this;
	}

	/**
	 * Returns the hostname restriction if any
	 *
	 * @return string
	 */
	public function getHostname()
	{
		return this->_hostname;
	}

	/**
	 * Adds a converter to perform an additional transformation for certain parameter
	 *
	 * @param string name
	 * @param callable converter
	 * @return Test\Router\Route
	 */
	public function convert(name, converter)
	{
		let this->_converters[name] = converter;
		return this;
	}

	/**
	 * Returns the router converter
	 *
	 * @return array
	 */
	public function getConverters()
	{
		return this->_converters;
	}

}
