
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/hash.h"


/**
 * Test\Router\Route
 *
 * This class represents every route added to the router
 */
ZEPHIR_INIT_CLASS(Test_Router_Route) {

	ZEPHIR_REGISTER_CLASS(Test\\Router, Route, test, router_route, test_router_route_method_entry, 0);

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
 * Test\Router\Route constructor
 *
 * @param string pattern
 * @param array paths
 * @param array|string httpMethods
 */
PHP_METHOD(Test_Router_Route, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *paths = NULL, *httpMethods = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &pattern, &paths, &httpMethods);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}
	if (!httpMethods) {
		httpMethods = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "reconfigure", NULL, 0, pattern, paths);
	zephir_check_call_status();
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

	zval *pattern = NULL, *idPattern = NULL, *_0$$4, _1$$4, *_2$$5, _3$$5, *_4$$6, _5$$6, *_6$$7, _7$$7, *_8$$8, _9$$8, _10$$8, *_11$$9, _12$$9, _13$$9;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern);

	ZEPHIR_SEPARATE_PARAM(pattern);


	if (zephir_memnstr_str(pattern, SL(":"), "test/router/route.zep", 56)) {
		ZEPHIR_INIT_VAR(idPattern);
		ZVAL_STRING(idPattern, "/([a-zA-Z0-9\\_\\-]+)", 1);
		if (zephir_memnstr_str(pattern, SL("/:module"), "test/router/route.zep", 62)) {
			ZEPHIR_INIT_VAR(_0$$4);
			ZEPHIR_SINIT_VAR(_1$$4);
			ZVAL_STRING(&_1$$4, "/:module", 0);
			zephir_fast_str_replace(&_0$$4, &_1$$4, idPattern, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _0$$4);
		}
		if (zephir_memnstr_str(pattern, SL("/:controller"), "test/router/route.zep", 67)) {
			ZEPHIR_INIT_VAR(_2$$5);
			ZEPHIR_SINIT_VAR(_3$$5);
			ZVAL_STRING(&_3$$5, "/:controller", 0);
			zephir_fast_str_replace(&_2$$5, &_3$$5, idPattern, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _2$$5);
		}
		if (zephir_memnstr_str(pattern, SL("/:namespace"), "test/router/route.zep", 72)) {
			ZEPHIR_INIT_VAR(_4$$6);
			ZEPHIR_SINIT_VAR(_5$$6);
			ZVAL_STRING(&_5$$6, "/:namespace", 0);
			zephir_fast_str_replace(&_4$$6, &_5$$6, idPattern, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _4$$6);
		}
		if (zephir_memnstr_str(pattern, SL("/:action"), "test/router/route.zep", 77)) {
			ZEPHIR_INIT_VAR(_6$$7);
			ZEPHIR_SINIT_VAR(_7$$7);
			ZVAL_STRING(&_7$$7, "/:action", 0);
			zephir_fast_str_replace(&_6$$7, &_7$$7, idPattern, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _6$$7);
		}
		if (zephir_memnstr_str(pattern, SL("/:params"), "test/router/route.zep", 82)) {
			ZEPHIR_INIT_VAR(_8$$8);
			ZEPHIR_SINIT_VAR(_9$$8);
			ZVAL_STRING(&_9$$8, "/:params", 0);
			ZEPHIR_SINIT_VAR(_10$$8);
			ZVAL_STRING(&_10$$8, "(/.*)*", 0);
			zephir_fast_str_replace(&_8$$8, &_9$$8, &_10$$8, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _8$$8);
		}
		if (zephir_memnstr_str(pattern, SL("/:int"), "test/router/route.zep", 87)) {
			ZEPHIR_INIT_VAR(_11$$9);
			ZEPHIR_SINIT_VAR(_12$$9);
			ZVAL_STRING(&_12$$9, "/:int", 0);
			ZEPHIR_SINIT_VAR(_13$$9);
			ZVAL_STRING(&_13$$9, "/([0-9]+)", 0);
			zephir_fast_str_replace(&_11$$9, &_12$$9, &_13$$9, pattern TSRMLS_CC);
			ZEPHIR_CPY_WRT(pattern, _11$$9);
		}
	}
	if (zephir_memnstr_str(pattern, SL("("), "test/router/route.zep", 93)) {
		ZEPHIR_CONCAT_SVS(return_value, "#^", pattern, "$#");
		RETURN_MM();
	}
	if (zephir_memnstr_str(pattern, SL("["), "test/router/route.zep", 98)) {
		ZEPHIR_CONCAT_SVS(return_value, "#^", pattern, "$#");
		RETURN_MM();
	}
	RETVAL_ZVAL(pattern, 1, 0);
	RETURN_MM();

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

	long _0, _5$$11, _26$$19;
	zend_bool notValid, _6$$12, _7$$12, _8$$12, _9$$12, _10$$12, _11$$12, _12$$12, _13$$12, _14$$12, _15$$12, _16$$12, _17$$12, _25$$18;
	int tmp = 0, cursor = 0, cursorVar = 0, marker = 0, bracketCount, parenthesesCount, foundPattern, intermediate, numberMatches;
	char ch = 0;
	zval *pattern_param = NULL, *matches = NULL, _1$$11 = zval_used_for_init, _2$$11 = zval_used_for_init, *_3$$11 = NULL, _18$$16 = zval_used_for_init, _19$$16 = zval_used_for_init, *_20$$16 = NULL, _22$$16 = zval_used_for_init, *_23$$16 = NULL, *_27$$19 = NULL, *_28$$28 = NULL;
	zval *pattern = NULL, *route = NULL, *item = NULL, *variable = NULL, *regexp = NULL, *_4$$11 = NULL, *_21$$16 = NULL, *_24$$16 = NULL, *_29$$29 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern_param);

	zephir_get_strval(pattern, pattern_param);


	notValid = 0;
	bracketCount = 0;
	parenthesesCount = 0;
	foundPattern = 0;
	intermediate = 0;
	numberMatches = 0;
	if (zephir_fast_strlen_ev(pattern) <= 0) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_INIT_VAR(matches);
	array_init(matches);
	ZEPHIR_INIT_VAR(route);
	ZVAL_EMPTY_STRING(route);
	for (_0 = 0; _0 < Z_STRLEN_P(pattern); _0++) {
		cursor = _0; 
		ch = ZEPHIR_STRING_OFFSET(pattern, _0);
		if (parenthesesCount == 0) {
			if (ch == '{') {
				if (bracketCount == 0) {
					marker = (cursor + 1);
					intermediate = 0;
					notValid = 0;
				}
				bracketCount++;
			} else {
				if (ch == '}') {
					bracketCount--;
					if (intermediate > 0) {
						if (bracketCount == 0) {
							numberMatches++;
							ZEPHIR_INIT_NVAR(variable);
							ZVAL_EMPTY_STRING(variable);
							ZEPHIR_INIT_NVAR(regexp);
							ZVAL_EMPTY_STRING(regexp);
							ZEPHIR_SINIT_NVAR(_1$$11);
							ZVAL_LONG(&_1$$11, marker);
							ZEPHIR_SINIT_NVAR(_2$$11);
							ZVAL_LONG(&_2$$11, (cursor - marker));
							ZEPHIR_INIT_NVAR(_3$$11);
							zephir_substr(_3$$11, pattern, zephir_get_intval(&_1$$11), zephir_get_intval(&_2$$11), 0);
							zephir_get_strval(_4$$11, _3$$11);
							ZEPHIR_CPY_WRT(item, _4$$11);
							for (_5$$11 = 0; _5$$11 < Z_STRLEN_P(item); _5$$11++) {
								cursorVar = _5$$11; 
								ch = ZEPHIR_STRING_OFFSET(item, _5$$11);
								if (ch == '\0') {
									break;
								}
								_6$$12 = cursorVar == 0;
								if (_6$$12) {
									_7$$12 = ch >= 'a';
									if (_7$$12) {
										_7$$12 = ch <= 'z';
									}
									_8$$12 = _7$$12;
									if (!(_8$$12)) {
										_9$$12 = ch >= 'A';
										if (_9$$12) {
											_9$$12 = ch <= 'Z';
										}
										_8$$12 = _9$$12;
									}
									_6$$12 = !(_8$$12);
								}
								if (_6$$12) {
									notValid = 1;
									break;
								}
								_10$$12 = ch >= 'a';
								if (_10$$12) {
									_10$$12 = ch <= 'z';
								}
								_11$$12 = _10$$12;
								if (!(_11$$12)) {
									_12$$12 = ch >= 'A';
									if (_12$$12) {
										_12$$12 = ch <= 'Z';
									}
									_11$$12 = _12$$12;
								}
								_13$$12 = _11$$12;
								if (!(_13$$12)) {
									_14$$12 = ch >= '0';
									if (_14$$12) {
										_14$$12 = ch <= '9';
									}
									_13$$12 = _14$$12;
								}
								_15$$12 = _13$$12;
								if (!(_15$$12)) {
									_15$$12 = ch == '-';
								}
								_16$$12 = _15$$12;
								if (!(_16$$12)) {
									_16$$12 = ch == '_';
								}
								_17$$12 = _16$$12;
								if (!(_17$$12)) {
									_17$$12 = ch == ':';
								}
								if (_17$$12) {
									if (ch == ':') {
										ZEPHIR_SINIT_NVAR(_18$$16);
										ZVAL_LONG(&_18$$16, 0);
										ZEPHIR_SINIT_NVAR(_19$$16);
										ZVAL_LONG(&_19$$16, cursorVar);
										ZEPHIR_INIT_NVAR(_20$$16);
										zephir_substr(_20$$16, item, 0 , zephir_get_intval(&_19$$16), 0);
										zephir_get_strval(_21$$16, _20$$16);
										ZEPHIR_CPY_WRT(variable, _21$$16);
										ZEPHIR_SINIT_NVAR(_22$$16);
										ZVAL_LONG(&_22$$16, (cursorVar + 1));
										ZEPHIR_INIT_NVAR(_23$$16);
										zephir_substr(_23$$16, item, zephir_get_intval(&_22$$16), 0, ZEPHIR_SUBSTR_NO_LENGTH);
										zephir_get_strval(_24$$16, _23$$16);
										ZEPHIR_CPY_WRT(regexp, _24$$16);
										break;
									}
								} else {
									notValid = 1;
									break;
								}
							}
							if (!(notValid)) {
								tmp = numberMatches;
								_25$$18 = zephir_is_true(variable);
								if (_25$$18) {
									_25$$18 = zephir_is_true(regexp);
								}
								if (_25$$18) {
									foundPattern = 0;
									for (_26$$19 = 0; _26$$19 < Z_STRLEN_P(regexp); _26$$19++) {
										ch = ZEPHIR_STRING_OFFSET(regexp, _26$$19);
										if (ch == '\0') {
											break;
										}
										if (!(foundPattern)) {
											if (ch == '(') {
												foundPattern = 1;
											}
										} else {
											if (ch == ')') {
												foundPattern = 2;
												break;
											}
										}
									}
									if (foundPattern != 2) {
										zephir_concat_self_str(&route, "(", sizeof("(")-1 TSRMLS_CC);
										zephir_concat_self(&route, regexp TSRMLS_CC);
										zephir_concat_self_str(&route, ")", sizeof(")")-1 TSRMLS_CC);
									} else {
										zephir_concat_self(&route, regexp TSRMLS_CC);
									}
									ZEPHIR_INIT_NVAR(_27$$19);
									ZVAL_LONG(_27$$19, tmp);
									zephir_array_update_zval(&matches, variable, &_27$$19, PH_COPY | PH_SEPARATE);
								} else {
									zephir_concat_self_str(&route, "([^/]*)", sizeof("([^/]*)")-1 TSRMLS_CC);
									ZEPHIR_INIT_NVAR(_28$$28);
									ZVAL_LONG(_28$$28, tmp);
									zephir_array_update_zval(&matches, item, &_28$$28, PH_COPY | PH_SEPARATE);
								}
							} else {
								ZEPHIR_INIT_LNVAR(_29$$29);
								ZEPHIR_CONCAT_SVS(_29$$29, "{", item, "}");
								zephir_concat_self(&route, _29$$29 TSRMLS_CC);
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
			zephir_concat_self_char(&route, ch TSRMLS_CC);
		}
	}
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(return_value, route);
	zephir_array_fast_append(return_value, matches);
	RETURN_MM();

}

/**
 * Reconfigure the route adding a new pattern and a set of paths
 *
 * @param string pattern
 * @param array paths
 */
PHP_METHOD(Test_Router_Route, reConfigure) {

	int ZEPHIR_LAST_CALL_STATUS, _0$$5;
	zval *pattern, *paths = NULL, *moduleName = NULL, *controllerName = NULL, *actionName = NULL, *parts = NULL, *routePaths = NULL, *realClassName = NULL, *namespaceName = NULL, *pcrePattern = NULL, *compiledPattern = NULL, *extracted = NULL, *_1$$10, *_2$$19, *_3$$19;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = ZEPHIR_GLOBAL(global_null);
	}


	if (Z_TYPE_P(pattern) != IS_STRING) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The pattern must be string", "test/router/route.zep", 270);
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
			ZEPHIR_INIT_VAR(parts);
			zephir_fast_explode_str(parts, SL("::"), paths, LONG_MAX TSRMLS_CC);
			do {
				_0$$5 = zephir_fast_count_int(parts TSRMLS_CC);
				if (_0$$5 == 3) {
					ZEPHIR_OBS_NVAR(moduleName);
					zephir_array_fetch_long(&moduleName, parts, 0, PH_NOISY, "test/router/route.zep", 286 TSRMLS_CC);
					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 1, PH_NOISY, "test/router/route.zep", 287 TSRMLS_CC);
					ZEPHIR_OBS_NVAR(actionName);
					zephir_array_fetch_long(&actionName, parts, 2, PH_NOISY, "test/router/route.zep", 288 TSRMLS_CC);
					break;
				}
				if (_0$$5 == 2) {
					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 0, PH_NOISY, "test/router/route.zep", 291 TSRMLS_CC);
					ZEPHIR_OBS_NVAR(actionName);
					zephir_array_fetch_long(&actionName, parts, 1, PH_NOISY, "test/router/route.zep", 292 TSRMLS_CC);
					break;
				}
				if (_0$$5 == 1) {
					ZEPHIR_OBS_NVAR(controllerName);
					zephir_array_fetch_long(&controllerName, parts, 0, PH_NOISY, "test/router/route.zep", 295 TSRMLS_CC);
					break;
				}
			} while(0);

			ZEPHIR_INIT_VAR(routePaths);
			array_init(routePaths);
			if (Z_TYPE_P(moduleName) != IS_NULL) {
				zephir_array_update_string(&routePaths, SL("module"), &moduleName, PH_COPY | PH_SEPARATE);
			}
			if (Z_TYPE_P(controllerName) != IS_NULL) {
				if (zephir_memnstr_str(controllerName, SL("\\"), "test/router/route.zep", 310)) {
					ZEPHIR_INIT_VAR(realClassName);
					zephir_get_class_ns(realClassName, controllerName, 0 TSRMLS_CC);
					ZEPHIR_INIT_VAR(namespaceName);
					zephir_get_ns_class(namespaceName, controllerName, 0 TSRMLS_CC);
					if (zephir_is_true(namespaceName)) {
						zephir_array_update_string(&routePaths, SL("namespace"), &namespaceName, PH_COPY | PH_SEPARATE);
					}
				} else {
					ZEPHIR_CPY_WRT(realClassName, controllerName);
				}
				ZEPHIR_INIT_VAR(_1$$10);
				zephir_uncamelize(_1$$10, realClassName, NULL  );
				zephir_array_update_string(&routePaths, SL("controller"), &_1$$10, PH_COPY | PH_SEPARATE);
			}
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
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_router_exception_ce, "The route contains invalid paths", "test/router/route.zep", 342);
		return;
	}
	if (!(zephir_start_with_str(pattern, SL("#")))) {
		if (zephir_memnstr_str(pattern, SL("{"), "test/router/route.zep", 348)) {
			ZEPHIR_CALL_METHOD(&extracted, this_ptr, "extractnamedparams", NULL, 0, pattern);
			zephir_check_call_status();
			ZEPHIR_OBS_VAR(pcrePattern);
			zephir_array_fetch_long(&pcrePattern, extracted, 0, PH_NOISY, "test/router/route.zep", 351 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_2$$19);
			zephir_array_fetch_long(&_3$$19, extracted, 1, PH_NOISY | PH_READONLY, "test/router/route.zep", 352 TSRMLS_CC);
			zephir_fast_array_merge(_2$$19, &(routePaths), &(_3$$19) TSRMLS_CC);
			ZEPHIR_CPY_WRT(routePaths, _2$$19);
		} else {
			ZEPHIR_CPY_WRT(pcrePattern, pattern);
		}
		ZEPHIR_CALL_METHOD(&compiledPattern, this_ptr, "compilepattern", NULL, 0, pcrePattern);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(compiledPattern, pattern);
	}
	zephir_update_property_this(this_ptr, SL("_pattern"), pattern TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_compiledPattern"), compiledPattern TSRMLS_CC);
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
 * @return Route
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
	zval *reversed = NULL, *path = NULL, *position = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(reversed);
	array_init(reversed);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_paths"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "test/router/route.zep", 478);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
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



	zephir_update_property_array(this_ptr, SL("_converters"), name, converter TSRMLS_CC);
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

