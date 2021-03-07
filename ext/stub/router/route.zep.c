
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


/**
 * Stub\Router\Route
 *
 * This class represents every route added to the router
 */
ZEPHIR_INIT_CLASS(Stub_Router_Route) {

	ZEPHIR_REGISTER_CLASS(Stub\\Router, Route, stub, router_route, stub_router_route_method_entry, 0);

	zend_declare_property_null(stub_router_route_ce, SL("_pattern"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_compiledPattern"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_paths"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_methods"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_hostname"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_converters"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_id"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_name"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_router_route_ce, SL("_beforeMatch"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

/**
 * Stub\Router\Route constructor
 *
 * @param string pattern
 * @param array paths
 * @param array|string httpMethods
 */
PHP_METHOD(Stub_Router_Route, __construct) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, *httpMethods = NULL, httpMethods_sub, __$null;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_UNDEF(&httpMethods_sub);
	ZVAL_NULL(&__$null);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_ZVAL(pattern)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(paths)
		Z_PARAM_ZVAL(httpMethods)
	ZEND_PARSE_PARAMETERS_END();

#endif


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


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "reconfigure", NULL, 0, pattern, paths);
	zephir_check_call_status();
	zephir_update_property_zval(this_ptr, ZEND_STRL("_methods"), httpMethods);
	ZEPHIR_MM_RESTORE();

}

/**
 * Replaces placeholders from pattern returning a valid PCRE regular expression
 *
 * @param string pattern
 * @return string
 */
PHP_METHOD(Stub_Router_Route, compilePattern) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *pattern = NULL, pattern_sub, idPattern, _0$$4, _1$$4, _2$$5, _3$$5, _4$$6, _5$$6, _6$$7, _7$$7, _8$$8, _9$$8, _10$$8, _11$$9, _12$$9, _13$$9;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&idPattern);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZVAL_UNDEF(&_4$$6);
	ZVAL_UNDEF(&_5$$6);
	ZVAL_UNDEF(&_6$$7);
	ZVAL_UNDEF(&_7$$7);
	ZVAL_UNDEF(&_8$$8);
	ZVAL_UNDEF(&_9$$8);
	ZVAL_UNDEF(&_10$$8);
	ZVAL_UNDEF(&_11$$9);
	ZVAL_UNDEF(&_12$$9);
	ZVAL_UNDEF(&_13$$9);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(pattern)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern);

	ZEPHIR_SEPARATE_PARAM(pattern);


	if (zephir_memnstr_str(pattern, SL(":"), "stub/router/route.zep", 56)) {
		ZEPHIR_INIT_VAR(&idPattern);
		ZVAL_STRING(&idPattern, "/([a-zA-Z0-9\\_\\-]+)");
		if (zephir_memnstr_str(pattern, SL("/:module"), "stub/router/route.zep", 62)) {
			ZEPHIR_INIT_VAR(&_0$$4);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "/:module");
			zephir_fast_str_replace(&_0$$4, &_1$$4, &idPattern, pattern);
			ZEPHIR_CPY_WRT(pattern, &_0$$4);
		}
		if (zephir_memnstr_str(pattern, SL("/:controller"), "stub/router/route.zep", 67)) {
			ZEPHIR_INIT_VAR(&_2$$5);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "/:controller");
			zephir_fast_str_replace(&_2$$5, &_3$$5, &idPattern, pattern);
			ZEPHIR_CPY_WRT(pattern, &_2$$5);
		}
		if (zephir_memnstr_str(pattern, SL("/:namespace"), "stub/router/route.zep", 72)) {
			ZEPHIR_INIT_VAR(&_4$$6);
			ZEPHIR_INIT_VAR(&_5$$6);
			ZVAL_STRING(&_5$$6, "/:namespace");
			zephir_fast_str_replace(&_4$$6, &_5$$6, &idPattern, pattern);
			ZEPHIR_CPY_WRT(pattern, &_4$$6);
		}
		if (zephir_memnstr_str(pattern, SL("/:action"), "stub/router/route.zep", 77)) {
			ZEPHIR_INIT_VAR(&_6$$7);
			ZEPHIR_INIT_VAR(&_7$$7);
			ZVAL_STRING(&_7$$7, "/:action");
			zephir_fast_str_replace(&_6$$7, &_7$$7, &idPattern, pattern);
			ZEPHIR_CPY_WRT(pattern, &_6$$7);
		}
		if (zephir_memnstr_str(pattern, SL("/:params"), "stub/router/route.zep", 82)) {
			ZEPHIR_INIT_VAR(&_8$$8);
			ZEPHIR_INIT_VAR(&_9$$8);
			ZVAL_STRING(&_9$$8, "/:params");
			ZEPHIR_INIT_VAR(&_10$$8);
			ZVAL_STRING(&_10$$8, "(/.*)*");
			zephir_fast_str_replace(&_8$$8, &_9$$8, &_10$$8, pattern);
			ZEPHIR_CPY_WRT(pattern, &_8$$8);
		}
		if (zephir_memnstr_str(pattern, SL("/:int"), "stub/router/route.zep", 87)) {
			ZEPHIR_INIT_VAR(&_11$$9);
			ZEPHIR_INIT_VAR(&_12$$9);
			ZVAL_STRING(&_12$$9, "/:int");
			ZEPHIR_INIT_VAR(&_13$$9);
			ZVAL_STRING(&_13$$9, "/([0-9]+)");
			zephir_fast_str_replace(&_11$$9, &_12$$9, &_13$$9, pattern);
			ZEPHIR_CPY_WRT(pattern, &_11$$9);
		}
	}
	if (zephir_memnstr_str(pattern, SL("("), "stub/router/route.zep", 93)) {
		ZEPHIR_CONCAT_SVS(return_value, "#^", pattern, "$#");
		RETURN_MM();
	}
	if (zephir_memnstr_str(pattern, SL("["), "stub/router/route.zep", 98)) {
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
 * @return Stub\Router\Route
 */
PHP_METHOD(Stub_Router_Route, via) {

	zval *httpMethods, httpMethods_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&httpMethods_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(httpMethods)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &httpMethods);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_methods"), httpMethods);
	RETURN_THISW();

}

