
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
#include "kernel/object.h"


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
 * Phalcon\Mvc\Router constructor
 *
 * @param boolean defaultRoutes
 */
PHP_METHOD(Test_Router, __construct) {

	zval *defaultRoutes, *routes, *paths, *actionPattern, *route, *paramsPattern, *params;

	zephir_fetch_params(1, 1, 0, &defaultRoutes);

	ZEPHIR_INIT_VAR(routes);
	array_init(routes);

	if (ZEPHIR_IS_TRUE(defaultRoutes)) {

		ZEPHIR_INIT_VAR(paths);
		array_init(paths);
		add_assoc_long_ex(paths, SS("controller"), 1);

		ZEPHIR_INIT_VAR(actionPattern);
		ZVAL_STRING(actionPattern, "#^/([a-zA-Z0-9\\_\\-]+)[/]{0,1}$#", 1);

		ZEPHIR_INIT_VAR(route);
		object_init_ex(route, test_router_route_ce);
		zephir_call_method_p2_noret(route, "__construct", actionPattern, paths);

		ZEPHIR_INIT_NVAR(routes);
		zephir_array_append(&routes, route, PH_SEPARATE);

		ZEPHIR_INIT_NVAR(paths);
		array_init(paths);
		add_assoc_long_ex(paths, SS("controller"), 1);
		add_assoc_long_ex(paths, SS("action"), 1);
		add_assoc_long_ex(paths, SS("params"), 1);

		ZEPHIR_INIT_VAR(paramsPattern);
		ZVAL_STRING(paramsPattern, "#^/([a-zA-Z0-9\\_\\-]+)/([a-zA-Z0-9\\.\\_]+)(/.*)*$#", 1);

		ZEPHIR_INIT_NVAR(route);
		object_init_ex(route, test_router_route_ce);
		zephir_call_method_p2_noret(route, "__construct", paramsPattern, paths);

		ZEPHIR_INIT_NVAR(routes);
		zephir_array_append(&routes, route, PH_SEPARATE);

	}

	ZEPHIR_INIT_VAR(params);
	array_init(params);

	phalcon_update_property_this(this_ptr, SL("_params"), params TSRMLS_CC);

	phalcon_update_property_this(this_ptr, SL("_routes"), routes TSRMLS_CC);

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

	phalcon_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);


}

/*
 * Returns the internal dependency injector
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Test_Router, getDI) {


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
PHP_METHOD(Test_Router, add) {

	zval *pattern, *paths, *httpMethods;

	zephir_fetch_params(0, 3, 0, &pattern, &paths, &httpMethods);


}

