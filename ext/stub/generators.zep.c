
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
#include "kernel/generator.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"


/**
 * Generator methods (`yield`) — issue #1849.
 * Exercised by tests/Extension/GeneratorTest.php.
 */
ZEPHIR_INIT_CLASS(Stub_Generators)
{
	ZEPHIR_REGISTER_CLASS(Stub, Generators, stub, generators, stub_generators_method_entry, 0);

	zend_declare_property_long(stub_generators_ce, SL("sideEffects"), 0, ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_generators_ce, SL("items"), ZEND_ACC_PUBLIC);
	stub_generators_ce->create_object = zephir_init_properties_Stub_Generators;

	return SUCCESS;
}

PHP_METHOD(Stub_Generators, countTo)
{
	zval *n_param = NULL;
	zend_long n;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_countTo, 1);
	zephir_generator_slot_set_long(return_value, 0, (zend_long) n);
	return;
}

PHP_METHOD(Stub_Generators, keysMix)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_keysMix, 0);
	return;
}

PHP_METHOD(Stub_Generators, bare)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_bare, 0);
	return;
}

PHP_METHOD(Stub_Generators, withReturn)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_withReturn, 0);
	return;
}

PHP_METHOD(Stub_Generators, three)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_three, 0);
	return;
}

PHP_METHOD(Stub_Generators, boom)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_boom, 0);
	return;
}

PHP_METHOD(Stub_Generators, seq)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval label;
	zend_bool tail;
	double step;
	zval *n_param = NULL, *step_param = NULL, *tail_param = NULL, *label_param = NULL;
	zend_long n;

	ZVAL_UNDEF(&label);
	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_LONG(n)
		Z_PARAM_ZVAL(step_param)
		Z_PARAM_BOOL(tail)
		Z_PARAM_ZVAL(label_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 4, 0, &n_param, &step_param, &tail_param, &label_param);
	step = zephir_get_doubleval(step_param);
	zephir_get_strval(&label, label_param);
	zephir_generator_create(return_value, NULL, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_seq, 4);
	zephir_generator_slot_set_long(return_value, 0, (zend_long) n);
	zephir_generator_slot_set_double(return_value, 1, step);
	zephir_generator_slot_set_bool(return_value, 2, (int) tail);
	zephir_generator_slot_set(return_value, 3, &label);
	ZEPHIR_MM_RESTORE();
	return;
}

PHP_METHOD(Stub_Generators, arr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *items_param = NULL;
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(items, items_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &items_param);
	zephir_get_arrval(&items, items_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_arr, 1);
	zephir_generator_slot_set(return_value, 0, &items);
	ZEPHIR_MM_RESTORE();
	return;
}

PHP_METHOD(Stub_Generators, arrReverse)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *items_param = NULL;
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(items, items_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &items_param);
	zephir_get_arrval(&items, items_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_arrReverse, 1);
	zephir_generator_slot_set(return_value, 0, &items);
	ZEPHIR_MM_RESTORE();
	return;
}

PHP_METHOD(Stub_Generators, nested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *matrix_param = NULL;
	zval matrix;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&matrix);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(matrix, matrix_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &matrix_param);
	zephir_get_arrval(&matrix, matrix_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_nested, 1);
	zephir_generator_slot_set(return_value, 0, &matrix);
	ZEPHIR_MM_RESTORE();
	return;
}

PHP_METHOD(Stub_Generators, overItems)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_overItems, 0);
	return;
}

PHP_METHOD(Stub_Generators, range4)
{
	zval *n_param = NULL;
	zend_long n;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_range4, 1);
	zephir_generator_slot_set_long(return_value, 0, (zend_long) n);
	return;
}

PHP_METHOD(Stub_Generators, viaSwitch)
{
	zval *mode_param = NULL;
	zend_long mode;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &mode_param);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_viaSwitch, 1);
	zephir_generator_slot_set_long(return_value, 0, (zend_long) mode);
	return;
}

PHP_METHOD(Stub_Generators, relay)
{
	zval *it, it_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&it_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(it, zend_ce_iterator)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &it);
	zephir_generator_create(return_value, this_ptr, stub_generators_ce, zep_Stub_Generators_zephir_gen_step_relay, 1);
	zephir_generator_slot_set(return_value, 0, it);
	return;
}

