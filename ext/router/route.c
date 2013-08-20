
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/string_type.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/hash.h"


/**
 * Test\Router\Route
 *
 * This class represents every route added to the router
 */
ZEPHIR_INIT_CLASS(Test_Router_Route) {

	ZEPHIR_REGISTER_CLASS(Test\\Router, Route, router_route, test_router_route_method_entry, 0);

	zend_declare_property_null(test_router_route_ce, SL("_pattern"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_compiledPattern"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_paths"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_methods"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_hostname"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_converters"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_id"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_name"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(test_router_route_ce, SL("_beforeMatch"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

/**
 * Test\Router\Route constructor
 *
 * @param string pattern
 * @param array paths
 * @param array|string httpMethods
 */
PHP_METHOD(Test_Router_Route, __construct) {

	zval *pattern, *paths = NULL, *httpMethods = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(0, 1, 2, &pattern, &paths, &httpMethods);

	if (!paths) {
		ZEPHIR_INIT_VAR(paths);
	}
	if (!httpMethods) {
		ZEPHIR_INIT_VAR(httpMethods);
	}


	//missing comment
	zephir_call_method_p2_noret(this_ptr, "reconfigure", pattern, paths);
	//missing comment

	zephir_update_property_this(this_ptr, SL("_methods"), httpMethods TSRMLS_CC);

	ZEPHIR_MM_RESTORE();

}

/**
 * Replaces placeholders from pattern returning a valid PCRE regular expression
 *
 * @param string pattern
 * @return string
 */
PHP_METHOD(Test_Router_Route, compilePattern) {

	zval *pattern, *compiledPattern = NULL, *idPattern, *patternCopy = NULL, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern);



	ZEPHIR_CPY_WRT(compiledPattern, pattern);

	//missing comment
	if (zephir_memnstr_str(pattern, SL(":"))) {
		//missing comment

		ZEPHIR_INIT_VAR(idPattern);
		ZVAL_STRING(idPattern, "/([a-zA-Z0-9\\_\\-]+)", 1);

		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:module"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_0);
			ZVAL_STRING(_0, "/:module", 1);
			ZEPHIR_INIT_VAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _0, idPattern, patternCopy);

		}
		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:controller"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "/:controller", 1);
			ZEPHIR_INIT_NVAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _1, idPattern, patternCopy);

		}
		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:namespace"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_2);
			ZVAL_STRING(_2, "/:namespace", 1);
			ZEPHIR_INIT_NVAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _2, idPattern, patternCopy);

		}
		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:action"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_3);
			ZVAL_STRING(_3, "/:action", 1);
			ZEPHIR_INIT_NVAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _3, idPattern, patternCopy);

		}
		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:params"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_4);
			ZVAL_STRING(_4, "/:params", 1);
			ZEPHIR_INIT_VAR(_5);
			ZVAL_STRING(_5, "(/.*)*", 1);
			ZEPHIR_INIT_NVAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _4, _5, patternCopy);

		}
		//missing comment
		if (zephir_memnstr_str(pattern, SL("/:int"))) {

			ZEPHIR_CPY_WRT(patternCopy, compiledPattern);

			ZEPHIR_INIT_VAR(_6);
			ZVAL_STRING(_6, "/:int", 1);
			ZEPHIR_INIT_VAR(_7);
			ZVAL_STRING(_7, "/([0-9]+)", 1);
			ZEPHIR_INIT_NVAR(compiledPattern);
			zephir_call_func_p3(compiledPattern, "str_replace", _6, _7, patternCopy);

		}
	}
	//missing comment
	if (zephir_memnstr_str(compiledPattern, SL("("))) {

		RETURN_MM_NULL();

	}
	//missing comment
	if (zephir_memnstr_str(compiledPattern, SL("["))) {

		RETURN_MM_NULL();

	}

	RETURN_CCTOR(compiledPattern);


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
PHP_METHOD(Test_Router_Route, via) {

	zval *httpMethods;

	zephir_fetch_params(0, 1, 0, &httpMethods);



	zephir_update_property_this(this_ptr, SL("_methods"), httpMethods TSRMLS_CC);

	RETURN_THISW();


}