/**
 * Extracts parameters from a string
 *
 * @param string pattern
 */
PHP_METHOD(Stub_Router_Route, extractNamedParams) {

	long _0, _5$$11, _26$$19;
	zend_bool notValid, _6$$12, _7$$12, _8$$12, _9$$12, _10$$12, _11$$12, _12$$12, _13$$12, _14$$12, _15$$12, _16$$12, _17$$12, _25$$18;
	zend_long tmp = 0, cursor = 0, cursorVar = 0, marker = 0, bracketCount, parenthesesCount, foundPattern, intermediate, numberMatches;
	char ch = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *pattern_param = NULL, matches, _1$$11, _2$$11, _3$$11, _18$$16, _19$$16, _20$$16, _22$$16, _23$$16, _27$$19, _28$$28;
	zval pattern, route, item, variable, regexp, _4$$11, _21$$16, _24$$16, _29$$29;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&route);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&variable);
	ZVAL_UNDEF(&regexp);
	ZVAL_UNDEF(&_4$$11);
	ZVAL_UNDEF(&_21$$16);
	ZVAL_UNDEF(&_24$$16);
	ZVAL_UNDEF(&_29$$29);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_1$$11);
	ZVAL_UNDEF(&_2$$11);
	ZVAL_UNDEF(&_3$$11);
	ZVAL_UNDEF(&_18$$16);
	ZVAL_UNDEF(&_19$$16);
	ZVAL_UNDEF(&_20$$16);
	ZVAL_UNDEF(&_22$$16);
	ZVAL_UNDEF(&_23$$16);
	ZVAL_UNDEF(&_27$$19);
	ZVAL_UNDEF(&_28$$28);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(pattern)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &pattern_param);

	zephir_get_strval(&pattern, pattern_param);


	notValid = 0;
	bracketCount = 0;
	parenthesesCount = 0;
	foundPattern = 0;
	intermediate = 0;
	numberMatches = 0;
	if (zephir_fast_strlen_ev(&pattern) <= 0) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_INIT_VAR(&matches);
	array_init(&matches);
	ZEPHIR_INIT_VAR(&route);
	ZVAL_STRING(&route, "");
	for (_0 = 0; _0 < Z_STRLEN_P(&pattern); _0++) {
		cursor = _0; 
		ch = ZEPHIR_STRING_OFFSET(&pattern, _0);
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
							ZEPHIR_INIT_NVAR(&variable);
							ZVAL_STRING(&variable, "");
							ZEPHIR_INIT_NVAR(&regexp);
							ZVAL_STRING(&regexp, "");
							ZVAL_LONG(&_1$$11, marker);
							ZVAL_LONG(&_2$$11, (cursor - marker));
							ZEPHIR_INIT_NVAR(&_3$$11);
							zephir_substr(&_3$$11, &pattern, zephir_get_intval(&_1$$11), zephir_get_intval(&_2$$11), 0);
							zephir_get_strval(&_4$$11, &_3$$11);
							ZEPHIR_CPY_WRT(&item, &_4$$11);
							for (_5$$11 = 0; _5$$11 < Z_STRLEN_P(&item); _5$$11++) {
								cursorVar = _5$$11; 
								ch = ZEPHIR_STRING_OFFSET(&item, _5$$11);
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
										ZVAL_LONG(&_18$$16, 0);
										ZVAL_LONG(&_19$$16, cursorVar);
										ZEPHIR_INIT_NVAR(&_20$$16);
										zephir_substr(&_20$$16, &item, 0 , zephir_get_intval(&_19$$16), 0);
										zephir_get_strval(&_21$$16, &_20$$16);
										ZEPHIR_CPY_WRT(&variable, &_21$$16);
										ZVAL_LONG(&_22$$16, (cursorVar + 1));
										ZEPHIR_INIT_NVAR(&_23$$16);
										zephir_substr(&_23$$16, &item, zephir_get_intval(&_22$$16), 0, ZEPHIR_SUBSTR_NO_LENGTH);
										zephir_get_strval(&_24$$16, &_23$$16);
										ZEPHIR_CPY_WRT(&regexp, &_24$$16);
										break;
									}
								} else {
									notValid = 1;
									break;
								}
							}
							if (!(notValid)) {
								tmp = numberMatches;
								_25$$18 = zephir_is_true(&variable);
								if (_25$$18) {
									_25$$18 = zephir_is_true(&regexp);
								}
								if (_25$$18) {
									foundPattern = 0;
									for (_26$$19 = 0; _26$$19 < Z_STRLEN_P(&regexp); _26$$19++) {
										ch = ZEPHIR_STRING_OFFSET(&regexp, _26$$19);
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
										zephir_concat_self_str(&route, "(", sizeof("(") - 1);
										zephir_concat_self(&route, &regexp);
										zephir_concat_self_str(&route, ")", sizeof(")") - 1);
									} else {
										zephir_concat_self(&route, &regexp);
									}
									ZEPHIR_INIT_NVAR(&_27$$19);
									ZVAL_LONG(&_27$$19, tmp);
									zephir_array_update_zval(&matches, &variable, &_27$$19, PH_COPY | PH_SEPARATE);
								} else {
									zephir_concat_self_str(&route, "([^/]*)", sizeof("([^/]*)") - 1);
									ZEPHIR_INIT_NVAR(&_28$$28);
									ZVAL_LONG(&_28$$28, tmp);
									zephir_array_update_zval(&matches, &item, &_28$$28, PH_COPY | PH_SEPARATE);
								}
							} else {
								ZEPHIR_INIT_NVAR(&_29$$29);
								ZEPHIR_CONCAT_SVS(&_29$$29, "{", &item, "}");
								zephir_concat_self(&route, &_29$$29);
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
			zephir_concat_self_char(&route, ch);
		}
	}
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &route);
	zephir_array_fast_append(return_value, &matches);
	RETURN_MM();

}

