
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

	zend_declare_property_null(test_router_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_uriSource"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_namespace"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_module"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_controller"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_action"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_params"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_routes"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_matchedRoute"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_matches"), ZEND_ACC_PROTECTED);

	zend_declare_property_bool(test_router_ce, SL("_wasMatched"), 0, ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_defaultController"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_removeExtraSlashes"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_router_ce, SL("_notFoundPaths"), ZEND_ACC_PROTECTED);

	zephir_declare_class_constant_long(test_router_ce, SL("URI_SOURCE_GET_URL"), 0);

	zephir_declare_class_constant_long(test_router_ce, SL("URI_SOURCE_SERVER_REQUEST_URI"), 1);

	return SUCCESS;

}

/**
 * Test\Router constructor
 *
 * @param boolean defaultRoutes
 */
PHP_METHOD(Test_Router, __construct) {

	zval _1$$3, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_3 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *defaultRoutes_param = NULL, routes, _6, _0$$3, _2$$3, _5$$3;
	zend_bool defaultRoutes;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&routes);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_4$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &defaultRoutes_param);

	if (!defaultRoutes_param) {
		defaultRoutes = 1;
	} else {
		defaultRoutes = zephir_get_boolval(defaultRoutes_param);
	}


	ZEPHIR_INIT_VAR(&routes);
	array_init(&routes);
	if (defaultRoutes == 1) {
		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, test_router_route_ce);
		ZEPHIR_INIT_VAR(&_1$$3);
		zephir_create_array(&_1$$3, 1, 0);
		add_assoc_long_ex(&_1$$3, SL("controller"), 1);
		ZEPHIR_INIT_VAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "#^/([a-zA-Z0-9\\_\\-]+)[/]{0,1}$#");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", &_3, 83, &_2$$3, &_1$$3);
		zephir_check_call_status();
		zephir_array_append(&routes, &_0$$3, PH_SEPARATE, "test/router.zep", 89);
		ZEPHIR_INIT_NVAR(&_2$$3);
		object_init_ex(&_2$$3, test_router_route_ce);
		ZEPHIR_INIT_VAR(&_4$$3);
		zephir_create_array(&_4$$3, 3, 0);
		add_assoc_long_ex(&_4$$3, SL("controller"), 1);
		add_assoc_long_ex(&_4$$3, SL("action"), 2);
		add_assoc_long_ex(&_4$$3, SL("params"), 3);
		ZEPHIR_INIT_VAR(&_5$$3);
		ZVAL_STRING(&_5$$3, "#^/([a-zA-Z0-9\\_\\-]+)/([a-zA-Z0-9\\.\\_]+)(/.*)*$#");
		ZEPHIR_CALL_METHOD(NULL, &_2$$3, "__construct", &_3, 83, &_5$$3, &_4$$3);
		zephir_check_call_status();
		zephir_array_append(&routes, &_2$$3, PH_SEPARATE, "test/router.zep", 95);
	}
	ZEPHIR_INIT_VAR(&_6);
	array_init(&_6);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_params"), &_6);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_routes"), &routes);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the dependency injector
 *
 * @param Test\DiInterface dependencyInjector
 */
PHP_METHOD(Test_Router, setDI) {

	zval *dependencyInjector, dependencyInjector_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dependencyInjector_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &dependencyInjector);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_dependencyInjector"), dependencyInjector);

}

/**
 * Returns the internal dependency injector
 *
 * @return Test\DiInterface
 */
PHP_METHOD(Test_Router, getDI) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_dependencyInjector");

}

/**
 * Get rewrite info. This info is read from $_GET['_url']. This returns '/' if the rewrite information cannot be read
 *
 * @return string
 */
PHP_METHOD(Test_Router, getRewriteUri) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval _GET, _SERVER, url, urlParts, realUri, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&urlParts);
	ZVAL_UNDEF(&realUri);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SERVER, SL("_SERVER"));
	zephir_get_global(&_GET, SL("_GET"));

	zephir_read_property(&_0, this_ptr, SL("_uriSource"), PH_NOISY_CC | PH_READONLY);
	if (!(zephir_is_true(&_0))) {
		ZEPHIR_OBS_VAR(&url);
		if (zephir_array_isset_string_fetch(&url, &_GET, SL("_url"), 0)) {
			if (!(zephir_is_true(&url))) {
				RETURN_CCTOR(&url);
			}
		}
	} else {
		ZEPHIR_OBS_NVAR(&url);
		if (zephir_array_isset_string_fetch(&url, &_SERVER, SL("REQUEST_URI"), 0)) {
			ZEPHIR_INIT_VAR(&urlParts);
			zephir_fast_explode_str(&urlParts, SL("?"), &url, LONG_MAX);
			zephir_array_fetch_long(&realUri, &urlParts, 0, PH_NOISY | PH_READONLY, "test/router.zep", 142);
			if (!(zephir_is_true(&realUri))) {
				RETURN_CTOR(&realUri);
			}
		}
	}
	RETURN_MM_STRING("/");

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

	zval *uriSource, uriSource_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uriSource_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &uriSource);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_uriSource"), uriSource);
	RETURN_THISW();

}

/**
 * Set whether router must remove the extra slashes in the handled routes
 *
 * @param boolean remove
 * @return Test\Router
 */