/**
 * Extracts parameters from a string
 *
 * @param string pattern
 */
PHP_METHOD(Test_Router_Route, extractNamedParams) {

	zend_bool notValid;
	int cursor, marker, bracketCount, parenthesesCount, intermediate, length, numberMatches, cursorVar, _2, _3;
	char ch;
	zval *pattern_param = NULL, variable, *_0, *_1;
	zephir_str *pattern, *route = NULL, *item = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern_param);

		zephir_get_strval(pattern_param, &pattern);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRINGL(_0, pattern->str, pattern->len, 1);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "strlen", _0);
	if (ZEPHIR_IS_LONG(_1, 0)) {

		RETURN_MM_BOOL(0);

	}
	for (_2 = 0; _2 < pattern->len; _2++) {
		cursor = _2; 
		ch = pattern->str[_2]; 
		if (parenthesesCount == 0) {
			if (ch == '{') {
				if (bracketCount == 0) {

					marker = cursor;

					intermediate = 0;

					notValid = 0;

				}

				bracketCount++;

			} else {
				if (ch == '}') {

					bracketCount--;

					if (intermediate > 0) {
						if (bracketCount == 0) {
							//missing comment

							numberMatches++;

							ZEPHIR_SINIT_NVAR(variable);
							ZVAL_NULL(&variable);

							length = cursor - marker - 1;

							zephir_str_assign(item, "", sizeof("")-1);

							for (_3 = 0; _3 < item->len; _3++) {
								cursorVar = _3; 
								ch = item->str[_3]; 
								if (ch == '\0') {
									break;
								}
								if (cursorVar == 0 && !((ch <= 'a' && ch <= 'z') || (ch <= 'A' && ch <= 'Z'))) {

									notValid = (1) ? 1 : 0;

									break;
								}
								if ((ch <= 'a' && ch <= 'z') || (ch <= 'A' && ch <= 'Z') || (ch <= '0' && ch <= '9') || ch == '-' || ch == '_' || ch == ':') {
									if (ch == ':') {
										break;
									}
								} else {

									notValid = 1;

									break;
								}
							}
							if (!(notValid)) {
								//missing comment
							} else {
								//missing comment
							}
							continue;
						}
					}
				}
			}
		}
		if (bracketCount == 0) {
			if (ch == '(') {

				parenthesesCount++;

			} else {
				if (ch == ')') {

					parenthesesCount--;

					if (parenthesesCount == 0) {

						numberMatches++;

					}
				}
			}
		}
		if (bracketCount > 0) {

			intermediate++;

		} else {

			zephir_str_append_char(route, ch);

		}
	}
	//missing comment
	ZEPHIR_MM_RESTORE();

}

/**
 * Reconfigure the route adding a new pattern and a set of paths
 *
 * @param string pattern
 * @param array paths
 */