/**
 * Reconfigure the route adding a new pattern and a set of paths
 *
 * @param string pattern
 * @param array paths
 */
PHP_METHOD(Stub_Router_Route, reConfigure) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _0$$5;
	zval *pattern, pattern_sub, *paths = NULL, paths_sub, __$null, moduleName, controllerName, actionName, parts, routePaths, realClassName, namespaceName, pcrePattern, compiledPattern, extracted, _1$$10, _2$$19, _3$$19;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&paths_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&moduleName);
	ZVAL_UNDEF(&controllerName);
	ZVAL_UNDEF(&actionName);
	ZVAL_UNDEF(&parts);
	ZVAL_UNDEF(&routePaths);
	ZVAL_UNDEF(&realClassName);
	ZVAL_UNDEF(&namespaceName);
	ZVAL_UNDEF(&pcrePattern);
	ZVAL_UNDEF(&compiledPattern);
	ZVAL_UNDEF(&extracted);
	ZVAL_UNDEF(&_1$$10);
	ZVAL_UNDEF(&_2$$19);
	ZVAL_UNDEF(&_3$$19);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(pattern)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(paths)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &paths_sub;
		paths = &__$null;
	}


	if (Z_TYPE_P(pattern) != IS_STRING) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(stub_router_exception_ce, "The pattern must be string", "stub/router/route.zep", 270);
		return;
	}
	if (Z_TYPE_P(paths) != IS_NULL) {
		if (Z_TYPE_P(paths) == IS_STRING) {
			ZEPHIR_INIT_VAR(&moduleName);
			ZVAL_NULL(&moduleName);
			ZEPHIR_INIT_VAR(&controllerName);
			ZVAL_NULL(&controllerName);
			ZEPHIR_INIT_VAR(&actionName);
			ZVAL_NULL(&actionName);
			ZEPHIR_INIT_VAR(&parts);
			zephir_fast_explode_str(&parts, SL("::"), paths, LONG_MAX);
			do {
				_0$$5 = zephir_fast_count_int(&parts);
				if (_0$$5 == 3) {
					ZEPHIR_OBS_NVAR(&moduleName);
					zephir_array_fetch_long(&moduleName, &parts, 0, PH_NOISY, "stub/router/route.zep", 286);
					ZEPHIR_OBS_NVAR(&controllerName);
					zephir_array_fetch_long(&controllerName, &parts, 1, PH_NOISY, "stub/router/route.zep", 287);
					ZEPHIR_OBS_NVAR(&actionName);
					zephir_array_fetch_long(&actionName, &parts, 2, PH_NOISY, "stub/router/route.zep", 288);
					break;
				}
				if (_0$$5 == 2) {
					ZEPHIR_OBS_NVAR(&controllerName);
					zephir_array_fetch_long(&controllerName, &parts, 0, PH_NOISY, "stub/router/route.zep", 291);
					ZEPHIR_OBS_NVAR(&actionName);
					zephir_array_fetch_long(&actionName, &parts, 1, PH_NOISY, "stub/router/route.zep", 292);
					break;
				}
				if (_0$$5 == 1) {
					ZEPHIR_OBS_NVAR(&controllerName);
					zephir_array_fetch_long(&controllerName, &parts, 0, PH_NOISY, "stub/router/route.zep", 295);
					break;
				}
			} while(0);

			ZEPHIR_INIT_VAR(&routePaths);
			array_init(&routePaths);
			if (Z_TYPE_P(&moduleName) != IS_NULL) {
				zephir_array_update_string(&routePaths, SL("module"), &moduleName, PH_COPY | PH_SEPARATE);
			}
			if (Z_TYPE_P(&controllerName) != IS_NULL) {
				if (zephir_memnstr_str(&controllerName, SL("\\"), "stub/router/route.zep", 310)) {
					ZEPHIR_INIT_VAR(&realClassName);
					zephir_get_class_ns(&realClassName, &controllerName, 0);
					ZEPHIR_INIT_VAR(&namespaceName);
					zephir_get_ns_class(&namespaceName, &controllerName, 0);
					if (zephir_is_true(&namespaceName)) {
						zephir_array_update_string(&routePaths, SL("namespace"), &namespaceName, PH_COPY | PH_SEPARATE);
					}
				} else {
					ZEPHIR_CPY_WRT(&realClassName, &controllerName);
				}
				ZEPHIR_INIT_VAR(&_1$$10);
				zephir_uncamelize(&_1$$10, &realClassName, NULL  );
				zephir_array_update_string(&routePaths, SL("controller"), &_1$$10, PH_COPY | PH_SEPARATE);
			}
			if (Z_TYPE_P(&actionName) != IS_NULL) {
				zephir_array_update_string(&routePaths, SL("action"), &actionName, PH_COPY | PH_SEPARATE);
			}
		} else {
			ZEPHIR_CPY_WRT(&routePaths, paths);
		}
	} else {
		ZEPHIR_INIT_NVAR(&routePaths);
		array_init(&routePaths);
	}
	if (Z_TYPE_P(&routePaths) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(stub_router_exception_ce, "The route contains invalid paths", "stub/router/route.zep", 342);
		return;
	}
	if (!(zephir_start_with_str(pattern, SL("#")))) {
		if (zephir_memnstr_str(pattern, SL("{"), "stub/router/route.zep", 348)) {
			ZEPHIR_CALL_METHOD(&extracted, this_ptr, "extractnamedparams", NULL, 0, pattern);
			zephir_check_call_status();
			ZEPHIR_OBS_VAR(&pcrePattern);
			zephir_array_fetch_long(&pcrePattern, &extracted, 0, PH_NOISY, "stub/router/route.zep", 351);
			ZEPHIR_INIT_VAR(&_2$$19);
			zephir_array_fetch_long(&_3$$19, &extracted, 1, PH_NOISY | PH_READONLY, "stub/router/route.zep", 352);
			zephir_fast_array_merge(&_2$$19, &routePaths, &_3$$19);
			ZEPHIR_CPY_WRT(&routePaths, &_2$$19);
		} else {
			ZEPHIR_CPY_WRT(&pcrePattern, pattern);
		}
		ZEPHIR_CALL_METHOD(&compiledPattern, this_ptr, "compilepattern", NULL, 0, &pcrePattern);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(&compiledPattern, pattern);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("_pattern"), pattern);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_compiledPattern"), &compiledPattern);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_paths"), &routePaths);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the route's name
 *
 * @return string
 */
