
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/variables.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Vars)
{
	ZEPHIR_REGISTER_CLASS(Stub, Vars, stub, vars, stub_vars_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Vars, testParam)
{
	zend_bool _9;
	zend_string *_5;
	zend_ulong _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_7 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *config_param = NULL, k, v, _0, *_1, _2, *_3, _8, _6$$3, _10$$4;
	zval config;

	ZVAL_UNDEF(&config);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_10$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(config, config_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &config_param);
	ZEPHIR_OBS_COPY_OR_DUP(&config, config_param);
	zephir_array_fetch_string(&_0, &config, SL("dir"), PH_NOISY | PH_READONLY, "stub/vars.zep", 8);
	if (Z_TYPE_P(&_0) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_2);
		zephir_string_to_char_array(&_2, &_0);
		_1 = &_2;
	} else {
		_1 = &_0;
	}
	zephir_is_iterable(_1, 0, "stub/vars.zep", 11);
	if (Z_TYPE_P(_1) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_1), _4, _5, _3)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_5 != NULL) { 
				ZVAL_STR_COPY(&k, _5);
			} else {
				ZVAL_LONG(&k, _4);
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _3);
			ZEPHIR_CALL_FUNCTION(&_6$$3, "realpath", &_7, 82, &v);
			zephir_check_call_status();
			zephir_array_update_multi(&config, &_6$$3, SL("sz"), 3, SL("dir"), &k);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _1, "rewind", NULL, 0);
		zephir_check_call_status();
		_9 = 1;
		while (1) {
			if (_9) {
				_9 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _1, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_8, _1, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_8)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, _1, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, _1, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CALL_FUNCTION(&_10$$4, "realpath", &_7, 82, &v);
				zephir_check_call_status();
				zephir_array_update_multi(&config, &_10$$4, SL("sz"), 3, SL("dir"), &k);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
	RETURN_CTOR(&config);
}

PHP_METHOD(Stub_Vars, testVarDump)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, a, ar, _0;

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

PHP_METHOD(Stub_Vars, testVarDump2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *ret, ret_sub, _0;

	ZVAL_UNDEF(&ret_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ret)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &ret);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_LONG(ret, 1));
	zephir_var_dump(&_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Vars, testVarExport)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, a, ar, ret, _0, _1, _2;

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

PHP_METHOD(Stub_Vars, test88Issue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval param1_zv, param2_zv, _0, _1, _2, _3;
	zend_string *param1 = NULL, *param2 = NULL;

	ZVAL_UNDEF(&param1_zv);
	ZVAL_UNDEF(&param2_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(param1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(param2)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&param1_zv);
	ZVAL_STR_COPY(&param1_zv, param1);
	if (!param2) {
		ZEPHIR_INIT_VAR(&param2_zv);
	} else {
		zephir_memory_observe(&param2_zv);
	ZVAL_STR_COPY(&param2_zv, param2);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CPY_WRT(&_0, &param1_zv);
	zephir_var_dump(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CPY_WRT(&_1, &param2_zv);
	zephir_var_dump(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZEPHIR_CPY_WRT(&_2, &param1_zv);
	zephir_var_export(&_2);
	ZEPHIR_INIT_VAR(&_3);
	ZEPHIR_CPY_WRT(&_3, &param2_zv);
	zephir_var_export(&_3);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Vars, test88IssueParam2InitString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval param1_zv, param2_zv, _0;
	zend_string *param1 = NULL, *param2 = NULL;

	ZVAL_UNDEF(&param1_zv);
	ZVAL_UNDEF(&param2_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(param1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(param2)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&param1_zv);
	ZVAL_STR_COPY(&param1_zv, param1);
	if (!param2) {
		param2 = zend_string_init(ZEND_STRL("test string"), 0);
		zephir_memory_observe(&param2_zv);
		ZVAL_STR(&param2_zv, param2);
	} else {
		zephir_memory_observe(&param2_zv);
	ZVAL_STR_COPY(&param2_zv, param2);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CPY_WRT(&_0, &param2_zv);
	zephir_var_export(&_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Vars, testVarDump2param)
{
	zval *p1, p1_sub, *p2, p2_sub;

	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(p1)
		Z_PARAM_ZVAL(p2)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &p1, &p2);
	zephir_var_dump(p1);
	zephir_var_dump(p2);
}

PHP_METHOD(Stub_Vars, testVarDump3param)
{
	zval *p1, p1_sub, *p2, p2_sub, *p3, p3_sub;

	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);
	ZVAL_UNDEF(&p3_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(p1)
		Z_PARAM_ZVAL(p2)
		Z_PARAM_ZVAL(p3)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &p1, &p2, &p3);
	zephir_var_dump(p1);
	zephir_var_dump(p2);
	zephir_var_dump(p3);
}

PHP_METHOD(Stub_Vars, testCountOptimizerVarDumpAndExport)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *testVar, testVar_sub, _0, _1;

	ZVAL_UNDEF(&testVar_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(testVar)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
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

PHP_METHOD(Stub_Vars, testArrayTypeVarDumpAndExport)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *testVar_param = NULL, _0, _1;
	zval testVar;

	ZVAL_UNDEF(&testVar);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		ZEPHIR_Z_PARAM_ARRAY(testVar, testVar_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
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
 * @link https://github.com/zephir-lang/zephir/issues/681
 */
PHP_METHOD(Stub_Vars, testIntVarDump)
{
	zval _0, _1;
	zend_long a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

PHP_METHOD(Stub_Vars, testDoubleVarDump)
{
	zval _0, _1;
	double a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

PHP_METHOD(Stub_Vars, testBoolVarDump)
{
	zval _0, _1;
	zend_bool a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

PHP_METHOD(Stub_Vars, testGetDefinedVars)
{
	double pi;
	zend_long a;
	a = 1;
	pi =  (3.14);
	zephir_get_defined_vars(return_value);
	return;
}