PHP_METHOD(Stub_Generators, consume)
{
	zend_bool _4;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *it, it_sub, v, *_0, _1, *_2, _3;

	ZVAL_UNDEF(&it_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(it, zend_ce_iterator)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &it);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	if (Z_TYPE_P(it) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, it);
		_0 = &_1;
	} else {
		_0 = it;
	}
	zephir_is_iterable(_0, 0, "stub/generators.zep", 139);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _2);
			zephir_array_append(&result, &v, PH_SEPARATE, "stub/generators.zep", 137);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_4 = 1;
		while (1) {
			if (_4) {
				_4 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_3, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, _0, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_append(&result, &v, PH_SEPARATE, "stub/generators.zep", 137);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_CTOR(&result);
}

void zep_Stub_Generators_zephir_gen_step_countTo(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0, _1, _2$$3;
	zend_long n, i;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("sideEffects", 11, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { n = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 0)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_2$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { i = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	i = 1;
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 35, PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 35, &_1);
	while (1) {
		if (!(i <= n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_LONG(&_2$$3, i);
		zephir_generator_yield(zephir_gen, NULL, &_2$$3, 1);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) n);
		zephir_generator_slot_set(zephir_gen, 1, &_2$$3);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) i);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_1:;
		i = (i + 1);
	}
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_keysMix(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0, _1, _2, _3, _4, _5, _6, _7;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 8);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_0);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_1);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &_2);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &_3);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 4, &_4);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 5, &_5);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 6, &_6);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 7, &_7);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		case 3: goto zephir_yield_resume_3;
		case 4: goto zephir_yield_resume_4;
		case 5: goto zephir_yield_resume_5;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_generator_yield(zephir_gen, &_0, &_1, 1);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
		zephir_generator_slot_set(zephir_gen, 4, &_4);
		zephir_generator_slot_set(zephir_gen, 5, &_5);
		zephir_generator_slot_set(zephir_gen, 6, &_6);
		zephir_generator_slot_set(zephir_gen, 7, &_7);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 10);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 2);
	zephir_generator_yield(zephir_gen, &_2, &_3, 2);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
		zephir_generator_slot_set(zephir_gen, 4, &_4);
		zephir_generator_slot_set(zephir_gen, 5, &_5);
		zephir_generator_slot_set(zephir_gen, 6, &_6);
		zephir_generator_slot_set(zephir_gen, 7, &_7);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_2:;
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_LONG(&_4, 3);
	zephir_generator_yield(zephir_gen, NULL, &_4, 3);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
		zephir_generator_slot_set(zephir_gen, 4, &_4);
		zephir_generator_slot_set(zephir_gen, 5, &_5);
		zephir_generator_slot_set(zephir_gen, 6, &_6);
		zephir_generator_slot_set(zephir_gen, 7, &_7);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_3:;
	ZEPHIR_INIT_NVAR(&_5);
	ZVAL_STRING(&_5, "z");
	ZEPHIR_INIT_NVAR(&_6);
	ZVAL_LONG(&_6, 4);
	zephir_generator_yield(zephir_gen, &_5, &_6, 4);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
		zephir_generator_slot_set(zephir_gen, 4, &_4);
		zephir_generator_slot_set(zephir_gen, 5, &_5);
		zephir_generator_slot_set(zephir_gen, 6, &_6);
		zephir_generator_slot_set(zephir_gen, 7, &_7);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_4:;
	ZEPHIR_INIT_NVAR(&_7);
	ZVAL_LONG(&_7, 5);
	zephir_generator_yield(zephir_gen, NULL, &_7, 5);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
		zephir_generator_slot_set(zephir_gen, 3, &_3);
		zephir_generator_slot_set(zephir_gen, 4, &_4);
		zephir_generator_slot_set(zephir_gen, 5, &_5);
		zephir_generator_slot_set(zephir_gen, 6, &_6);
		zephir_generator_slot_set(zephir_gen, 7, &_7);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_5:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_bare(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 0);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	zephir_generator_yield(zephir_gen, NULL, NULL, 1);
	
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	zephir_generator_yield(zephir_gen, NULL, NULL, 2);
	
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_2:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_withReturn(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0, _1;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 2);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_0);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_1);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_generator_yield(zephir_gen, NULL, &_0, 1);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "done");
	zephir_generator_finish(zephir_gen, &_1);
	ZEPHIR_MM_RESTORE();
	return;
}

