
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


TEST_INIT_CLASS(Test_Router) {

	TEST_REGISTER_CLASS(Test, Router, router, test_router_method_entry, 0);

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
	zend_declare_property_null(test_router_ce, SL("_wasMatched"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultController"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_removeExtraSlashes"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_notFoundPaths"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Router, __construct) {

test_fetch_params(&defaultRoutes);

}

PHP_METHOD(Test_Router, setDI) {

test_fetch_params(&dependencyInjector);

}

PHP_METHOD(Test_Router, getDI) {

}

PHP_METHOD(Test_Router, add) {

test_fetch_params(&pattern, &paths, &httpMethods);

	zval *method;
	if (ZEPHIR_IS_LONG(method, 1)) {
		zval_print(method);
	}
}