PHP_METHOD(Stub_Router_Route, getName) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_name");

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
PHP_METHOD(Stub_Router_Route, setName) {

	zval *name, name_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &name);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_name"), name);
	RETURN_THISW();

}

/**
 * Sets a callback that is called if the route is matched.
 * The developer can implement any arbitrary conditions here
 * If the callback returns false the route is treaded as not matched
 *
 * @param callback callback
 * @return Stub\Router\Route
 */
PHP_METHOD(Stub_Router_Route, beforeMatch) {

	zval *callback, callback_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&callback_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(callback)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &callback);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_beforeMatch"), callback);
	RETURN_THISW();

}

/**
 * Returns the 'before match' callback if any
 *
 * @return mixed
 */
PHP_METHOD(Stub_Router_Route, getBeforeMatch) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_beforeMatch");

}

/**
 * Returns the route's id
 *
 * @return string
 */
PHP_METHOD(Stub_Router_Route, getRouteId) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_id");

}

/**
 * Returns the route's pattern
 *
 * @return string
 */
PHP_METHOD(Stub_Router_Route, getPattern) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_pattern");

}

/**
 * Returns the route's compiled pattern
 *
 * @return string
 */
PHP_METHOD(Stub_Router_Route, getCompiledPattern) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_compiledPattern");

}