void zep_Stub_Generators_zephir_gen_step_three(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0, _1, _2;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 3);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_0);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_1);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &_2);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		case 3: goto zephir_yield_resume_3;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_generator_yield(zephir_gen, NULL, &_0, 1);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_generator_yield(zephir_gen, NULL, &_1, 2);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_2:;
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 3);
	zephir_generator_yield(zephir_gen, NULL, &_2, 3);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
		zephir_generator_slot_set(zephir_gen, 1, &_1);
		zephir_generator_slot_set(zephir_gen, 2, &_2);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_3:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_boom(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 1);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_0);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_generator_yield(zephir_gen, NULL, &_0, 1);
	zephir_generator_slot_set(zephir_gen, 0, &_0);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "boom", "stub/generators.zep", 54);
	return;
}

void zep_Stub_Generators_zephir_gen_step_seq(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0$$3;
	zval label;
	zend_bool tail;
	double step, acc;
	zend_long n, i;
		ZVAL_UNDEF(&label);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 7);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { n = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 0)); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 1)) != IS_UNDEF) { step = Z_DVAL_P(zephir_generator_slot(zephir_gen, 1)); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { tail = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) == IS_TRUE); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &label);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 4, &_0$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 5)) != IS_UNDEF) { acc = Z_DVAL_P(zephir_generator_slot(zephir_gen, 5)); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 6)) != IS_UNDEF) { i = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 6)); }
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	i = 1;
	acc =  (0.0);
	while (1) {
		if (!(i <= n)) {
			break;
		}
		acc =  ((acc + step));
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZVAL_DOUBLE(&_0$$3, acc);
		zephir_generator_yield(zephir_gen, NULL, &_0$$3, 1);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) n);
		zephir_generator_slot_set_double(zephir_gen, 1, step);
		zephir_generator_slot_set_bool(zephir_gen, 2, (int) tail);
		zephir_generator_slot_set(zephir_gen, 3, &label);
		zephir_generator_slot_set(zephir_gen, 4, &_0$$3);
		zephir_generator_slot_set_double(zephir_gen, 5, acc);
		zephir_generator_slot_set_long(zephir_gen, 6, (zend_long) i);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_1:;
		i = (i + 1);
	}
	if (tail) {
		zephir_generator_yield(zephir_gen, NULL, &label, 2);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) n);
		zephir_generator_slot_set_double(zephir_gen, 1, step);
		zephir_generator_slot_set_bool(zephir_gen, 2, (int) tail);
		zephir_generator_slot_set(zephir_gen, 3, &label);
		zephir_generator_slot_set(zephir_gen, 4, &_0$$3);
		zephir_generator_slot_set_double(zephir_gen, 5, acc);
		zephir_generator_slot_set_long(zephir_gen, 6, (zend_long) i);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_2:;
	}
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_arr(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	HashPosition _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, k, v, _0, *_2;
	zval items;
		ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 5);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &items);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &k);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 4, &v);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	zephir_is_iterable(&items, 0, "stub/generators.zep", 77);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_COPY(&_0, &items);
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(&_0), &_1);
	while (1) {
		_2 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_0), &_1);
		if (_2 == NULL) {
			break;
		}
		ZEPHIR_INIT_NVAR(&k);
		zend_hash_get_current_key_zval_ex(Z_ARRVAL_P(&_0), &k, &_1);
		ZEPHIR_INIT_NVAR(&v);
		ZVAL_COPY(&v, _2);
		zend_hash_move_forward_ex(Z_ARRVAL_P(&_0), &_1);
			zephir_generator_yield(zephir_gen, &k, &v, 1);
			zephir_generator_slot_set(zephir_gen, 0, &items);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set(zephir_gen, 3, &k);
		zephir_generator_slot_set(zephir_gen, 4, &v);
			ZEPHIR_MM_RESTORE();
			return;
			zephir_yield_resume_1:;
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_arrReverse(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	HashPosition _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, v, _0, *_2;
	zval items;
		ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 4);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &items);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &v);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	zephir_is_iterable(&items, 0, "stub/generators.zep", 85);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_COPY(&_0, &items);
	zend_hash_internal_pointer_end_ex(Z_ARRVAL_P(&_0), &_1);
	while (1) {
		_2 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_0), &_1);
		if (_2 == NULL) {
			break;
		}
		ZEPHIR_INIT_NVAR(&v);
		ZVAL_COPY(&v, _2);
		zend_hash_move_backwards_ex(Z_ARRVAL_P(&_0), &_1);
			zephir_generator_yield(zephir_gen, NULL, &v, 1);
			zephir_generator_slot_set(zephir_gen, 0, &items);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set(zephir_gen, 3, &v);
			ZEPHIR_MM_RESTORE();
			return;
			zephir_yield_resume_1:;
	}
	ZEPHIR_INIT_NVAR(&v);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_nested(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zend_bool _7$$3;
	HashPosition _1, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, row, x, _0, *_2, _3$$3, *_5$$3, _6$$3;
	zval matrix;
		ZVAL_UNDEF(&matrix);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&row);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_6$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 9);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &matrix);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &_3$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 4)) != IS_UNDEF) { _4$$3 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 4)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 5, &_6$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 6)) != IS_UNDEF) { _7$$3 = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 6)) == IS_TRUE); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 7, &row);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 8, &x);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	zephir_is_iterable(&matrix, 0, "stub/generators.zep", 95);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_COPY(&_0, &matrix);
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(&_0), &_1);
	while (1) {
		_2 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_0), &_1);
		if (_2 == NULL) {
			break;
		}
		ZEPHIR_INIT_NVAR(&row);
		ZVAL_COPY(&row, _2);
		zend_hash_move_forward_ex(Z_ARRVAL_P(&_0), &_1);
			zephir_is_iterable(&row, 0, "stub/generators.zep", 94);
			if (Z_TYPE_P(&row) == IS_ARRAY) {
				ZEPHIR_INIT_NVAR(&_3$$3);
				ZVAL_COPY(&_3$$3, &row);
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(&_3$$3), &_4$$3);
				while (1) {
					_5$$3 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_3$$3), &_4$$3);
					if (_5$$3 == NULL) {
						break;
					}
					ZEPHIR_INIT_NVAR(&x);
					ZVAL_COPY(&x, _5$$3);
					zend_hash_move_forward_ex(Z_ARRVAL_P(&_3$$3), &_4$$3);
						zephir_generator_yield(zephir_gen, NULL, &x, 1);
						zephir_generator_slot_set(zephir_gen, 0, &matrix);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set(zephir_gen, 3, &_3$$3);
		zephir_generator_slot_set_long(zephir_gen, 4, (zend_long) _4$$3);
		zephir_generator_slot_set(zephir_gen, 5, &_6$$3);
		zephir_generator_slot_set_bool(zephir_gen, 6, (int) _7$$3);
		zephir_generator_slot_set(zephir_gen, 7, &row);
		zephir_generator_slot_set(zephir_gen, 8, &x);
						ZEPHIR_MM_RESTORE();
						return;
						zephir_yield_resume_1:;
				}
			} else {
				ZEPHIR_CALL_METHOD(NULL, &row, "rewind", NULL, 0);
				zephir_check_call_status();
				_7$$3 = 1;
				while (1) {
					if (_7$$3) {
						_7$$3 = 0;
					} else {
						ZEPHIR_CALL_METHOD(NULL, &row, "next", NULL, 0);
						zephir_check_call_status();
					}
					ZEPHIR_CALL_METHOD(&_6$$3, &row, "valid", NULL, 0);
					zephir_check_call_status();
					if (!zend_is_true(&_6$$3)) {
						break;
					}
					ZEPHIR_CALL_METHOD(&x, &row, "current", NULL, 0);
					zephir_check_call_status();
						zephir_generator_yield(zephir_gen, NULL, &x, 2);
						zephir_generator_slot_set(zephir_gen, 0, &matrix);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set(zephir_gen, 3, &_3$$3);
		zephir_generator_slot_set_long(zephir_gen, 4, (zend_long) _4$$3);
		zephir_generator_slot_set(zephir_gen, 5, &_6$$3);
		zephir_generator_slot_set_bool(zephir_gen, 6, (int) _7$$3);
		zephir_generator_slot_set(zephir_gen, 7, &row);
		zephir_generator_slot_set(zephir_gen, 8, &x);
						ZEPHIR_MM_RESTORE();
						return;
						zephir_yield_resume_2:;
				}
			}
			ZEPHIR_INIT_NVAR(&x);
	}
	ZEPHIR_INIT_NVAR(&row);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_overItems(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zend_bool _5;
	HashPosition _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, v, _0, _1, *_3, _4;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("items", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 5);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &_1);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 1)) != IS_UNDEF) { _2 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 1)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &_4);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 3)) != IS_UNDEF) { _5 = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 3)) == IS_TRUE); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 4, &v);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 36, PH_NOISY_CC | PH_READONLY);
	zephir_is_iterable(&_0, 0, "stub/generators.zep", 103);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEPHIR_INIT_NVAR(&_1);
		ZVAL_COPY(&_1, &_0);
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(&_1), &_2);
		while (1) {
			_3 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_1), &_2);
			if (_3 == NULL) {
				break;
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _3);
			zend_hash_move_forward_ex(Z_ARRVAL_P(&_1), &_2);
				zephir_generator_yield(zephir_gen, NULL, &v, 1);
				zephir_generator_slot_set(zephir_gen, 0, &_1);
		zephir_generator_slot_set_long(zephir_gen, 1, (zend_long) _2);
		zephir_generator_slot_set(zephir_gen, 2, &_4);
		zephir_generator_slot_set_bool(zephir_gen, 3, (int) _5);
		zephir_generator_slot_set(zephir_gen, 4, &v);
				ZEPHIR_MM_RESTORE();
				return;
				zephir_yield_resume_1:;
		}
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		_5 = 1;
		while (1) {
			if (_5) {
				_5 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_4, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_4)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, &_0, "current", NULL, 0);
			zephir_check_call_status();
				zephir_generator_yield(zephir_gen, NULL, &v, 2);
				zephir_generator_slot_set(zephir_gen, 0, &_1);
		zephir_generator_slot_set_long(zephir_gen, 1, (zend_long) _2);
		zephir_generator_slot_set(zephir_gen, 2, &_4);
		zephir_generator_slot_set_bool(zephir_gen, 3, (int) _5);
		zephir_generator_slot_set(zephir_gen, 4, &v);
				ZEPHIR_MM_RESTORE();
				return;
				zephir_yield_resume_2:;
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_range4(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _3$$3;
	zend_long n, i = 0, _1, _2;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 6);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { n = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 0)); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 1)) != IS_UNDEF) { _0 = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 1)) == IS_TRUE); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 3)) != IS_UNDEF) { _2 = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 3)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 4, &_3$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 5)) != IS_UNDEF) { i = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 5)); }
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	_2 = n;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_generator_yield(zephir_gen, NULL, &_3$$3, 1);
			zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) n);
		zephir_generator_slot_set_bool(zephir_gen, 1, (int) _0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set_long(zephir_gen, 3, (zend_long) _2);
		zephir_generator_slot_set(zephir_gen, 4, &_3$$3);
		zephir_generator_slot_set_long(zephir_gen, 5, (zend_long) i);
			ZEPHIR_MM_RESTORE();
			return;
			zephir_yield_resume_1:;
		}
	}
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_viaSwitch(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0$$3, _1$$4;
	zend_long mode;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { mode = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 0)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0$$3);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &_1$$4);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	if (mode == 1) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZVAL_STRING(&_0$$3, "one");
		zephir_generator_yield(zephir_gen, NULL, &_0$$3, 1);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) mode);
		zephir_generator_slot_set(zephir_gen, 1, &_0$$3);
		zephir_generator_slot_set(zephir_gen, 2, &_1$$4);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_1:;
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		ZEPHIR_INIT_NVAR(&_1$$4);
		ZVAL_STRING(&_1$$4, "other");
		zephir_generator_yield(zephir_gen, NULL, &_1$$4, 2);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) mode);
		zephir_generator_slot_set(zephir_gen, 1, &_0$$3);
		zephir_generator_slot_set(zephir_gen, 2, &_1$$4);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_2:;
	zephir_switch_0_end: ;

	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Generators_zephir_gen_step_relay(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval it, *zephir_gen, zephir_gen_sub, v, _0;
		ZVAL_UNDEF(&it);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 4);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &it);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) == IS_TRUE); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &v);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	zephir_is_iterable(&it, 0, "stub/generators.zep", 130);
	ZEPHIR_CALL_METHOD(NULL, &it, "rewind", NULL, 0);
	zephir_check_call_status();
	_1 = 1;
	while (1) {
		if (_1) {
			_1 = 0;
		} else {
			ZEPHIR_CALL_METHOD(NULL, &it, "next", NULL, 0);
			zephir_check_call_status();
		}
		ZEPHIR_CALL_METHOD(&_0, &it, "valid", NULL, 0);
		zephir_check_call_status();
		if (!zend_is_true(&_0)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&v, &it, "current", NULL, 0);
		zephir_check_call_status();
			zephir_generator_yield(zephir_gen, NULL, &v, 1);
			zephir_generator_slot_set(zephir_gen, 0, &it);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_bool(zephir_gen, 2, (int) _1);
		zephir_generator_slot_set(zephir_gen, 3, &v);
			ZEPHIR_MM_RESTORE();
			return;
			zephir_yield_resume_1:;
	}
	ZEPHIR_INIT_NVAR(&v);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

zend_object *zephir_init_properties_Stub_Generators(zend_class_entry *class_type)
{
		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("items"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("items"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

