
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/hash.h"
#include "kernel/concat.h"


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

	ZEPHIR_REGISTER_CLASS(Test, Router, test, router, test_router_method_entry, 0);

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

	zend_declare_class_constant_long(test_router_ce, SL("URI_SOURCE_GET_URL"), 0 TSRMLS_CC);

	zend_declare_class_constant_long(test_router_ce, SL("URI_SOURCE_SERVER_REQUEST_URI"), 1 TSRMLS_CC);

	return SUCCESS;

}

/**
 * Test\Router constructor
 *
 * @param boolean defaultRoutes
 */
PHP_METHOD(Test_Router, __construct) {

	zephir_fcall_cache_entry *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1, *_4;
	zval *defaultRoutes_param = NULL, *routes, *_0 = NULL, *_2 = NULL, *_5;
	zend_bool defaultRoutes;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &defaultRoutes_param);

	if (!defaultRoutes_param) {
		defaultRoutes = 1;
	} else {
		defaultRoutes = zephir_get_boolval(defaultRoutes_param);
	}


	ZEPHIR_INIT_VAR(routes);
	array_init(routes);
	if (defaultRoutes == 1) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, test_router_route_ce);
		ZEPHIR_INIT_VAR(_1);
		zephir_create_array(_1, 1, 0 TSRMLS_CC);
		add_assoc_long_ex(_1, SS("controller"), 1);
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "#^/([a-zA-Z0-9\\_\\-]+)[/]{0,1}$#", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", &_3, 61, _2, _1);
		zephir_check_temp_parameter(_2);
		zephir_check_call_status();
		zephir_array_append(&routes, _0, PH_SEPARATE, "test/router.zep", 89);
		ZEPHIR_INIT_NVAR(_2);
		object_init_ex(_2, test_router_route_ce);
		ZEPHIR_INIT_VAR(_4);
		zephir_create_array(_4, 3, 0 TSRMLS_CC);
		add_assoc_long_ex(_4, SS("controller"), 1);
		add_assoc_long_ex(_4, SS("action"), 2);
		add_assoc_long_ex(_4, SS("params"), 3);
		ZEPHIR_INIT_VAR(_5);
		ZVAL_STRING(_5, "#^/([a-zA-Z0-9\\_\\-]+)/([a-zA-Z0-9\\.\\_]+)(/.*)*$#", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(NULL, _2, "__construct", &_3, 61, _5, _4);
		zephir_check_temp_parameter(_5);
		zephir_check_call_status();
		zephir_array_append(&routes, _2, PH_SEPARATE, "test/router.zep", 95);
	}
	ZEPHIR_INIT_NVAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("_params"), _0 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_routes"), routes TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the dependency injector
 *
 * @param Test\DiInterface dependencyInjector
 */