PHP_METHOD(Test_Router, removeExtraSlashes) {

	zval *remove, remove_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&remove_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &remove);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_removeExtraSlashes"), remove);
	RETURN_THISW();

}

/**
 * Sets the name of the default namespace
 *
 * @param string namespaceName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultNamespace) {

	zval *namespaceName, namespaceName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&namespaceName_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &namespaceName);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultNamespace"), namespaceName);
	RETURN_THISW();

}

/**
 * Sets the name of the default module
 *
 * @param string moduleName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultModule) {

	zval *moduleName, moduleName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&moduleName_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &moduleName);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultModule"), moduleName);
	RETURN_THISW();

}

/**
 * Sets the default controller name
 *
 * @param string controllerName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultController) {

	zval *controllerName, controllerName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&controllerName_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &controllerName);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultController"), controllerName);
	RETURN_THISW();

}

/**
 * Sets the default action name
 *
 * @param string actionName
 * @return Test\Router
 */
PHP_METHOD(Test_Router, setDefaultAction) {

	zval *actionName, actionName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&actionName_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &actionName);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultAction"), actionName);
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

	zval *defaults, defaults_sub, namespaceName, module, controller, action, params;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&defaults_sub);
	ZVAL_UNDEF(&namespaceName);
	ZVAL_UNDEF(&module);
	ZVAL_UNDEF(&controller);
	ZVAL_UNDEF(&action);
	ZVAL_UNDEF(&params);

	zephir_fetch_params_without_memory_grow(1, 0, &defaults);



	if (Z_TYPE_P(defaults) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_router_exception_ce, "Defaults must be an array", "test/router.zep", 246);
		return;
	}
	if (zephir_array_isset_string_fetch(&namespaceName, defaults, SL("namespace"), 1)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultNamespace"), &namespaceName);
	}
	if (zephir_array_isset_string_fetch(&module, defaults, SL("module"), 1)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultModule"), &module);
	}
	if (zephir_array_isset_string_fetch(&controller, defaults, SL("controller"), 1)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultController"), &controller);
	}
	if (zephir_array_isset_string_fetch(&action, defaults, SL("action"), 1)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultAction"), &action);
	}
	if (zephir_array_isset_string_fetch(&params, defaults, SL("params"), 1)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_defaultParams"), &params);
	}
	RETURN_THISW();

}

/**
 * x
 */