PHP_METHOD(Test_Router_Route, reConfigure) {

	zval *pattern, *paths = NULL, *moduleName = NULL, *controllerName = NULL, *actionName = NULL, *parts, *routePaths = NULL, *realClassName = NULL, *namespaceName, *pcrePattern = NULL, *compiledPattern = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		ZEPHIR_INIT_VAR(paths);
	}


	if (Z_TYPE_P(pattern) != IS_STRING) {
		ZEPHIR_THROW_EXCEPTION_STR(test_router_exception_ce, "The pattern must be string");
		return;
	}
	if (Z_TYPE_P(paths) != IS_NULL) {
		if (Z_TYPE_P(paths) == IS_STRING) {

			ZEPHIR_INIT_VAR(moduleName);
			ZVAL_NULL(moduleName);

			ZEPHIR_INIT_VAR(controllerName);
			ZVAL_NULL(controllerName);

			ZEPHIR_INIT_VAR(actionName);
			ZVAL_NULL(actionName);

			//missing comment
			ZEPHIR_INIT_VAR(_0);
			ZVAL_STRING(_0, "::", 1);
			ZEPHIR_INIT_VAR(parts);
			zephir_call_func_p2(parts, "explode", _0, paths);

			//missing comment
			do {
				if (zephir_fast_count_ev(parts TSRMLS_CC) == 3) {
					ZEPHIR_OBS_NVAR(moduleName);
					zephir_array_fetch_long(&moduleName, parts, 0, PH_NOISY);

					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 1, PH_NOISY);

					ZEPHIR_OBS_NVAR(actionName);
					zephir_array_fetch_long(&actionName, parts, 2, PH_NOISY);

					break;
				}
				if (zephir_fast_count_ev(parts TSRMLS_CC) == 2) {
					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 0, PH_NOISY);

					ZEPHIR_OBS_NVAR(actionName);
					zephir_array_fetch_long(&actionName, parts, 1, PH_NOISY);

					break;
				}
				if (zephir_fast_count_ev(parts TSRMLS_CC) == 1) {
					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 0, PH_NOISY);

					break;
				}
			} while(0); 

			ZEPHIR_INIT_VAR(routePaths);
			array_init(routePaths);

			//missing comment
			if (Z_TYPE_P(moduleName) != IS_NULL) {

				zephir_array_update_string(&routePaths, SL("module"), &moduleName, PH_COPY | PH_SEPARATE);

			}
			//missing comment
			if (Z_TYPE_P(controllerName) != IS_NULL) {
				//missing comment
				if (zephir_memnstr_str(controllerName, SL("\\"))) {
					//missing comment
					ZEPHIR_INIT_VAR(realClassName);
					zephir_call_func_p1(realClassName, "get_class_ns", controllerName);

					//missing comment
					ZEPHIR_INIT_VAR(namespaceName);
					zephir_call_func_p1(namespaceName, "get_ns_class", controllerName);

					//missing comment
					if (zend_is_true(namespaceName)) {

						zephir_array_update_string(&routePaths, SL("namespace"), &namespaceName, PH_COPY | PH_SEPARATE);

					}
				} else {

					ZEPHIR_CPY_WRT(realClassName, controllerName);

				}
				//missing comment
				ZEPHIR_INIT_VAR(_1);
				zephir_call_func_p1(_1, "uncamelize", realClassName);

				zephir_array_update_string(&routePaths, SL("controller"), &_1, PH_COPY | PH_SEPARATE);

			}
			//missing comment
			if (Z_TYPE_P(actionName) != IS_NULL) {

				zephir_array_update_string(&routePaths, SL("action"), &actionName, PH_COPY | PH_SEPARATE);

			}
		} else {

			ZEPHIR_CPY_WRT(routePaths, paths);

		}
	} else {

		ZEPHIR_INIT_NVAR(routePaths);
		array_init(routePaths);

	}
	if (Z_TYPE_P(routePaths) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_STR(test_router_exception_ce, "The route contains invalid paths");
		return;
	}
	//missing comment
	if (!(zephir_start_with_str(pattern, SL("#")))) {
		if (zephir_memnstr_str(pattern, SL("{"))) {
			//missing comment
			//missing comment
			//missing comment
			//missing comment

			ZEPHIR_CPY_WRT(pcrePattern, pattern);

		} else {

			ZEPHIR_CPY_WRT(pcrePattern, pattern);

		}
		//missing comment
		ZEPHIR_INIT_VAR(compiledPattern);
		zephir_call_method_p1(compiledPattern, this_ptr, "compilepattern", pcrePattern);

	} else {

		ZEPHIR_CPY_WRT(compiledPattern, pattern);

	}
	//missing comment

	zephir_update_property_this(this_ptr, SL("_pattern"), pattern TSRMLS_CC);

	//missing comment

	zephir_update_property_this(this_ptr, SL("_compiledPattern"), compiledPattern TSRMLS_CC);

	//missing comment

	zephir_update_property_this(this_ptr, SL("_paths"), routePaths TSRMLS_CC);

	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the route's name
 *
 * @return string
 */