/**
 * Returns the paths
 *
 * @return array
 */
PHP_METHOD(Stub_Router_Route, getPaths) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_paths");

}

/**
 * Returns the paths using positions as keys and names as values
 *
 * @return array
 */
PHP_METHOD(Stub_Router_Route, getReversedPaths) {

	zend_string *_4;
	zend_ulong _3;
	zval reversed, path, position, _0, *_1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&reversed);
	ZVAL_UNDEF(&path);
	ZVAL_UNDEF(&position);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&reversed);
	array_init(&reversed);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("_paths"), PH_NOISY_CC | PH_READONLY);
	zephir_is_iterable(&_0, 0, "stub/router/route.zep", 478);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&_0), _3, _4, _1)
		{
			ZEPHIR_INIT_NVAR(&path);
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&path, _4);
			} else {
				ZVAL_LONG(&path, _3);
			}
			ZEPHIR_INIT_NVAR(&position);
			ZVAL_COPY(&position, _1);
			zephir_array_update_zval(&reversed, &position, &path, PH_COPY | PH_SEPARATE);
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
			ZEPHIR_CALL_METHOD(&path, &_0, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&position, &_0, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_update_zval(&reversed, &position, &path, PH_COPY | PH_SEPARATE);
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&position);
	ZEPHIR_INIT_NVAR(&path);
	RETURN_CCTOR(&reversed);

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
 * @return Stub\Router\Route
 */
PHP_METHOD(Stub_Router_Route, setHttpMethods) {

	zval *httpMethods, httpMethods_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&httpMethods_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(httpMethods)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &httpMethods);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_methods"), httpMethods);
	RETURN_THISW();

}

/**
 * Returns the HTTP methods that constraint matching the route
 *
 * @return string|array
 */
PHP_METHOD(Stub_Router_Route, getHttpMethods) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_methods");

}

/**
 * Sets a hostname restriction to the route
 *
 *<code>
 * $route->setHostname('localhost');
 *</code>
 *
 * @param string|array httpMethods
 * @return Stub\Router\Route
 */
PHP_METHOD(Stub_Router_Route, setHostname) {

	zval *hostname, hostname_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&hostname_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(hostname)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &hostname);



	zephir_update_property_zval(this_ptr, ZEND_STRL("_hostname"), hostname);
	RETURN_THISW();

}

/**
 * Returns the hostname restriction if any
 *
 * @return string
 */
PHP_METHOD(Stub_Router_Route, getHostname) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_hostname");

}

/**
 * Adds a converter to perform an additional transformation for certain parameter
 *
 * @param string name
 * @param callable converter
 * @return Stub\Router\Route
 */
PHP_METHOD(Stub_Router_Route, convert) {

	zval *name, name_sub, *converter, converter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&converter_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(converter)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(2, 0, &name, &converter);



	zephir_update_property_array(this_ptr, SL("_converters"), name, converter);
	RETURN_THISW();

}

/**
 * Returns the router converter
 *
 * @return array
 */
PHP_METHOD(Stub_Router_Route, getConverters) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "_converters");

}

