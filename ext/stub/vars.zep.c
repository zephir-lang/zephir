
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/variables.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Vars) {

	ZEPHIR_REGISTER_CLASS(Stub, Vars, stub, vars, stub_vars_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Vars, testParam) {

	zend_string *_4;
	zend_ulong _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_6 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *config_param = NULL, k, v, _0, *_1, _2, _5$$3, _7$$4;
	zval config;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&config);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_7$$4);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(config)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config_param);

	ZEPHIR_OBS_COPY_OR_DUP(&config, config_param);


	zephir_array_fetch_string(&_0, &config, SL("dir"), PH_NOISY | PH_READONLY, "stub/vars.zep", 8);
	zephir_is_iterable(&_0, 0, "stub/vars.zep", 11);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&_0), _3, _4, _1)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&k, _4);
			} else {
				ZVAL_LONG(&k, _3);
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _1);
			ZEPHIR_CALL_FUNCTION(&_5$$3, "realpath", &_6, 59, &v);
			zephir_check_call_status();
			zephir_array_update_multi(&config, &_5$$3, SL("sz"), 3, SL("dir"), &k);
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
			ZEPHIR_CALL_METHOD(&k, &_0, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CALL_FUNCTION(&_7$$4, "realpath", &_6, 59, &v);
				zephir_check_call_status();
				zephir_array_update_multi(&config, &_7$$4, SL("sz"), 3, SL("dir"), &k);
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
	RETURN_CTOR(&config);

}

PHP_METHOD(Stub_Vars, testVarDump) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, a, ar, _0;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "hello");
	ZEPHIR_INIT_VAR(&ar);
	zephir_create_array(&ar, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&ar, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "world");
	zephir_array_fast_append(&ar, &_0);
	zephir_array_fast_append(&ar, &__$false);
	zephir_var_dump(&ar);
	zephir_var_dump(&a);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testVarDump2) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *ret, ret_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ret)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &ret);



	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_LONG(ret, 1));
	zephir_var_dump(&_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testVarExport) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, a, ar, ret, _0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "hello");
	ZEPHIR_INIT_VAR(&ar);
	zephir_create_array(&ar, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&ar, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "world");
	zephir_array_fast_append(&ar, &_0);
	zephir_array_fast_append(&ar, &__$false);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CPY_WRT(&_1, &ar);
	zephir_var_export(&_1);
	ZEPHIR_INIT_VAR(&ret);
	zephir_var_export_ex(&ret, &ar);
	ZEPHIR_INIT_VAR(&_2);
	ZEPHIR_CPY_WRT(&_2, &a);
	zephir_var_export(&_2);
	ZEPHIR_INIT_NVAR(&ret);
	zephir_var_export_ex(&ret, &a);
	RETURN_CCTOR(&ret);

}

PHP_METHOD(Stub_Vars, test88Issue) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *param1_param = NULL, *param2_param = NULL, _0, _1, _2, _3;
	zval param1, param2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param1);
	ZVAL_UNDEF(&param2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(param1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(param2)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &param1_param, &param2_param);

	if (UNEXPECTED(Z_TYPE_P(param1_param) != IS_STRING && Z_TYPE_P(param1_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param1' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param1_param) == IS_STRING)) {
		zephir_get_strval(&param1, param1_param);
	} else {
		ZEPHIR_INIT_VAR(&param1);
		ZVAL_EMPTY_STRING(&param1);
	}
	if (!param2_param) {
		ZEPHIR_INIT_VAR(&param2);
		ZVAL_STRING(&param2, "");
	} else {
		zephir_get_strval(&param2, param2_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CPY_WRT(&_0, &param1);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CPY_WRT(&_1, &param2);
	zephir_var_dump(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZEPHIR_CPY_WRT(&_2, &param1);
	zephir_var_export(&_2);
	ZEPHIR_INIT_VAR(&_3);
	ZEPHIR_CPY_WRT(&_3, &param2);
	zephir_var_export(&_3);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, test88IssueParam2InitString) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *param1_param = NULL, *param2_param = NULL, _0;
	zval param1, param2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param1);
	ZVAL_UNDEF(&param2);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(param1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(param2)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &param1_param, &param2_param);

	if (UNEXPECTED(Z_TYPE_P(param1_param) != IS_STRING && Z_TYPE_P(param1_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param1' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param1_param) == IS_STRING)) {
		zephir_get_strval(&param1, param1_param);
	} else {
		ZEPHIR_INIT_VAR(&param1);
		ZVAL_EMPTY_STRING(&param1);
	}
	if (!param2_param) {
		ZEPHIR_INIT_VAR(&param2);
		ZVAL_STRING(&param2, "test string");
	} else {
		zephir_get_strval(&param2, param2_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CPY_WRT(&_0, &param2);
	zephir_var_export(&_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testVarDump2param) {

	zval *p1, p1_sub, *p2, p2_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(p1)
		Z_PARAM_ZVAL(p2)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(2, 0, &p1, &p2);



	zephir_var_dump(p1);
	zephir_var_dump(p2);

}

PHP_METHOD(Stub_Vars, testVarDump3param) {

	zval *p1, p1_sub, *p2, p2_sub, *p3, p3_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);
	ZVAL_UNDEF(&p3_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(p1)
		Z_PARAM_ZVAL(p2)
		Z_PARAM_ZVAL(p3)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(3, 0, &p1, &p2, &p3);



	zephir_var_dump(p1);
	zephir_var_dump(p2);
	zephir_var_dump(p3);

}

PHP_METHOD(Stub_Vars, testCountOptimizerVarDumpAndExport) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *testVar, testVar_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(testVar)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &testVar);



	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_fast_count_int(testVar));
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, zephir_fast_count_int(testVar));
	zephir_var_export(&_1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testArrayTypeVarDumpAndExport) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *testVar_param = NULL, _0, _1;
	zval testVar;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(testVar)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &testVar_param);

	if (!testVar_param) {
		ZEPHIR_INIT_VAR(&testVar);
		array_init(&testVar);
	} else {
		zephir_get_arrval(&testVar, testVar_param);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CPY_WRT(&_0, &testVar);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CPY_WRT(&_1, &testVar);
	zephir_var_export(&_1);
	ZEPHIR_MM_RESTORE();

}

/**
 * @link https://github.com/phalcon/zephir/issues/681
 */
PHP_METHOD(Stub_Vars, testIntVarDump) {

	zval _0, _1;
	zend_long a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	a = 1;
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, a);
	zephir_var_export(&_1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testDoubleVarDump) {

	zval _0, _1;
	double a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	a = (double) (1);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, a);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, a);
	zephir_var_export(&_1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testBoolVarDump) {

	zval _0, _1;
	zend_bool a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	a = 1;
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_BOOL(&_1, a);
	zephir_var_export(&_1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Vars, testGetDefinedVars) {

	double pi;
	zend_long a;
	zval *this_ptr = getThis();




	a = 1;
	pi = 3.14;
	zephir_get_defined_vars(return_value);
	return;

}