PHP_METHOD(Test_Router_Route, getName) {

	RETURN_MEMBER(this_ptr, "_name");

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
PHP_METHOD(Test_Router_Route, setName) {

	zval *name;

	zephir_fetch_params(0, 1, 0, &name);



	zephir_update_property_this(this_ptr, SL("_name"), name TSRMLS_CC);

	RETURN_THISW();


}

/**
 * Sets a callback that is called if the route is matched.
 * The developer can implement any arbitrary conditions here
 * If the callback returns false the route is treaded as not matched
 *
 * @param callback callback
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router_Route, beforeMatch) {

	zval *callback;

	zephir_fetch_params(0, 1, 0, &callback);



	zephir_update_property_this(this_ptr, SL("_beforeMatch"), callback TSRMLS_CC);

	RETURN_THISW();


}

/**
 * Returns the 'before match' callback if any
 *
 * @return mixed
 */
PHP_METHOD(Test_Router_Route, getBeforeMatch) {

	RETURN_MEMBER(this_ptr, "_beforeMatch");

}

/**
 * Returns the route's id
 *
 * @return string
 */
PHP_METHOD(Test_Router_Route, getRouteId) {

	RETURN_MEMBER(this_ptr, "_id");

}

/**
 * Returns the route's pattern
 *
 * @return string
 */
PHP_METHOD(Test_Router_Route, getPattern) {

	RETURN_MEMBER(this_ptr, "_pattern");

}

/**
 * Returns the route's compiled pattern
 *
 * @return string
 */
PHP_METHOD(Test_Router_Route, getCompiledPattern) {

	RETURN_MEMBER(this_ptr, "_compiledPattern");

}

/**
 * Returns the paths
 *
 * @return array
 */
PHP_METHOD(Test_Router_Route, getPaths) {

	RETURN_MEMBER(this_ptr, "_paths");

}

/**
 * Returns the paths using positions as keys and names as values
 *
 * @return array
 */
PHP_METHOD(Test_Router_Route, getReversedPaths) {

	HashTable *_2;
	HashPosition _1;
	zval *reversed, *path = NULL, *position = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(reversed);
	array_init(reversed);

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property(&_0, this_ptr, SL("_paths"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(path, _2, _1);
		ZEPHIR_GET_HVALUE(position, _3);

		zephir_array_update_zval(&reversed, position, &path, PH_COPY | PH_SEPARATE);

	}

	RETURN_CCTOR(reversed);


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
PHP_METHOD(Test_Router_Route, setHttpMethods) {

	zval *httpMethods;

	zephir_fetch_params(0, 1, 0, &httpMethods);



	zephir_update_property_this(this_ptr, SL("_methods"), httpMethods TSRMLS_CC);

	RETURN_THISW();


}

/**
 * Returns the HTTP methods that constraint matching the route
 *
 * @return string|array
 */
PHP_METHOD(Test_Router_Route, getHttpMethods) {

	RETURN_MEMBER(this_ptr, "_methods");

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
PHP_METHOD(Test_Router_Route, setHostname) {

	zval *hostname;

	zephir_fetch_params(0, 1, 0, &hostname);



	zephir_update_property_this(this_ptr, SL("_hostname"), hostname TSRMLS_CC);

	RETURN_THISW();


}

/**
 * Returns the hostname restriction if any
 *
 * @return string
 */
PHP_METHOD(Test_Router_Route, getHostname) {

	RETURN_MEMBER(this_ptr, "_hostname");

}

/**
 * Adds a converter to perform an additional transformation for certain parameter
 *
 * @param string name
 * @param callable converter
 * @return Test\Router\Route
 */
PHP_METHOD(Test_Router_Route, convert) {

	zval *name, *converter;

	zephir_fetch_params(0, 2, 0, &name, &converter);



	//missing object-property-array-index

	RETURN_THISW();


}

/**
 * Returns the router converter
 *
 * @return array
 */
PHP_METHOD(Test_Router_Route, getConverters) {

	RETURN_MEMBER(this_ptr, "_converters");

}

