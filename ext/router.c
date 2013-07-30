
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"


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
ZEPHIR_INIT_CLASS(Test_Router) {

	ZEPHIR_REGISTER_CLASS(Test, Router, router, test_router_method_entry, 0);

	zend_declare_property_null(test_router_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_uriSource"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_namespace"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_module"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_controller"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_action"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_params"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_routes"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_matchedRoute"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_matches"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(test_router_ce, SL("_wasMatched"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultController"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_removeExtraSlashes"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_notFoundPaths"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

/**
 * Test\Router constructor
 *
 * @param boolean defaultRoutes
 */
PHP_METHOD(Test_Router, __construct) {

	zval *defaultRoutes, *routes = NULL, *paths = NULL, *actionPattern, *route, *paramsPattern, *_0;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 1, 0, &defaultRoutes);

	ZEPHIR_INIT_VAR(routes);
	array_init(routes);

	if (ZEPHIR_IS_TRUE(defaultRoutes)) {
		//missing comment

		ZEPHIR_INIT_VAR(paths);
		array_init(paths);
		add_assoc_long_ex(paths, SS("controller"), 1);

		ZEPHIR_INIT_VAR(actionPattern);
		ZVAL_STRING(actionPattern, "#^/([a-zA-Z0-9\\_\\-]+)[/]{0,1}$#", 1);

		object_init_ex(route, test_router_route_ce);
		zephir_call_method_p2_noret(route, "__construct", actionPattern, paths);

		ZEPHIR_INIT_NVAR(routes);
		zephir_array_append(&routes, route, PH_SEPARATE);

		ZEPHIR_INIT_NVAR(paths);
		array_init(paths);
		add_assoc_long_ex(paths, SS("controller"), 1);
		add_assoc_long_ex(paths, SS("action"), 2);
		add_assoc_long_ex(paths, SS("params"), 3);

		ZEPHIR_INIT_VAR(paramsPattern);
		ZVAL_STRING(paramsPattern, "#^/([a-zA-Z0-9\\_\\-]+)/([a-zA-Z0-9\\.\\_]+)(/.*)*$#", 1);

		object_init_ex(route, test_router_route_ce);
		zephir_call_method_p2_noret(route, "__construct", paramsPattern, paths);

		ZEPHIR_INIT_NVAR(routes);
		zephir_array_append(&routes, route, PH_SEPARATE);

	}

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_zval(this, SL("_params"), _0 TSRMLS_CC);

	zephir_update_property_this(this_ptr, SL("_routes"), routes TSRMLS_CC);

	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the dependency injector
 *
 * @param Phalcon\DiInterface dependencyInjector
 */
PHP_METHOD(Test_Router, setDI) {

	zval *dependencyInjector;


	zephir_fetch_params(0, 1, 0, &dependencyInjector);

	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);


}

/**
 * Returns the internal dependency injector
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Test_Router, getDI) {

	RETURN_MEMBER("_dependencyInjector");

}

/**
 * Get rewrite info. This info is read from $_GET['_url']. This returns '/' if the rewrite information cannot be read
 *
 * @return string
 */
PHP_METHOD(Test_Router, getRewriteUri) {

	zval *uriSource, *url = NULL, *urlParts, *realUri, *_GET, *_SERVER;

	ZEPHIR_MM_GROW();
	//missing comment

	//missing

	//missing comment
	if (!(zend_is_true(uriSource))) {
		if (phalcon_array_isset_string(_GET, SS("_url")) {

			ZEPHIR_OBS_VAR(url);
			zephir_array_fetch_string(&url, _GET, SL("_url"), PH_NOISY);

			if (!(zend_is_true(url))) {

				RETURN_CCTOR(url);

			}
		}
	} else {
		//missing comment
		if (phalcon_array_isset_string(_SERVER, SS("REQUEST_URI")) {

			ZEPHIR_OBS_NVAR(url);
			zephir_array_fetch_string(&url, _SERVER, SL("REQUEST_URI"), PH_NOISY);

			//missing

			ZEPHIR_OBS_VAR(realUri);
			zephir_array_fetch_long(&realUri, urlParts, 0, PH_NOISY);

			if (!(zend_is_true(realUri))) {

				RETURN_CCTOR(realUri);

			}
		}
	}

	RETURN_MM_STRING("/", 1);


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
PHP_METHOD(Test_Router, setUriSource) {

	zval *uriSource;


	zephir_fetch_params(0, 1, 0, &uriSource);

	zephir_update_property_this(this_ptr, SL("_uriSource"), uriSource TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Set whether router must remove the extra slashes in the handled routes
 *
 * @param boolean remove
 * @return Test\Router
 */
PHP_METHOD(Test_Router, removeExtraSlashes) {

	zval *remove;


	zephir_fetch_params(0, 1, 0, &remove);

	zephir_update_property_this(this_ptr, SL("_removeExtraSlashes"), remove TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Sets the name of the default namespace
 *
 * @param string namespaceName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultNamespace) {

	zval *namespaceName;


	zephir_fetch_params(0, 1, 0, &namespaceName);

	zephir_update_property_this(this_ptr, SL("_defaultNamespace"), namespaceName TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Sets the name of the default module
 *
 * @param string moduleName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultModule) {

	zval *moduleName;


	zephir_fetch_params(0, 1, 0, &moduleName);

	zephir_update_property_this(this_ptr, SL("_defaultModule"), moduleName TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Sets the default controller name
 *
 * @param string controllerName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultController) {

	zval *controllerName;


	zephir_fetch_params(0, 1, 0, &controllerName);

	zephir_update_property_this(this_ptr, SL("_defaultController"), controllerName TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Sets the default action name
 *
 * @param string actionName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultAction) {

	zval *actionName;


	zephir_fetch_params(0, 1, 0, &actionName);

	zephir_update_property_this(this_ptr, SL("_defaultAction"), actionName TSRMLS_CC);

	RETURN_THISW(this);


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
PHP_METHOD(Test_Router, setDefaults) {

	zval *defaults, *namespaceName, *moduleName, *controllerName, *actionName, *params;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 1, 0, &defaults);

	if (Z_TYPE_P(defaults) == IS_ARRAY) {
		//missing throw
	}
	//missing comment
	if (phalcon_array_isset_string(defaults, SS("namespace")) {

		ZEPHIR_OBS_VAR(namespaceName);
		zephir_array_fetch_string(&namespaceName, defaults, SL("namespace"), PH_NOISY);

		zephir_update_property_this(this_ptr, SL("_defaultNamespace"), namespaceName TSRMLS_CC);

	}
	//missing comment
	if (phalcon_array_isset_string(defaults, SS("module")) {

		ZEPHIR_OBS_VAR(moduleName);
		zephir_array_fetch_string(&moduleName, defaults, SL("module"), PH_NOISY);

		zephir_update_property_this(this_ptr, SL("_defaultModule"), moduleName TSRMLS_CC);

	}
	//missing comment
	if (phalcon_array_isset_string(defaults, SS("controller")) {

		ZEPHIR_OBS_VAR(controllerName);
		zephir_array_fetch_string(&controllerName, defaults, SL("controller"), PH_NOISY);

		zephir_update_property_this(this_ptr, SL("_defaultController"), controllerName TSRMLS_CC);

	}
	//missing comment
	if (phalcon_array_isset_string(defaults, SS("action")) {

		ZEPHIR_OBS_VAR(actionName);
		zephir_array_fetch_string(&actionName, defaults, SL("action"), PH_NOISY);

		zephir_update_property_this(this_ptr, SL("_defaultAction"), actionName TSRMLS_CC);

	}
	//missing comment
	if (phalcon_array_isset_string(defaults, SS("params")) {

		ZEPHIR_OBS_VAR(params);
		zephir_array_fetch_string(&params, defaults, SL("params"), PH_NOISY);

		zephir_update_property_this(this_ptr, SL("_defaultParams"), params TSRMLS_CC);

	}

	RETURN_THIS(this);


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
PHP_METHOD(Test_Router, handle) {

	zval *uri, *realUri, *request, *currentHostName, *routeFound = NULL, *parts, *params, *matches, *routes, *reversedRoutes, *notFoundPaths, *vnamespace, *defaultNamespace, *module, *defaultModule, *controller, *defaultController, *action, *defaultAction, *paramsStr, *strParams, *paramsMerge, *defaultParams, *_0, *_1, *_2, *_3;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 1, 0, &uri);

	if (!(zend_is_true(uri))) {
		//missing comment

		//missing

	} else {

		ZEPHIR_CPY_WRT(realUri, uri);

	}

	ZEPHIR_INIT_VAR(request);
	ZVAL_NULL(request);

	ZEPHIR_INIT_VAR(currentHostName);
	ZVAL_NULL(currentHostName);

	ZEPHIR_INIT_VAR(routeFound);
	ZVAL_BOOL(routeFound, 0);

	ZEPHIR_INIT_VAR(parts);
	array_init(parts);

	ZEPHIR_INIT_VAR(params);
	array_init(params);

	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);

	ZEPHIR_INIT_VAR(_0);
	zephir_update_property_zval(this, SL("_wasMatched"), _0 TSRMLS_CC);

	ZEPHIR_INIT_VAR(_1);
	zephir_update_property_zval(this, SL("_matchedRoute"), _1 TSRMLS_CC);

	//missing comment

	//missing

	//missing

	//missing for
	//missing comment
	if (zend_is_true(routeFound)) {

		ZEPHIR_INIT_VAR(_2);
		zephir_update_property_zval(this, SL("_wasMatched"), _2 TSRMLS_CC);

	} else {

		ZEPHIR_INIT_VAR(_3);
		zephir_update_property_zval(this, SL("_wasMatched"), _3 TSRMLS_CC);

	}
	//missing comment
	if (!(zend_is_true(routeFound))) {

		//missing

		if (Z_TYPE_P(notFoundPaths) == IS_NULL) {

			ZEPHIR_CPY_WRT(parts, notFoundPaths);

			ZEPHIR_INIT_NVAR(routeFound);
			ZVAL_BOOL(routeFound, 1);

		}
	}
	if (zend_is_true(routeFound)) {
		//missing comment
		if (phalcon_array_isset_string(parts, SS("namespace")) {

			ZEPHIR_OBS_VAR(vnamespace);
			zephir_array_fetch_string(&vnamespace, parts, SL("namespace"), PH_NOISY);

			//missing
			if (!()) {

				zephir_update_property_this(this_ptr, SL("_namespace"), vnamespace TSRMLS_CC);

			}
			//missing unset
		} else {

			//missing

			zephir_update_property_this(this_ptr, SL("_namespace"), defaultNamespace TSRMLS_CC);

		}
		//missing comment
		if (phalcon_array_isset_string(parts, SS("module")) {

			ZEPHIR_OBS_VAR(module);
			zephir_array_fetch_string(&module, parts, SL("module"), PH_NOISY);

			//missing
			if (!()) {

				zephir_update_property_this(this_ptr, SL("_module"), module TSRMLS_CC);

			}
			//missing unset
		} else {

			//missing

			zephir_update_property_this(this_ptr, SL("_module"), defaultModule TSRMLS_CC);

		}
		//missing comment
		if (phalcon_array_isset_string(parts, SS("controller")) {

			ZEPHIR_OBS_VAR(controller);
			zephir_array_fetch_string(&controller, parts, SL("controller"), PH_NOISY);

			//missing
			if (!()) {

				zephir_update_property_this(this_ptr, SL("_controller"), controller TSRMLS_CC);

			}
			//missing unset
		} else {

			//missing

			zephir_update_property_this(this_ptr, SL("_controller"), defaultController TSRMLS_CC);

		}
		//missing comment
		if (phalcon_array_isset_string(parts, SS("action")) {

			ZEPHIR_OBS_VAR(action);
			zephir_array_fetch_string(&action, parts, SL("action"), PH_NOISY);

			//missing
			if (!()) {

				zephir_update_property_this(this_ptr, SL("_action"), action TSRMLS_CC);

			}
			//missing unset
		} else {

			//missing

			zephir_update_property_this(this_ptr, SL("_action"), defaultAction TSRMLS_CC);

		}
		//missing comment
		if (phalcon_array_isset_string(parts, SS("params")) {

			ZEPHIR_OBS_VAR(paramsStr);
			zephir_array_fetch_string(&paramsStr, parts, SL("params"), PH_NOISY);

			//missing

			if (zend_is_true(strParams)) {

				//missing

			}
			//missing unset
		}
		//missing
		if () {

			//missing

		} else {

			ZEPHIR_CPY_WRT(paramsMerge, parts);

		}

		zephir_update_property_this(this_ptr, SL("_params"), paramsMerge TSRMLS_CC);

	} else {
		//missing comment

		//missing

		zephir_update_property_this(this_ptr, SL("_namespace"), defaultNamespace TSRMLS_CC);

		//missing

		zephir_update_property_this(this_ptr, SL("_module"), defaultModule TSRMLS_CC);

		//missing

		zephir_update_property_this(this_ptr, SL("_controller"), defaultController TSRMLS_CC);

		//missing

		zephir_update_property_this(this_ptr, SL("_action"), defaultAction TSRMLS_CC);

		//missing

		zephir_update_property_this(this_ptr, SL("_params"), defaultParams TSRMLS_CC);

	}
	ZEPHIR_MM_RESTORE();

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
PHP_METHOD(Test_Router, add) {

	zval *pattern, *paths, *httpMethods, *route;


	zephir_fetch_params(0, 3, 0, &pattern, &paths, &httpMethods);

	//missing comment

	object_init_ex(route, test_router_route_ce);
	zephir_call_method_p3_noret(route, "__construct", pattern, paths, httpMethods);

	//missing

	RETURN_CCTORW(route);


}

/**
 * Adds a route to the router that only match if the HTTP method is GET
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addGet) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "GET", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Adds a route to the router that only match if the HTTP method is POST
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPost) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "POST", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Adds a route to the router that only match if the HTTP method is PUT
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPut) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "PUT", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Adds a route to the router that only match if the HTTP method is PATCH
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPatch) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "PATCH", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Adds a route to the router that only match if the HTTP method is DELETE
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addDelete) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "DELETE", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Add a route to the router that only match if the HTTP method is OPTIONS
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addOptions) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "OPTIONS", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Adds a route to the router that only match if the HTTP method is HEAD
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addHead) {

	zval *pattern, *paths, *method;

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &pattern, &paths);

	ZEPHIR_INIT_VAR(method);
	ZVAL_STRING(method, "HEAD", 1);

	//missing mcall
	ZEPHIR_MM_RESTORE();

}

/**
 * Mounts a group of routes in the router
 *
 * @param Test\Router\Group route
 * @return Test\Router
 */
PHP_METHOD(Test_Router, mount) {

	zval *group, *groupRoutes, *beforeMatch, *hostname, *routes, *newRoutes;


	zephir_fetch_params(0, 1, 0, &group);

	if (Z_TYPE_P(group) != IS_OBJECT) {
		//missing throw
	}

	//missing

	//missing
	if (!()) {
		//missing throw
	}
	//missing comment

	//missing

	if (Z_TYPE_P(beforeMatch) == IS_NULL) {
		//missing for
	}
	//missing comment

	//missing

	if (Z_TYPE_P(hostname) == IS_NULL) {
		//missing for
	}

	//missing

	if (Z_TYPE_P(routes) == IS_ARRAY) {

		//missing

		zephir_update_property_this(this_ptr, SL("_routes"), newRoutes TSRMLS_CC);

	} else {

		zephir_update_property_this(this_ptr, SL("_routes"), groupRoutes TSRMLS_CC);

	}

	RETURN_THISW(this);


}

/**
 * Set a group of paths to be returned when none of the defined routes are matched
 *
 * @param array paths
 * @return Test\Router
 */
PHP_METHOD(Test_Router, notFound) {

	zval *paths;


	zephir_fetch_params(0, 1, 0, &paths);

	if (Z_TYPE_P(paths) != IS_ARRAY) {
		if (Z_TYPE_P(paths) != IS_STRING) {
			//missing throw
		}
	}

	zephir_update_property_this(this_ptr, SL("_notFoundPaths"), paths TSRMLS_CC);

	RETURN_THISW(this);


}

/**
 * Removes all the pre-defined routes
 */
PHP_METHOD(Test_Router, clear) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_zval(this, SL("_routes"), _0 TSRMLS_CC);

	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the processed namespace name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getNamespaceName) {

	RETURN_MEMBER("_namespace");

}

/**
 * Returns the processed module name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getModuleName) {

	RETURN_MEMBER("_module");

}

/**
 * Returns the processed controller name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getControllerName) {

	RETURN_MEMBER("_controller");

}

/**
 * Returns the processed action name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getActionName) {

	RETURN_MEMBER("_action");

}

/**
 * Returns the processed parameters
 *
 * @return array
 */
PHP_METHOD(Test_Router, getParams) {

	RETURN_MEMBER("_params");

}

/**
 * Returns the route that matchs the handled URI
 *
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getMatchedRoute) {

	RETURN_MEMBER("_matchedRoute");

}

/**
 * Returns the sub expressions in the regular expression matched
 *
 * @return array
 */
PHP_METHOD(Test_Router, getMatches) {

	RETURN_MEMBER("_matches");

}

/**
 * Checks if the router macthes any of the defined routes
 *
 * @return bool
 */
PHP_METHOD(Test_Router, wasMatched) {

	RETURN_MEMBER("_wasMatched");

}

/**
 * Returns all the routes defined in the router
 *
 * @return Test\Router\Route[]
 */
PHP_METHOD(Test_Router, getRoutes) {

	RETURN_MEMBER("_routes");

}

/**
 * Returns a route object by its id
 *
 * @param string id
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteById) {

	zval *id, *routes;


	zephir_fetch_params(0, 1, 0, &id);

	//missing

	//missing for

	RETURN_BOOL(0);


}

/**
 * Returns a route object by its name
 *
 * @param string name
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteByName) {

	zval *name, *routes;


	zephir_fetch_params(0, 1, 0, &name);

	//missing

	//missing for

	RETURN_BOOL(0);


}

