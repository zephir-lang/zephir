
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/concat.h"


/**
 * Call sites passing named arguments.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2824, stub, issue2824, stub_issue2824_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2824, typed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, untyped)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, positionalThenNamed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "1");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_update_string(&_2, SL("b"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2), &_0);
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, typedMixed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "1");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_update_string(&_2, SL("b"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2), &_0);
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, skipped)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_update_string(&_1, SL("b"), &_0, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "withDefault", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, skippedUntyped)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_update_string(&_1, SL("b"), &_0, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "withDefault", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, skippedCallable)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_update_string(&_1, SL("b"), &_0, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "withCallable", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, skippedObject)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_update_string(&_1, SL("b"), &_0, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "withObject", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, skippedInt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZVAL_LONG(&_0, 7);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_update_string(&_1, SL("b"), &_0, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "withInt", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, variadicExtras)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZVAL_LONG(&_2, 3);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0);
	zephir_array_update_string(&_3, SL("x"), &_2, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "variadic", Z_ARRVAL_P(&_3), &_0, &_1);
	ZEPHIR_INIT_NVAR(&_3);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, unknownName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "1");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("a"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("zz"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, overwrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "1");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2), &_0);
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, statement)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_CALL_CLASS_METHOD_NAMED(NULL, zephir_fcall_method, NULL, p, "pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2824, classStatic)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_ce, stub_issue2824pair_ce, NULL, "spair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, dynamicClass)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval className_zv, _1, _2, _3;
	zend_string *className = NULL;

	ZVAL_UNDEF(&className_zv);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&className_zv);
	ZVAL_STR_COPY(&className_zv, className);
	_0 = zephir_fetch_class(&className_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "1");
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 2, 0);
	zephir_array_update_string(&_3, SL("b"), &_1, PH_COPY);
	zephir_array_update_string(&_3, SL("a"), &_2, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_ce, _0, NULL, "spair", Z_ARRVAL_P(&_3));
	ZEPHIR_INIT_NVAR(&_3);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, dynamicMethod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zend_string *method = NULL;
	zval *p, p_sub, method_zv, _0, _1, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&method_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(p)
		Z_PARAM_STR(method)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	p = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&method_zv);
	ZVAL_STR_COPY(&method_zv, method);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_ZVAL_NAMED(zephir_fcall_method, NULL, p, &method_zv, Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, userFunction)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION_NAMED("Stub\\issue2824_pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, numberFormat)
{
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, " ");
	ZVAL_DOUBLE(&_1, 1234.567);
	ZVAL_LONG(&_2, 2);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 3, 0);
	zephir_array_update_string(&_3, SL("thousands_separator"), &_0, PH_COPY);
	zephir_array_update_string(&_3, SL("num"), &_1, PH_COPY);
	zephir_array_update_string(&_3, SL("decimals"), &_2, PH_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION_NAMED("number_format", Z_ARRVAL_P(&_3));
	ZEPHIR_INIT_NVAR(&_3);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, strposNamed)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "b");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "abc");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("needle"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("haystack"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION_NAMED("strpos", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, inArrayNamed)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval __$true, _1, _2;

	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 3, 0);
	zephir_array_update_string(&_1, SL("haystack"), &_0, PH_COPY);
	zephir_array_update_string(&_1, SL("strict"), &__$true, PH_COPY);
	zephir_array_update_string(&_1, SL("needle"), &_2, PH_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION_NAMED("in_array", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, pregMatchNamed)
{
	zval matches, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "/(b)/");
	ZEPHIR_OBSERVE_IF_UNDEF(&matches);
	ZEPHIR_MAKE_REF(&matches);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 3, 0);
	zephir_array_update_string(&_2, SL("subject"), &_0, PH_COPY);
	Z_TRY_ADDREF_P(&matches); zend_hash_str_update(Z_ARRVAL_P(&_2), SL("matches"), &matches);
	zephir_array_update_string(&_2, SL("pattern"), &_1, PH_COPY);
	ZEPHIR_CALL_FUNCTION_NAMED(NULL, "preg_match", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	ZEPHIR_UNREF_WRITE(&matches);
	zephir_check_call_status();
	RETURN_CCTOR(&matches);
}

PHP_METHOD(Stub_Issue2824, positionalByReference)
{
	zval result, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a=1");
	ZEPHIR_OBSERVE_IF_UNDEF(&result);
	ZEPHIR_MAKE_REF(&result);
	ZEPHIR_CALL_FUNCTION(NULL, "parse_str", NULL, 54, &_0, &result);
	ZEPHIR_UNREF(&result);
	zephir_check_call_status();
	RETURN_CCTOR(&result);
}

PHP_METHOD(Stub_Issue2824, namedArgumentReleased)
{
	zend_class_entry *_1, *_3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, probe, weak, released, _0, _2, _4;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&probe);
	ZVAL_UNDEF(&weak);
	ZVAL_UNDEF(&released);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZEND_TLS zephir_method_cache _zephir_call_cache[5];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, stub_issue2824pair_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&released);
	array_init(&released);
	ZEPHIR_INIT_VAR(&_0);
	object_init(&_0);
	ZEPHIR_CPY_WRT(&probe, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	_1 = zephir_fetch_class_str_ex(SL("WeakReference"), ZEND_FETCH_CLASS_AUTO);
	ZEPHIR_CALL_CE_STATIC_CACHED(&weak, _1, "create", &_zephir_call_cache[0], &probe);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_string(&_0, SL("value"), &probe, PH_COPY);
	ZEPHIR_CALL_FUNCTION_NAMED(NULL, "gettype", Z_ARRVAL_P(&_0));
	ZEPHIR_INIT_NVAR(&_0);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&probe);
	ZVAL_NULL(&probe);
	ZEPHIR_CALL_METHOD_CACHED(&_2, &weak, "get", &_zephir_call_cache[1]);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, Z_TYPE_P(&_2) == IS_NULL);
	zephir_array_append(&released, &_0, PH_SEPARATE, "stub/issue2824.zep", 143);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_0);
	object_init(&_0);
	ZEPHIR_CPY_WRT(&probe, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	_3 = zephir_fetch_class_str_ex(SL("WeakReference"), ZEND_FETCH_CLASS_AUTO);
	ZEPHIR_CALL_CE_STATIC_CACHED(&weak, _3, "create", &_zephir_call_cache[2], &probe);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_string(&_0, SL("o"), &probe, PH_COPY);
	ZEPHIR_CALL_CLASS_METHOD_NAMED(NULL, zephir_fcall_method, NULL, p, "withObject", Z_ARRVAL_P(&_0));
	ZEPHIR_INIT_NVAR(&_0);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&probe);
	ZVAL_NULL(&probe);
	ZEPHIR_CALL_METHOD_CACHED(&_4, &weak, "get", &_zephir_call_cache[4]);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, Z_TYPE_P(&_4) == IS_NULL);
	zephir_array_append(&released, &_0, PH_SEPARATE, "stub/issue2824.zep", 149);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&released);
}

PHP_METHOD(Stub_Issue2824, byReferenceLeftUnassigned)
{
	zval word, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;

	ZVAL_UNDEF(&word);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "%s");
	ZEPHIR_OBSERVE_IF_UNDEF(&word);
	ZEPHIR_MAKE_REF(&word);
	ZEPHIR_CALL_FUNCTION(NULL, "sscanf", &_2, 56, &_0, &_1, &word);
	ZEPHIR_UNREF(&word);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "abc");
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "%s");
	ZEPHIR_OBSERVE_IF_UNDEF(&word);
	ZEPHIR_MAKE_REF(&word);
	ZEPHIR_CALL_FUNCTION(NULL, "sscanf", &_2, 56, &_0, &_1, &word);
	ZEPHIR_UNREF(&word);
	zephir_check_call_status();
	RETURN_CCTOR(&word);
}

PHP_METHOD(Stub_Issue2824, construct)
{
	zval instance, _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&instance);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_issue2824ctor_ce);
	zephir_constructor_start(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "1");
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 2, 0);
	zephir_array_update_string(&_3, SL("b"), &_1, PH_COPY);
	zephir_array_update_string(&_3, SL("a"), &_2, PH_COPY);
	ZEPHIR_CALL_CLASS_METHOD_NAMED(NULL, zephir_fcall_method, NULL, &_0, "__construct", Z_ARRVAL_P(&_3));
	ZEPHIR_INIT_NVAR(&_3);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&instance, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_RETURN_CALL_METHOD_CACHED(&instance, "getArrayCopy", &_zephir_call_cache[1]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, closure)
{
	zval f, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&f);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&f);
	zephir_create_closure_ex(&f, NULL, stub_75__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION_NAMED(&f, Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824, callUserFunc)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *p, p_sub, _1, _2, _3;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &p);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, p);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "pair");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "2");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "1");
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 2, 0);
	zephir_array_update_string(&_3, SL("b"), &_1, PH_COPY);
	zephir_array_update_string(&_3, SL("a"), &_2, PH_COPY);
	ZEPHIR_CALL_USER_FUNC_ARGS(return_value, &_0, &_3);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_FUNCTION(f_Stub_issue2824_pair) {
	zval *a, a_sub, *b, b_sub;
		ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	ZEPHIR_CONCAT_VSV(return_value, a, ",", b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