PHP_METHOD(Test_Router, doRemoveExtraSlashes) {

	zval *route, route_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&route_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &route);



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

	zend_string *_12$$28, *_21$$62;
	zend_ulong _11$$28, _20$$62;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *uri = NULL, uri_sub, __$true, __$false, __$null, realUri, request, currentHostName, routeFound, parts, params, matches, notFoundPaths, vnamespace, module, controller, action, paramsStr, strParams, paramsMerge, route, methods, dependencyInjector, hostname, regexHostName, matched, pattern, handledUri, beforeMatch, paths, converters, part, position, matchPosition, _0, _1, *_2, _3, _4$$9, _5$$9, _6$$8, _7$$13, _8$$17, *_9$$28, _10$$28, _13$$43, _14$$43, _15$$42, _16$$47, _17$$51, *_18$$62, _19$$62, _22$$82, _23$$85, _24$$88, _25$$91, _26$$92, _27$$96, _28$$96, _29$$96, _30$$96, _31$$96;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uri_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&realUri);
	ZVAL_UNDEF(&request);
	ZVAL_UNDEF(&currentHostName);
	ZVAL_UNDEF(&routeFound);
	ZVAL_UNDEF(&parts);
	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&notFoundPaths);
	ZVAL_UNDEF(&vnamespace);
	ZVAL_UNDEF(&module);
	ZVAL_UNDEF(&controller);
	ZVAL_UNDEF(&action);
	ZVAL_UNDEF(&paramsStr);
	ZVAL_UNDEF(&strParams);
	ZVAL_UNDEF(&paramsMerge);
	ZVAL_UNDEF(&route);
	ZVAL_UNDEF(&methods);
	ZVAL_UNDEF(&dependencyInjector);
	ZVAL_UNDEF(&hostname);
	ZVAL_UNDEF(&regexHostName);
	ZVAL_UNDEF(&matched);
	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&handledUri);
	ZVAL_UNDEF(&beforeMatch);
	ZVAL_UNDEF(&paths);
	ZVAL_UNDEF(&converters);
	ZVAL_UNDEF(&part);
	ZVAL_UNDEF(&position);
	ZVAL_UNDEF(&matchPosition);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4$$9);
	ZVAL_UNDEF(&_5$$9);
	ZVAL_UNDEF(&_6$$8);
	ZVAL_UNDEF(&_7$$13);
	ZVAL_UNDEF(&_8$$17);
	ZVAL_UNDEF(&_10$$28);
	ZVAL_UNDEF(&_13$$43);
	ZVAL_UNDEF(&_14$$43);
	ZVAL_UNDEF(&_15$$42);
	ZVAL_UNDEF(&_16$$47);
	ZVAL_UNDEF(&_17$$51);
	ZVAL_UNDEF(&_19$$62);
	ZVAL_UNDEF(&_22$$82);
	ZVAL_UNDEF(&_23$$85);
	ZVAL_UNDEF(&_24$$88);
	ZVAL_UNDEF(&_25$$91);
	ZVAL_UNDEF(&_26$$92);
	ZVAL_UNDEF(&_27$$96);
	ZVAL_UNDEF(&_28$$96);
	ZVAL_UNDEF(&_29$$96);
	ZVAL_UNDEF(&_30$$96);
	ZVAL_UNDEF(&_31$$96);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = &uri_sub;
		uri = &__$null;
	}


	if (!(zephir_is_true(uri))) {
		ZEPHIR_CALL_METHOD(&realUri, this_ptr, "getrewriteuri", NULL, 0);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(&realUri, uri);
	}
	zephir_read_property(&_0, this_ptr, SL("_removeExtraSlashes"), PH_NOISY_CC | PH_READONLY);
	if (zephir_is_true(&_0)) {
		ZEPHIR_CALL_METHOD(&handledUri, this_ptr, "doremoveextraslashes", NULL, 0, &realUri);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(&handledUri, &realUri);
	}
	ZEPHIR_INIT_VAR(&request);
	ZVAL_NULL(&request);
	ZEPHIR_INIT_VAR(&currentHostName);
	ZVAL_NULL(&currentHostName);
	ZEPHIR_INIT_VAR(&routeFound);
	ZVAL_BOOL(&routeFound, 0);
	ZEPHIR_INIT_VAR(&parts);
	array_init(&parts);
	ZEPHIR_INIT_VAR(&params);
	array_init(&params);
	ZEPHIR_INIT_VAR(&matches);
	ZVAL_NULL(&matches);
	if (0) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$false);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("_matchedRoute"), &__$null);
	zephir_read_property(&_1, this_ptr, SL("_routes"), PH_NOISY_CC | PH_READONLY);
	zephir_is_iterable(&_1, 0, "test/router.zep", 474);
	if (Z_TYPE_P(&_1) == IS_ARRAY) {
		ZEND_HASH_REVERSE_FOREACH_VAL(Z_ARRVAL_P(&_1), _2)
		{
			ZEPHIR_INIT_NVAR(&route);
			ZVAL_COPY(&route, _2);
			ZEPHIR_CALL_METHOD(&methods, &route, "gethttpmethods", NULL, 0);
			zephir_check_call_status();
			if (Z_TYPE_P(&methods) != IS_NULL) {
				if (Z_TYPE_P(&request) == IS_NULL) {
					zephir_read_property(&_4$$9, this_ptr, SL("_dependencyInjector"), PH_NOISY_CC | PH_READONLY);
					ZEPHIR_CPY_WRT(&dependencyInjector, &_4$$9);
					if (Z_TYPE_P(&dependencyInjector) != IS_OBJECT) {
						ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 342);
						return;
					}
					ZEPHIR_INIT_NVAR(&_5$$9);
					ZVAL_STRING(&_5$$9, "request");
					ZEPHIR_CALL_METHOD(&request, &dependencyInjector, "getshared", NULL, 0, &_5$$9);
					zephir_check_call_status();
				}
				ZEPHIR_CALL_METHOD(&_6$$8, &request, "ismethod", NULL, 0, &methods);
				zephir_check_call_status();
				if (ZEPHIR_IS_FALSE_IDENTICAL(&_6$$8)) {
					continue;
				}
			}
			ZEPHIR_CALL_METHOD(&hostname, &route, "gethostname", NULL, 0);
			zephir_check_call_status();
			if (Z_TYPE_P(&hostname) != IS_NULL) {
				if (Z_TYPE_P(&request) == IS_NULL) {
					ZEPHIR_OBS_NVAR(&dependencyInjector);
					zephir_read_property(&dependencyInjector, this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
					if (Z_TYPE_P(&dependencyInjector) != IS_OBJECT) {
						ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 363);
						return;
					}
					ZEPHIR_INIT_NVAR(&_7$$13);
					ZVAL_STRING(&_7$$13, "request");
					ZEPHIR_CALL_METHOD(&request, &dependencyInjector, "getshared", NULL, 0, &_7$$13);
					zephir_check_call_status();
				}
				if (Z_TYPE_P(&currentHostName) != IS_OBJECT) {
					ZEPHIR_CALL_METHOD(&currentHostName, &request, "gethttphost", NULL, 0);
					zephir_check_call_status();
				}
				if (Z_TYPE_P(&currentHostName) != IS_NULL) {
					continue;
				}
				if (zephir_memnstr_str(&hostname, SL("("), "test/router.zep", 380)) {
					if (zephir_memnstr_str(&hostname, SL("#"), "test/router.zep", 381)) {
						ZEPHIR_INIT_NVAR(&regexHostName);
						ZEPHIR_CONCAT_SVS(&regexHostName, "#^", &hostname, "$#");
					} else {
						ZEPHIR_CPY_WRT(&regexHostName, &hostname);
					}
					ZEPHIR_INIT_NVAR(&_8$$17);
					ZEPHIR_INIT_NVAR(&matched);
					zephir_preg_match(&matched, &regexHostName, &currentHostName, &_8$$17, 0, 0 , 0 );
				} else {
					ZEPHIR_INIT_NVAR(&matched);
					ZVAL_BOOL(&matched, ZEPHIR_IS_EQUAL(&currentHostName, &hostname));
				}
				if (!(zephir_is_true(&matched))) {
					continue;
				}
			}
			ZEPHIR_CALL_METHOD(&pattern, &route, "getcompiledpattern", NULL, 0);
			zephir_check_call_status();
			if (zephir_memnstr_str(&pattern, SL("^"), "test/router.zep", 399)) {
				ZEPHIR_INIT_NVAR(&routeFound);
				zephir_preg_match(&routeFound, &pattern, &handledUri, &matches, 0, 0 , 0 );
			} else {
				ZEPHIR_INIT_NVAR(&routeFound);
				ZVAL_BOOL(&routeFound, ZEPHIR_IS_EQUAL(&pattern, &handledUri));
			}
			if (zephir_is_true(&routeFound)) {
				ZEPHIR_CALL_METHOD(&beforeMatch, &route, "getbeforematch", NULL, 0);
				zephir_check_call_status();
				if (Z_TYPE_P(&beforeMatch) != IS_NULL) {
					if (zephir_is_callable(&beforeMatch)) {
						ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "Before-Match callback is not callable in matched route", "test/router.zep", 413);
						return;
					}
				}
			}
			if (zephir_is_true(&routeFound)) {
				ZEPHIR_CALL_METHOD(&paths, &route, "getpaths", NULL, 0);
				zephir_check_call_status();
				ZEPHIR_CPY_WRT(&parts, &paths);
				if (Z_TYPE_P(&matches) == IS_ARRAY) {
					ZEPHIR_CALL_METHOD(&converters, &route, "getconverters", NULL, 0);
					zephir_check_call_status();
					zephir_is_iterable(&paths, 0, "test/router.zep", 465);
					if (Z_TYPE_P(&paths) == IS_ARRAY) {
						ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&paths), _11$$28, _12$$28, _9$$28)
						{
							ZEPHIR_INIT_NVAR(&part);
							if (_12$$28 != NULL) { 
								ZVAL_STR_COPY(&part, _12$$28);
							} else {
								ZVAL_LONG(&part, _11$$28);
							}
							ZEPHIR_INIT_NVAR(&position);
							ZVAL_COPY(&position, _9$$28);
							ZEPHIR_OBS_NVAR(&matchPosition);
							if (zephir_array_isset_fetch(&matchPosition, &matches, &position, 0)) {
								if (Z_TYPE_P(&converters) == IS_ARRAY) {
									if (zephir_array_isset(&converters, &part)) {
										continue;
									}
								}
								zephir_array_update_zval(&parts, &part, &matchPosition, PH_COPY | PH_SEPARATE);
							} else {
								if (Z_TYPE_P(&converters) == IS_ARRAY) {
									if (zephir_array_isset(&converters, &part)) {
									}
								}
							}
						} ZEND_HASH_FOREACH_END();
					} else {
						ZEPHIR_CALL_METHOD(NULL, &paths, "rewind", NULL, 0);
						zephir_check_call_status();
						while (1) {
							ZEPHIR_CALL_METHOD(&_10$$28, &paths, "valid", NULL, 0);
							zephir_check_call_status();
							if (!zend_is_true(&_10$$28)) {
								break;
							}
							ZEPHIR_CALL_METHOD(&part, &paths, "key", NULL, 0);
							zephir_check_call_status();
							ZEPHIR_CALL_METHOD(&position, &paths, "current", NULL, 0);
							zephir_check_call_status();
								ZEPHIR_OBS_NVAR(&matchPosition);
								if (zephir_array_isset_fetch(&matchPosition, &matches, &position, 0)) {
									if (Z_TYPE_P(&converters) == IS_ARRAY) {
										if (zephir_array_isset(&converters, &part)) {
											continue;
										}
									}
									zephir_array_update_zval(&parts, &part, &matchPosition, PH_COPY | PH_SEPARATE);
								} else {
									if (Z_TYPE_P(&converters) == IS_ARRAY) {
										if (zephir_array_isset(&converters, &part)) {
										}
									}
								}
							ZEPHIR_CALL_METHOD(NULL, &paths, "next", NULL, 0);
							zephir_check_call_status();
						}
					}
					ZEPHIR_INIT_NVAR(&position);
					ZEPHIR_INIT_NVAR(&part);
					zephir_update_property_zval(this_ptr, ZEND_STRL("_matches"), &matches);
				}
				zephir_update_property_zval(this_ptr, ZEND_STRL("_matchedRoute"), &route);
				break;
			}
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_1, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_3, &_1, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&route, &_1, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&methods, &route, "gethttpmethods", NULL, 0);
				zephir_check_call_status();
				if (Z_TYPE_P(&methods) != IS_NULL) {
					if (Z_TYPE_P(&request) == IS_NULL) {
						zephir_read_property(&_13$$43, this_ptr, SL("_dependencyInjector"), PH_NOISY_CC | PH_READONLY);
						ZEPHIR_CPY_WRT(&dependencyInjector, &_13$$43);
						if (Z_TYPE_P(&dependencyInjector) != IS_OBJECT) {
							ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 342);
							return;
						}
						ZEPHIR_INIT_NVAR(&_14$$43);
						ZVAL_STRING(&_14$$43, "request");
						ZEPHIR_CALL_METHOD(&request, &dependencyInjector, "getshared", NULL, 0, &_14$$43);
						zephir_check_call_status();
					}
					ZEPHIR_CALL_METHOD(&_15$$42, &request, "ismethod", NULL, 0, &methods);
					zephir_check_call_status();
					if (ZEPHIR_IS_FALSE_IDENTICAL(&_15$$42)) {
						continue;
					}
				}
				ZEPHIR_CALL_METHOD(&hostname, &route, "gethostname", NULL, 0);
				zephir_check_call_status();
				if (Z_TYPE_P(&hostname) != IS_NULL) {
					if (Z_TYPE_P(&request) == IS_NULL) {
						ZEPHIR_OBS_NVAR(&dependencyInjector);
						zephir_read_property(&dependencyInjector, this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
						if (Z_TYPE_P(&dependencyInjector) != IS_OBJECT) {
							ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "A dependency injection container is required to access the 'request' service", "test/router.zep", 363);
							return;
						}
						ZEPHIR_INIT_NVAR(&_16$$47);
						ZVAL_STRING(&_16$$47, "request");
						ZEPHIR_CALL_METHOD(&request, &dependencyInjector, "getshared", NULL, 0, &_16$$47);
						zephir_check_call_status();
					}
					if (Z_TYPE_P(&currentHostName) != IS_OBJECT) {
						ZEPHIR_CALL_METHOD(&currentHostName, &request, "gethttphost", NULL, 0);
						zephir_check_call_status();
					}
					if (Z_TYPE_P(&currentHostName) != IS_NULL) {
						continue;
					}
					if (zephir_memnstr_str(&hostname, SL("("), "test/router.zep", 380)) {
						if (zephir_memnstr_str(&hostname, SL("#"), "test/router.zep", 381)) {
							ZEPHIR_INIT_NVAR(&regexHostName);
							ZEPHIR_CONCAT_SVS(&regexHostName, "#^", &hostname, "$#");
						} else {
							ZEPHIR_CPY_WRT(&regexHostName, &hostname);
						}
						ZEPHIR_INIT_NVAR(&_17$$51);
						ZEPHIR_INIT_NVAR(&matched);
						zephir_preg_match(&matched, &regexHostName, &currentHostName, &_17$$51, 0, 0 , 0 );
					} else {
						ZEPHIR_INIT_NVAR(&matched);
						ZVAL_BOOL(&matched, ZEPHIR_IS_EQUAL(&currentHostName, &hostname));
					}
					if (!(zephir_is_true(&matched))) {
						continue;
					}
				}
				ZEPHIR_CALL_METHOD(&pattern, &route, "getcompiledpattern", NULL, 0);
				zephir_check_call_status();
				if (zephir_memnstr_str(&pattern, SL("^"), "test/router.zep", 399)) {
					ZEPHIR_INIT_NVAR(&routeFound);
					zephir_preg_match(&routeFound, &pattern, &handledUri, &matches, 0, 0 , 0 );
				} else {
					ZEPHIR_INIT_NVAR(&routeFound);
					ZVAL_BOOL(&routeFound, ZEPHIR_IS_EQUAL(&pattern, &handledUri));
				}
				if (zephir_is_true(&routeFound)) {
					ZEPHIR_CALL_METHOD(&beforeMatch, &route, "getbeforematch", NULL, 0);
					zephir_check_call_status();
					if (Z_TYPE_P(&beforeMatch) != IS_NULL) {
						if (zephir_is_callable(&beforeMatch)) {
							ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "Before-Match callback is not callable in matched route", "test/router.zep", 413);
							return;
						}
					}
				}
				if (zephir_is_true(&routeFound)) {
					ZEPHIR_CALL_METHOD(&paths, &route, "getpaths", NULL, 0);
					zephir_check_call_status();
					ZEPHIR_CPY_WRT(&parts, &paths);
					if (Z_TYPE_P(&matches) == IS_ARRAY) {
						ZEPHIR_CALL_METHOD(&converters, &route, "getconverters", NULL, 0);
						zephir_check_call_status();
						zephir_is_iterable(&paths, 0, "test/router.zep", 465);
						if (Z_TYPE_P(&paths) == IS_ARRAY) {
							ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&paths), _20$$62, _21$$62, _18$$62)
							{
								ZEPHIR_INIT_NVAR(&part);
								if (_21$$62 != NULL) { 
									ZVAL_STR_COPY(&part, _21$$62);
								} else {
									ZVAL_LONG(&part, _20$$62);
								}
								ZEPHIR_INIT_NVAR(&position);
								ZVAL_COPY(&position, _18$$62);
								ZEPHIR_OBS_NVAR(&matchPosition);
								if (zephir_array_isset_fetch(&matchPosition, &matches, &position, 0)) {
									if (Z_TYPE_P(&converters) == IS_ARRAY) {
										if (zephir_array_isset(&converters, &part)) {
											continue;
										}
									}
									zephir_array_update_zval(&parts, &part, &matchPosition, PH_COPY | PH_SEPARATE);
								} else {
									if (Z_TYPE_P(&converters) == IS_ARRAY) {
										if (zephir_array_isset(&converters, &part)) {
										}
									}
								}
							} ZEND_HASH_FOREACH_END();
						} else {
							ZEPHIR_CALL_METHOD(NULL, &paths, "rewind", NULL, 0);
							zephir_check_call_status();
							while (1) {
								ZEPHIR_CALL_METHOD(&_19$$62, &paths, "valid", NULL, 0);
								zephir_check_call_status();
								if (!zend_is_true(&_19$$62)) {
									break;
								}
								ZEPHIR_CALL_METHOD(&part, &paths, "key", NULL, 0);
								zephir_check_call_status();
								ZEPHIR_CALL_METHOD(&position, &paths, "current", NULL, 0);
								zephir_check_call_status();
									ZEPHIR_OBS_NVAR(&matchPosition);
									if (zephir_array_isset_fetch(&matchPosition, &matches, &position, 0)) {
										if (Z_TYPE_P(&converters) == IS_ARRAY) {
											if (zephir_array_isset(&converters, &part)) {
												continue;
											}
										}
										zephir_array_update_zval(&parts, &part, &matchPosition, PH_COPY | PH_SEPARATE);
									} else {
										if (Z_TYPE_P(&converters) == IS_ARRAY) {
											if (zephir_array_isset(&converters, &part)) {
											}
										}
									}
								ZEPHIR_CALL_METHOD(NULL, &paths, "next", NULL, 0);
								zephir_check_call_status();
							}
						}
						ZEPHIR_INIT_NVAR(&position);
						ZEPHIR_INIT_NVAR(&part);
						zephir_update_property_zval(this_ptr, ZEND_STRL("_matches"), &matches);
					}
					zephir_update_property_zval(this_ptr, ZEND_STRL("_matchedRoute"), &route);
					break;
				}
			ZEPHIR_CALL_METHOD(NULL, &_1, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&route);
	if (zephir_is_true(&routeFound)) {
		if (1) {
			zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$true);
		} else {
			zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$false);
		}
	} else {
		if (0) {
			zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$true);
		} else {
			zephir_update_property_zval(this_ptr, ZEND_STRL("_wasMatched"), &__$false);
		}
	}
	if (!(zephir_is_true(&routeFound))) {
		ZEPHIR_OBS_VAR(&notFoundPaths);
		zephir_read_property(&notFoundPaths, this_ptr, SL("_notFoundPaths"), PH_NOISY_CC);
		if (Z_TYPE_P(&notFoundPaths) != IS_NULL) {
			ZEPHIR_CPY_WRT(&parts, &notFoundPaths);
			ZEPHIR_INIT_NVAR(&routeFound);
			ZVAL_BOOL(&routeFound, 1);
		}
	}
	if (zephir_is_true(&routeFound)) {
		ZEPHIR_OBS_VAR(&vnamespace);
		if (zephir_array_isset_string_fetch(&vnamespace, &parts, SL("namespace"), 0)) {
			if (!(zephir_is_numeric(&vnamespace))) {
				zephir_update_property_zval(this_ptr, ZEND_STRL("_namespace"), &vnamespace);
			}
			zephir_array_unset_string(&parts, SL("namespace"), PH_SEPARATE);
		} else {
			zephir_read_property(&_22$$82, this_ptr, SL("_defaultNamespace"), PH_NOISY_CC | PH_READONLY);
			zephir_update_property_zval(this_ptr, ZEND_STRL("_namespace"), &_22$$82);
		}
		ZEPHIR_OBS_VAR(&module);
		if (zephir_array_isset_string_fetch(&module, &parts, SL("module"), 0)) {
			if (!(zephir_is_numeric(&module))) {
				zephir_update_property_zval(this_ptr, ZEND_STRL("_module"), &module);
			}
			zephir_array_unset_string(&parts, SL("module"), PH_SEPARATE);
		} else {
			zephir_read_property(&_23$$85, this_ptr, SL("_defaultModule"), PH_NOISY_CC | PH_READONLY);
			zephir_update_property_zval(this_ptr, ZEND_STRL("_module"), &_23$$85);
		}
		ZEPHIR_OBS_VAR(&controller);
		if (zephir_array_isset_string_fetch(&controller, &parts, SL("controller"), 0)) {
			if (!(zephir_is_numeric(&controller))) {
				zephir_update_property_zval(this_ptr, ZEND_STRL("_controller"), &controller);
			}
			zephir_array_unset_string(&parts, SL("controller"), PH_SEPARATE);
		} else {
			zephir_read_property(&_24$$88, this_ptr, SL("_defaultController"), PH_NOISY_CC | PH_READONLY);
			zephir_update_property_zval(this_ptr, ZEND_STRL("_controller"), &_24$$88);
		}
		ZEPHIR_OBS_VAR(&action);
		if (zephir_array_isset_string_fetch(&action, &parts, SL("action"), 0)) {
			if (!(zephir_is_numeric(&action))) {
				zephir_update_property_zval(this_ptr, ZEND_STRL("_action"), &action);
			}
			zephir_array_unset_string(&parts, SL("action"), PH_SEPARATE);
		} else {
			zephir_read_property(&_25$$91, this_ptr, SL("_defaultAction"), PH_NOISY_CC | PH_READONLY);
			zephir_update_property_zval(this_ptr, ZEND_STRL("_action"), &_25$$91);
		}
		ZEPHIR_OBS_VAR(&paramsStr);
		if (zephir_array_isset_string_fetch(&paramsStr, &parts, SL("params"), 0)) {
			ZVAL_LONG(&_26$$92, 1);
			ZEPHIR_INIT_VAR(&strParams);
			zephir_substr(&strParams, &paramsStr, 1 , 0, ZEPHIR_SUBSTR_NO_LENGTH);
			if (zephir_is_true(&strParams)) {
				ZEPHIR_INIT_NVAR(&params);
				zephir_fast_explode_str(&params, SL("/"), &strParams, LONG_MAX);
			}
			zephir_array_unset_string(&parts, SL("params"), PH_SEPARATE);
		}
		if (zephir_fast_count_int(&params)) {
			ZEPHIR_INIT_VAR(&paramsMerge);
			zephir_fast_array_merge(&paramsMerge, &params, &parts);
		} else {
			ZEPHIR_CPY_WRT(&paramsMerge, &parts);
		}
		zephir_update_property_zval(this_ptr, ZEND_STRL("_params"), &paramsMerge);
	} else {
		zephir_read_property(&_27$$96, this_ptr, SL("_defaultNamespace"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_namespace"), &_27$$96);
		zephir_read_property(&_28$$96, this_ptr, SL("_defaultModule"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_module"), &_28$$96);
		zephir_read_property(&_29$$96, this_ptr, SL("_defaultController"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_controller"), &_29$$96);
		zephir_read_property(&_30$$96, this_ptr, SL("_defaultAction"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_action"), &_30$$96);
		zephir_read_property(&_31$$96, this_ptr, SL("_defaultParams"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_params"), &_31$$96);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, *httpMethods = NULL, httpMethods_sub, __$null, route;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_UNDEF(&httpMethods_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&route);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &pattern, &paths, &httpMethods);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}
	if (!httpMethods) {
		httpMethods = &httpMethods_sub;
		httpMethods = &__$null;
	}


	ZEPHIR_INIT_VAR(&route);
	object_init_ex(&route, test_router_route_ce);
	ZEPHIR_CALL_METHOD(NULL, &route, "__construct", NULL, 83, pattern, paths, httpMethods);
	zephir_check_call_status();
	zephir_update_property_array_append(this_ptr, SL("_routes"), &route);
	RETURN_CCTOR(&route);

}

/**
 * Adds a route to the router that only match if the HTTP method is GET
 *
 * @param string pattern
 * @param string/array paths
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, addGet) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "GET");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "POST");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "PUT");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "PATCH");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "DELETE");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "OPTIONS");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "HEAD");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "add", NULL, 0, pattern, paths, &_0);
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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *group, group_sub, groupRoutes, beforeMatch, hostname, routes, route, *_0$$5, _1$$5, *_2$$8, _3$$8, _4$$11;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&group_sub);
	ZVAL_UNDEF(&groupRoutes);
	ZVAL_UNDEF(&beforeMatch);
	ZVAL_UNDEF(&hostname);
	ZVAL_UNDEF(&routes);
	ZVAL_UNDEF(&route);
	ZVAL_UNDEF(&_1$$5);
	ZVAL_UNDEF(&_3$$8);
	ZVAL_UNDEF(&_4$$11);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &group);



	if (Z_TYPE_P(group) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The group of routes is not valid", "test/router.zep", 677);
		return;
	}
	ZEPHIR_CALL_METHOD(&groupRoutes, group, "getroutes", NULL, 0);
	zephir_check_call_status();
	if (!(zephir_fast_count_int(&groupRoutes))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The group of routes does not contain any routes", "test/router.zep", 682);
		return;
	}
	ZEPHIR_CALL_METHOD(&beforeMatch, group, "getbeforematch", NULL, 0);
	zephir_check_call_status();
	if (Z_TYPE_P(&beforeMatch) != IS_NULL) {
		zephir_is_iterable(&groupRoutes, 0, "test/router.zep", 692);
		if (Z_TYPE_P(&groupRoutes) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&groupRoutes), _0$$5)
			{
				ZEPHIR_INIT_NVAR(&route);
				ZVAL_COPY(&route, _0$$5);
				ZEPHIR_CALL_METHOD(NULL, &route, "beforematch", NULL, 0, &beforeMatch);
				zephir_check_call_status();
			} ZEND_HASH_FOREACH_END();
		} else {
			ZEPHIR_CALL_METHOD(NULL, &groupRoutes, "rewind", NULL, 0);
			zephir_check_call_status();
			while (1) {
				ZEPHIR_CALL_METHOD(&_1$$5, &groupRoutes, "valid", NULL, 0);
				zephir_check_call_status();
				if (!zend_is_true(&_1$$5)) {
					break;
				}
				ZEPHIR_CALL_METHOD(&route, &groupRoutes, "current", NULL, 0);
				zephir_check_call_status();
					ZEPHIR_CALL_METHOD(NULL, &route, "beforematch", NULL, 0, &beforeMatch);
					zephir_check_call_status();
				ZEPHIR_CALL_METHOD(NULL, &groupRoutes, "next", NULL, 0);
				zephir_check_call_status();
			}
		}
		ZEPHIR_INIT_NVAR(&route);
	}
	ZEPHIR_CALL_METHOD(&hostname, group, "gethostname", NULL, 0);
	zephir_check_call_status();
	if (Z_TYPE_P(&hostname) != IS_NULL) {
		zephir_is_iterable(&groupRoutes, 0, "test/router.zep", 701);
		if (Z_TYPE_P(&groupRoutes) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&groupRoutes), _2$$8)
			{
				ZEPHIR_INIT_NVAR(&route);
				ZVAL_COPY(&route, _2$$8);
				ZEPHIR_CALL_METHOD(NULL, &route, "sethostname", NULL, 0, &hostname);
				zephir_check_call_status();
			} ZEND_HASH_FOREACH_END();
		} else {
			ZEPHIR_CALL_METHOD(NULL, &groupRoutes, "rewind", NULL, 0);
			zephir_check_call_status();
			while (1) {
				ZEPHIR_CALL_METHOD(&_3$$8, &groupRoutes, "valid", NULL, 0);
				zephir_check_call_status();
				if (!zend_is_true(&_3$$8)) {
					break;
				}
				ZEPHIR_CALL_METHOD(&route, &groupRoutes, "current", NULL, 0);
				zephir_check_call_status();
					ZEPHIR_CALL_METHOD(NULL, &route, "sethostname", NULL, 0, &hostname);
					zephir_check_call_status();
				ZEPHIR_CALL_METHOD(NULL, &groupRoutes, "next", NULL, 0);
				zephir_check_call_status();
			}
		}
		ZEPHIR_INIT_NVAR(&route);
	}
	ZEPHIR_OBS_VAR(&routes);
	zephir_read_property(&routes, this_ptr, SL("_routes"), PH_NOISY_CC);
	if (Z_TYPE_P(&routes) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(&_4$$11);
		zephir_fast_array_merge(&_4$$11, &routes, &groupRoutes);
		zephir_update_property_zval(this_ptr, ZEND_STRL("_routes"), &_4$$11);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("_routes"), &groupRoutes);
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

	zval *paths, paths_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&paths_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &paths);



	if (Z_TYPE_P(paths) != IS_ARRAY) {
		if (Z_TYPE_P(paths) != IS_STRING) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_router_exception_ce, "The not-found paths must be an array or string", "test/router.zep", 724);
			return;
		}
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("_notFoundPaths"), paths);
	RETURN_THISW();

}

/**
 * Removes all the pre-defined routes
 */
PHP_METHOD(Test_Router, clear) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_routes"), &_0);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the processed namespace name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getNamespaceName) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_namespace");

}

/**
 * Returns the processed module name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getModuleName) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_module");

}

/**
 * Returns the processed controller name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getControllerName) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_controller");

}

/**
 * Returns the processed action name
 *
 * @return string
 */
PHP_METHOD(Test_Router, getActionName) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_action");

}

/**
 * Returns the processed parameters
 *
 * @return array
 */
PHP_METHOD(Test_Router, getParams) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_params");

}

/**
 * Returns the route that matchs the handled URI
 *
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getMatchedRoute) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_matchedRoute");

}

/**
 * Returns the sub expressions in the regular expression matched
 *
 * @return array
 */
PHP_METHOD(Test_Router, getMatches) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_matches");

}

/**
 * Checks if the router macthes any of the defined routes
 *
 * @return bool
 */
PHP_METHOD(Test_Router, wasMatched) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_wasMatched");

}

/**
 * Returns all the routes defined in the router
 *
 * @return Test\Router\Route[]
 */
PHP_METHOD(Test_Router, getRoutes) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "_routes");

}

/**
 * Returns a route object by its id
 *
 * @param string id
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteById) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *id, id_sub, route, _0, *_1, _2, _3$$3, _4$$5;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&id_sub);
	ZVAL_UNDEF(&route);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &id);



	zephir_read_property(&_0, this_ptr, SL("_routes"), PH_NOISY_CC | PH_READONLY);
	zephir_is_iterable(&_0, 0, "test/router.zep", 844);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _1)
		{
			ZEPHIR_INIT_NVAR(&route);
			ZVAL_COPY(&route, _1);
			ZEPHIR_CALL_METHOD(&_3$$3, &route, "getrouteid", NULL, 0);
			zephir_check_call_status();
			if (ZEPHIR_IS_EQUAL(&_3$$3, id)) {
				RETURN_CCTOR(&route);
			}
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&route, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&_4$$5, &route, "getrouteid", NULL, 0);
				zephir_check_call_status();
				if (ZEPHIR_IS_EQUAL(&_4$$5, id)) {
					RETURN_CCTOR(&route);
				}
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&route);
	RETURN_MM_BOOL(0);

}

/**
 * Returns a route object by its name
 *
 * @param string name
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router, getRouteByName) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub, route, _0, *_1, _2, _3$$3, _4$$5;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&route);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name);



	zephir_read_property(&_0, this_ptr, SL("_routes"), PH_NOISY_CC | PH_READONLY);
	zephir_is_iterable(&_0, 0, "test/router.zep", 862);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _1)
		{
			ZEPHIR_INIT_NVAR(&route);
			ZVAL_COPY(&route, _1);
			ZEPHIR_CALL_METHOD(&_3$$3, &route, "getname", NULL, 0);
			zephir_check_call_status();
			if (ZEPHIR_IS_EQUAL(&_3$$3, name)) {
				RETURN_CCTOR(&route);
			}
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&route, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&_4$$5, &route, "getname", NULL, 0);
				zephir_check_call_status();
				if (ZEPHIR_IS_EQUAL(&_4$$5, name)) {
					RETURN_CCTOR(&route);
				}
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&route);
	RETURN_MM_BOOL(0);

}

