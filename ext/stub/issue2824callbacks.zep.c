
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/generator.h"


/**
 * A callback is resolved where it is written, so a private method of the
 * class is callable from any of its methods.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824Callbacks)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2824Callbacks, stub, issue2824callbacks, stub_issue2824callbacks_method_entry, 0);

	zephir_init_internal_frame(&zephir_frame_zep_Stub_Issue2824Callbacks_viaInternal, stub_issue2824callbacks_ce, SL("viaInternal"));
	zephir_init_internal_frame(&zephir_frame_zep_Stub_Issue2824Callbacks_viaInternalFunc, stub_issue2824callbacks_ce, SL("viaInternalFunc"));
	return SUCCESS;
}

zend_internal_function zephir_frame_zep_Stub_Issue2824Callbacks_viaInternal;
zend_internal_function zephir_frame_zep_Stub_Issue2824Callbacks_viaInternalFunc;

PHP_METHOD(Stub_Issue2824Callbacks, secret)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *b = NULL, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 1, &a, &b);
	if (!b) {
		b = &b_sub;
		ZEPHIR_INIT_VAR(b);
		ZVAL_STRING(b, "");
	}
	ZEPHIR_CONCAT_SVV(return_value, "secret:", a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

void zep_Stub_Issue2824Callbacks_viaInternal(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used)
{
	zval _1;
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "secret");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "x");
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_0, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

void zep_Stub_Issue2824Callbacks_viaInternalFunc(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "secret");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "y");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_CALL_USER_FUNC_ARGS(return_value, &_0, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, fromInternal)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_INTERNAL_METHOD_P0(&_0, this_ptr, stub_issue2824callbacks_ce, zep_Stub_Issue2824Callbacks_viaInternal);
	zephir_check_call_status();
	ZEPHIR_CALL_INTERNAL_METHOD_P0(&_1, this_ptr, stub_issue2824callbacks_ce, zep_Stub_Issue2824Callbacks_viaInternalFunc);
	zephir_check_call_status();
	ZEPHIR_CONCAT_VSV(return_value, &_0, "|", &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, fromMethod)
{
	zval _1, _2, _3;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "secret");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "z");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "!");
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 2, 0);
	zephir_array_fast_append(&_3, &_1);
	zephir_array_fast_append(&_3, &_2);
	ZEPHIR_CALL_USER_FUNC_ARGS(return_value, &_0, &_3);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, fromMethodArray)
{
	zval _1;
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "secret");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "w");
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "?");
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_0, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, fromMethodNamed)
{
	zval _1;
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "secret");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	add_assoc_stringl_ex(&_2, SL("b"), SL("2"));
	add_assoc_stringl_ex(&_2, SL("a"), SL("1"));
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_0, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, fromGenerator)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_issue2824callbacks_ce, zep_Stub_Issue2824Callbacks_zephir_gen_step_fromGenerator, 0);
	return;
}

PHP_METHOD(Stub_Issue2824Callbacks, notCallable)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub;

	ZVAL_UNDEF(&f_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(f)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &f);
	ZEPHIR_CALL_USER_FUNC(return_value, f);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, notCallableWithArgs)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub, _0, _1;

	ZVAL_UNDEF(&f_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(f)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &f);
	ZVAL_LONG(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_CALL_USER_FUNC_ARGS(return_value, f, &_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, notCallableArray)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub, _0;

	ZVAL_UNDEF(&f_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(f)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &f);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, f, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Callbacks, notArray)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub, *args, args_sub;

	ZVAL_UNDEF(&f_sub);
	ZVAL_UNDEF(&args_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(f)
		Z_PARAM_ZVAL(args)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &f, &args);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, f, args);
	zephir_check_call_status();
	RETURN_MM();
}

void zep_Stub_Issue2824Callbacks_zephir_gen_step_fromGenerator(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, _0, _2, _3;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 4);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_0);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_1);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &_2);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &_3);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 2, 0);
	zephir_array_fast_append(&_1, this_ptr);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "secret");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "g");
	ZEPHIR_INIT_NVAR(&_3);
	zephir_create_array(&_3, 1, 0);
	zephir_array_fast_append(&_3, &_2);
	ZEPHIR_CALL_USER_FUNC_ARGS(&_0, &_1, &_3);
	zephir_check_call_status();
	zephir_generator_yield(zephir_gen, NULL, &_0, 1);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

