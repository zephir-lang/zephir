
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


/**
 * Test\Router\Route
 *
 * This class represents every route added to the router
 */
ZEPHIR_INIT_CLASS(Test_Router_Route) {

	ZEPHIR_REGISTER_CLASS(Test_Router, Route, route, test_router_route_method_entry, 0);

	zend_declare_property_null(test_router_route_ce, SL("_pattern"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_compiledPattern"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_paths"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_methods"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_hostname"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_converters"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_id"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_beforeMatch"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

/**
 * Phalcon\Mvc\Router\Route constructor
 *
 * @param string pattern
 * @param array paths
 * @param array|string httpMethods
 */
PHP_METHOD(Test_Router_Route, __construct) {

	zval *pattern, *paths, *httpMethods;


	zephir_fetch_params(0, 3, 0, &pattern, &paths, &httpMethods);


}