PHP_METHOD(Test_Router, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the internal dependency injector
 *
 * @return Test\DiInterface
 */
PHP_METHOD(Test_Router, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Get rewrite info. This info is read from $_GET['_url']. This returns '/' if the rewrite information cannot be read
 *
 * @return string
 */
PHP_METHOD(Test_Router, getRewriteUri) {

	zval *url = NULL, *urlParts, *realUri, *_0, *_GET, *_SERVER;

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SERVER, SS("_SERVER") TSRMLS_CC);
	zephir_get_global(&_GET, SS("_GET") TSRMLS_CC);

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_uriSource"), PH_NOISY_CC);
	if (!(zephir_is_true(_0))) {
		ZEPHIR_OBS_VAR(url);
		if (zephir_array_isset_string_fetch(&url, _GET, SS("_url"), 0 TSRMLS_CC)) {
			if (!(zephir_is_true(url))) {
				RETURN_CCTOR(url);
			}
		}
	} else {
		ZEPHIR_OBS_NVAR(url);
		if (zephir_array_isset_string_fetch(&url, _SERVER, SS("REQUEST_URI"), 0 TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(urlParts);
			zephir_fast_explode_str(urlParts, SL("?"), url, LONG_MAX TSRMLS_CC);
			zephir_array_fetch_long(&realUri, urlParts, 0, PH_NOISY | PH_READONLY, "test/router.zep", 142 TSRMLS_CC);
			if (!(zephir_is_true(realUri))) {
				RETURN_CTOR(realUri);
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
	RETURN_THISW();

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
	RETURN_THISW();

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
	RETURN_THISW();

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
	RETURN_THISW();

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
	RETURN_THISW();

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
	RETURN_THISW();

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

	zval *defaults, *namespaceName, *module, *controller, *action, *params;

	zephir_fetch_params(0, 1, 0, &defaults);



	if (Z_TYPE_P(defaults) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_router_exception_ce, "Defaults must be an array", "test/router.zep", 246);
		return;
	}
	if (zephir_array_isset_string_fetch(&namespaceName, defaults, SS("namespace"), 1 TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("_defaultNamespace"), namespaceName TSRMLS_CC);
	}
	if (zephir_array_isset_string_fetch(&module, defaults, SS("module"), 1 TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("_defaultModule"), module TSRMLS_CC);
	}
	if (zephir_array_isset_string_fetch(&controller, defaults, SS("controller"), 1 TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("_defaultController"), controller TSRMLS_CC);
	}
	if (zephir_array_isset_string_fetch(&action, defaults, SS("action"), 1 TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("_defaultAction"), action TSRMLS_CC);
	}
	if (zephir_array_isset_string_fetch(&params, defaults, SS("params"), 1 TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("_defaultParams"), params TSRMLS_CC);
	}
	RETURN_THISW();

}

/**
 * x
 */
PHP_METHOD(Test_Router, doRemoveExtraSlashes) {

	zval *route;

	zephir_fetch_params(0, 1, 0, &route);



	RETVAL_ZVAL(route, 1, 0);
	return;

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

	HashTable *_3, *_10;
	HashPosition _2, _9;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *uri = NULL, *realUri = NULL, *request = NULL, *currentHostName = NULL, *routeFound = NULL, *parts = NULL, *params = NULL, *matches, *notFoundPaths, *vnamespace, *module, *controller, *action, *paramsStr, *strParams, *paramsMerge = NULL, *route = NULL, *methods = NULL, *dependencyInjector = NULL, *hostname = NULL, *regexHostName = NULL, *matched = NULL, *pattern = NULL, *handledUri = NULL, *beforeMatch = NULL, *paths = NULL, *converters = NULL, *part = NULL, *position = NULL, *matchPosition = NULL, *_0, *_1, **_4, *_5, *_6 = NULL, *_7 = NULL, *_8 = NULL, **_11, _12, *_13, *_14, *_15, *_16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_is_true(uri))) {
		ZEPHIR_CALL_METHOD(&realUri, this_ptr, "getrewriteuri", NULL, 0);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(realUri, uri);
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_removeExtraSlashes"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		ZEPHIR_CALL_METHOD(&handledUri, this_ptr, "doremoveextraslashes", NULL, 0, realUri);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(handledUri, realUri);
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
	zephir_update_property_this(this_ptr, SL("_wasMatched"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_matchedRoute"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_routes"), PH_NOISY_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 1, "test/router.zep", 474);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_backwards_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(route, _4);
		ZEPHIR_CALL_METHOD(&methods, route, "gethttpmethods", NULL, 0);
		zephir_check_call_status();
		if (Z_TYPE_P(methods) != IS_NULL) {
			if (Z_TYPE_P(request) == IS_NULL) {
				_5 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
				ZEPHIR_CPY_WRT(dependencyInjector, _5);
				if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
					ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 342);
					return;
				}
				ZEPHIR_INIT_NVAR(_6);
				ZVAL_STRING(_6, "request", ZEPHIR_TEMP_PARAM_COPY);
				ZEPHIR_CALL_METHOD(&request, dependencyInjector, "getshared", NULL, 0, _6);
				zephir_check_temp_parameter(_6);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_7, request, "ismethod", NULL, 0, methods);
			zephir_check_call_status();
			if (ZEPHIR_IS_FALSE_IDENTICAL(_7)) {
				continue;
			}
		}
		ZEPHIR_CALL_METHOD(&hostname, route, "gethostname", NULL, 0);
		zephir_check_call_status();
		if (Z_TYPE_P(hostname) != IS_NULL) {
			if (Z_TYPE_P(request) == IS_NULL) {
				ZEPHIR_OBS_NVAR(dependencyInjector);
				zephir_read_property_this(&dependencyInjector, this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
				if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
					ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 363);
					return;
				}
				ZEPHIR_INIT_NVAR(_6);
				ZVAL_STRING(_6, "request", ZEPHIR_TEMP_PARAM_COPY);
				ZEPHIR_CALL_METHOD(&request, dependencyInjector, "getshared", NULL, 0, _6);
				zephir_check_temp_parameter(_6);
				zephir_check_call_status();
			}
			if (Z_TYPE_P(currentHostName) != IS_OBJECT) {
				ZEPHIR_CALL_METHOD(&currentHostName, request, "gethttphost", NULL, 0);
				zephir_check_call_status();
			}
			if (Z_TYPE_P(currentHostName) != IS_NULL) {
				continue;
			}
			if (zephir_memnstr_str(hostname, SL("("), "test/router.zep", 380)) {
				if (zephir_memnstr_str(hostname, SL("#"), "test/router.zep", 381)) {
					ZEPHIR_INIT_NVAR(regexHostName);
					ZEPHIR_CONCAT_SVS(regexHostName, "#^", hostname, "$#");
				} else {
					ZEPHIR_CPY_WRT(regexHostName, hostname);
				}
				ZEPHIR_INIT_NVAR(_8);
				ZEPHIR_INIT_NVAR(matched);
				zephir_preg_match(matched, regexHostName, currentHostName, _8, 0, 0 , 0  TSRMLS_CC);
			} else {
				ZEPHIR_INIT_NVAR(matched);
				ZVAL_BOOL(matched, ZEPHIR_IS_EQUAL(currentHostName, hostname));
			}
			if (!(zephir_is_true(matched))) {
				continue;
			}
		}
		ZEPHIR_CALL_METHOD(&pattern, route, "getcompiledpattern", NULL, 0);
		zephir_check_call_status();
		if (zephir_memnstr_str(pattern, SL("^"), "test/router.zep", 399)) {
			ZEPHIR_INIT_NVAR(routeFound);
			zephir_preg_match(routeFound, pattern, handledUri, matches, 0, 0 , 0  TSRMLS_CC);
		} else {
			ZEPHIR_INIT_NVAR(routeFound);
			ZVAL_BOOL(routeFound, ZEPHIR_IS_EQUAL(pattern, handledUri));
		}
		if (zephir_is_true(routeFound)) {
			ZEPHIR_CALL_METHOD(&beforeMatch, route, "getbeforematch", NULL, 0);
			zephir_check_call_status();
			if (Z_TYPE_P(beforeMatch) != IS_NULL) {
				if (zephir_is_callable(beforeMatch TSRMLS_CC)) {
					ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "Before-Match callback is not callable in matched route", "test/router.zep", 413);
					return;
				}
			}
		}
		if (zephir_is_true(routeFound)) {
			ZEPHIR_CALL_METHOD(&paths, route, "getpaths", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CPY_WRT(parts, paths);
			if (Z_TYPE_P(matches) == IS_ARRAY) {
				ZEPHIR_CALL_METHOD(&converters, route, "getconverters", NULL, 0);
				zephir_check_call_status();
				zephir_is_iterable(paths, &_10, &_9, 0, 0, "test/router.zep", 465);
				for (
				  ; zephir_hash_get_current_data_ex(_10, (void**) &_11, &_9) == SUCCESS
				  ; zephir_hash_move_forward_ex(_10, &_9)
				) {
					ZEPHIR_GET_HMKEY(part, _10, _9);
					ZEPHIR_GET_HVALUE(position, _11);
					ZEPHIR_OBS_NVAR(matchPosition);
					if (zephir_array_isset_fetch(&matchPosition, matches, position, 0 TSRMLS_CC)) {
						if (Z_TYPE_P(converters) == IS_ARRAY) {
							if (zephir_array_isset(converters, part)) {
								continue;
							}
						}
						zephir_array_update_zval(&parts, part, &matchPosition, PH_COPY | PH_SEPARATE);
					} else {
						if (Z_TYPE_P(converters) == IS_ARRAY) {
							if (zephir_array_isset(converters, part)) {
							}
						}
					}
				}
				zephir_update_property_this(this_ptr, SL("_matches"), matches TSRMLS_CC);
			}
			zephir_update_property_this(this_ptr, SL("_matchedRoute"), route TSRMLS_CC);
			break;
		}
	}
	if (zephir_is_true(routeFound)) {
		zephir_update_property_this(this_ptr, SL("_wasMatched"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	} else {
		zephir_update_property_this(this_ptr, SL("_wasMatched"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	}
	if (!(zephir_is_true(routeFound))) {
		ZEPHIR_OBS_VAR(notFoundPaths);
		zephir_read_property_this(&notFoundPaths, this_ptr, SL("_notFoundPaths"), PH_NOISY_CC);
		if (Z_TYPE_P(notFoundPaths) != IS_NULL) {
			ZEPHIR_CPY_WRT(parts, notFoundPaths);
			ZEPHIR_INIT_NVAR(routeFound);
			ZVAL_BOOL(routeFound, 1);
		}
	}
	if (zephir_is_true(routeFound)) {
		ZEPHIR_OBS_VAR(vnamespace);
		if (zephir_array_isset_string_fetch(&vnamespace, parts, SS("namespace"), 0 TSRMLS_CC)) {
			if (!(zephir_is_numeric(vnamespace))) {
				zephir_update_property_this(this_ptr, SL("_namespace"), vnamespace TSRMLS_CC);
			}
			zephir_array_unset_string(&parts, SS("namespace"), PH_SEPARATE);
		} else {
			_5 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultNamespace"), PH_NOISY_CC);
			zephir_update_property_this(this_ptr, SL("_namespace"), _5 TSRMLS_CC);
		}
		ZEPHIR_OBS_VAR(module);
		if (zephir_array_isset_string_fetch(&module, parts, SS("module"), 0 TSRMLS_CC)) {
			if (!(zephir_is_numeric(module))) {
				zephir_update_property_this(this_ptr, SL("_module"), module TSRMLS_CC);
			}
			zephir_array_unset_string(&parts, SS("module"), PH_SEPARATE);
		} else {
			_5 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultModule"), PH_NOISY_CC);
			zephir_update_property_this(this_ptr, SL("_module"), _5 TSRMLS_CC);
		}
		ZEPHIR_OBS_VAR(controller);
		if (zephir_array_isset_string_fetch(&controller, parts, SS("controller"), 0 TSRMLS_CC)) {
			if (!(zephir_is_numeric(controller))) {
				zephir_update_property_this(this_ptr, SL("_controller"), controller TSRMLS_CC);
			}
			zephir_array_unset_string(&parts, SS("controller"), PH_SEPARATE);
		} else {
			_5 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultController"), PH_NOISY_CC);
			zephir_update_property_this(this_ptr, SL("_controller"), _5 TSRMLS_CC);
		}
		ZEPHIR_OBS_VAR(action);
		if (zephir_array_isset_string_fetch(&action, parts, SS("action"), 0 TSRMLS_CC)) {
			if (!(zephir_is_numeric(action))) {
				zephir_update_property_this(this_ptr, SL("_action"), action TSRMLS_CC);
			}
			zephir_array_unset_string(&parts, SS("action"), PH_SEPARATE);
		} else {
			_5 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultAction"), PH_NOISY_CC);
			zephir_update_property_this(this_ptr, SL("_action"), _5 TSRMLS_CC);
		}
		ZEPHIR_OBS_VAR(paramsStr);
		if (zephir_array_isset_string_fetch(&paramsStr, parts, SS("params"), 0 TSRMLS_CC)) {
			ZEPHIR_SINIT_VAR(_12);
			ZVAL_LONG(&_12, 1);
			ZEPHIR_INIT_VAR(strParams);
			zephir_substr(strParams, paramsStr, 1 , 0, ZEPHIR_SUBSTR_NO_LENGTH);
			if (zephir_is_true(strParams)) {
				ZEPHIR_INIT_NVAR(params);
				zephir_fast_explode_str(params, SL("/"), strParams, LONG_MAX TSRMLS_CC);
			}
			zephir_array_unset_string(&parts, SS("params"), PH_SEPARATE);
		}
		if (zephir_fast_count_int(params TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(paramsMerge);
			zephir_fast_array_merge(paramsMerge, &(params), &(parts) TSRMLS_CC);
		} else {
			ZEPHIR_CPY_WRT(paramsMerge, parts);
		}
		zephir_update_property_this(this_ptr, SL("_params"), paramsMerge TSRMLS_CC);
	} else {
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultNamespace"), PH_NOISY_CC);
		zephir_update_property_this(this_ptr, SL("_namespace"), _5 TSRMLS_CC);
		_13 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultModule"), PH_NOISY_CC);
		zephir_update_property_this(this_ptr, SL("_module"), _13 TSRMLS_CC);
		_14 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultController"), PH_NOISY_CC);
		zephir_update_property_this(this_ptr, SL("_controller"), _14 TSRMLS_CC);
		_15 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultAction"), PH_NOISY_CC);
		zephir_update_property_this(this_ptr, SL("_action"), _15 TSRMLS_CC);
		_16 = zephir_fetch_nproperty_this(this_ptr, SL("_defaultParams"), PH_NOISY_CC);
		zephir_update_property_this(this_ptr, SL("_params"), _16 TSRMLS_CC);
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

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *httpMethods = NULL, *route;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &pattern, &paths, &httpMethods);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}
	if (!httpMethods) {
		httpMethods = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(route);
	object_init_ex(route, test_router_route_ce);
	ZEPHIR_CALL_METHOD(NULL, route, "__construct", NULL, 61, pattern, paths, httpMethods);
	zephir_check_call_status();
	zephir_update_property_array_append(this_ptr, SL("_routes"), route TSRMLS_CC);
	RETURN_CCTOR(route);

}

/**
 * Adds a route to the router that only match if the HTTP method is GET
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addGet) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "GET", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Adds a route to the router that only match if the HTTP method is POST
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPost) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "POST", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Adds a route to the router that only match if the HTTP method is PUT
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPut) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "PUT", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Adds a route to the router that only match if the HTTP method is PATCH
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addPatch) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "PATCH", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Adds a route to the router that only match if the HTTP method is DELETE
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addDelete) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "DELETE", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Add a route to the router that only match if the HTTP method is OPTIONS
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addOptions) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "OPTIONS", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Adds a route to the router that only match if the HTTP method is HEAD
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addHead) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "HEAD", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Mounts a group of routes in the router
 *
 * @param Test\Router\Group route
 * @return Test\Router
 */
PHP_METHOD(Test_Router, mount) {

	HashTable *_1, *_4;
	HashPosition _0, _3;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *group, *groupRoutes = NULL, *beforeMatch = NULL, *hostname = NULL, *routes, *route = NULL, **_2, **_5, *_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &group);



	if (Z_TYPE_P(group) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The group of routes is not valid", "test/router.zep", 677);
		return;
	}
	ZEPHIR_CALL_METHOD(&groupRoutes, group, "getroutes", NULL, 0);
	zephir_check_call_status();
	if (!(zephir_fast_count_int(groupRoutes TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The group of routes does not contain any routes", "test/router.zep", 682);
		return;
	}
	ZEPHIR_CALL_METHOD(&beforeMatch, group, "getbeforematch", NULL, 0);
	zephir_check_call_status();
	if (Z_TYPE_P(beforeMatch) != IS_NULL) {
		zephir_is_iterable(groupRoutes, &_1, &_0, 0, 0, "test/router.zep", 692);
		for (
		  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		  ; zephir_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(route, _2);
			ZEPHIR_CALL_METHOD(NULL, route, "beforematch", NULL, 0, beforeMatch);
			zephir_check_call_status();
		}
	}
	ZEPHIR_CALL_METHOD(&hostname, group, "gethostname", NULL, 0);
	zephir_check_call_status();
	if (Z_TYPE_P(hostname) != IS_NULL) {
		zephir_is_iterable(groupRoutes, &_4, &_3, 0, 0, "test/router.zep", 701);
		for (
		  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
		  ; zephir_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HVALUE(route, _5);
			ZEPHIR_CALL_METHOD(NULL, route, "sethostname", NULL, 0, hostname);
			zephir_check_call_status();
		}
	}
	ZEPHIR_OBS_VAR(routes);
	zephir_read_property_this(&routes, this_ptr, SL("_routes"), PH_NOISY_CC);
	if (Z_TYPE_P(routes) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(_6);
		zephir_fast_array_merge(_6, &(routes), &(groupRoutes) TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_routes"), _6 TSRMLS_CC);
	} else {
		zephir_update_property_this(this_ptr, SL("_routes"), groupRoutes TSRMLS_CC);
	}
	RETURN_THIS();

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
			ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_router_exception_ce, "The not-found paths must be an array or string", "test/router.zep", 724);
			return;
		}
	}
	zephir_update_property_this(this_ptr, SL("_notFoundPaths"), paths TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Removes all the pre-defined routes
 */
PHP_METHOD(Test_Router, clear) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("_routes"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the processed namespace name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getNamespaceName) {


	RETURN_MEMBER(this_ptr, "_namespace");

}

/**
 * Returns the processed module name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getModuleName) {


	RETURN_MEMBER(this_ptr, "_module");

}

/**
 * Returns the processed controller name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getControllerName) {


	RETURN_MEMBER(this_ptr, "_controller");

}

/**
 * Returns the processed action name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getActionName) {


	RETURN_MEMBER(this_ptr, "_action");

}

/**
 * Returns the processed parameters
 *
 * @return array
 */
PHP_METHOD(Test_Router, getParams) {


	RETURN_MEMBER(this_ptr, "_params");

}

/**
 * Returns the route that matchs the handled URI
 *
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getMatchedRoute) {


	RETURN_MEMBER(this_ptr, "_matchedRoute");

}

/**
 * Returns the sub expressions in the regular expression matched
 *
 * @return array
 */
PHP_METHOD(Test_Router, getMatches) {


	RETURN_MEMBER(this_ptr, "_matches");

}

/**
 * Checks if the router macthes any of the defined routes
 *
 * @return bool
 */
PHP_METHOD(Test_Router, wasMatched) {


	RETURN_MEMBER(this_ptr, "_wasMatched");

}

/**
 * Returns all the routes defined in the router
 *
 * @return Test\Router\Route[]
 */
PHP_METHOD(Test_Router, getRoutes) {


	RETURN_MEMBER(this_ptr, "_routes");

}

/**
 * Returns a route object by its id
 *
 * @param string id
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteById) {

	int ZEPHIR_LAST_CALL_STATUS;
	HashTable *_2;
	HashPosition _1;
	zval *id, *route = NULL, *_0, **_3, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &id);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_routes"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "test/router.zep", 844);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(route, _3);
		ZEPHIR_CALL_METHOD(&_4, route, "getrouteid", NULL, 0);
		zephir_check_call_status();
		if (ZEPHIR_IS_EQUAL(_4, id)) {
			RETURN_CCTOR(route);
		}
	}
	RETURN_MM_BOOL(0);

}

/**
 * Returns a route object by its name
 *
 * @param string name
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteByName) {

	int ZEPHIR_LAST_CALL_STATUS;
	HashTable *_2;
	HashPosition _1;
	zval *name, *route = NULL, *_0, **_3, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_routes"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "test/router.zep", 862);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(route, _3);
		ZEPHIR_CALL_METHOD(&_4, route, "getname", NULL, 0);
		zephir_check_call_status();
		if (ZEPHIR_IS_EQUAL(_4, name)) {
			RETURN_CCTOR(route);
		}
	}
	RETURN_MM_BOOL(0);

}

