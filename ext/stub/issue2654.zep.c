
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/generator.h"


/**
 * A declared variable that is never assigned keeps the ZVAL_UNDEF it receives
 * at declaration, and that zval reaches userland unchanged: var_dump() renders
 * it as UNKNOWN:0, which is not a PHP value. PHP evaluates an unset variable as
 * null.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2654
 */
ZEPHIR_INIT_CLASS(Stub_Issue2654)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2654, stub, issue2654, stub_issue2654_method_entry, 0);

	zend_declare_property_null(stub_issue2654_ce, SL("retained"), ZEND_ACC_PROTECTED);
	stub_issue2654_ce->create_object = zephir_init_properties_Stub_Issue2654;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2654, readVar)
{
	zval x;

	ZVAL_NULL(&x);
	RETURN_CCTORW(&x);
}

PHP_METHOD(Stub_Issue2654, readString)
{
	zval s;

	ZVAL_NULL(&s);
	RETURN_CTORW(&s);
}

PHP_METHOD(Stub_Issue2654, readArray)
{
	zval a;

	ZVAL_NULL(&a);
	RETURN_CTORW(&a);
}

PHP_METHOD(Stub_Issue2654, readInt)
{
	zend_long i = 0;
	RETURN_LONG(i);
}

/**
 * Any read is affected, not only `return`: an unassigned local handed to a
 * function has to arrive as null too.
 */
PHP_METHOD(Stub_Issue2654, passAsArgument)
{
	zval x;

	ZVAL_NULL(&x);
	zephir_gettype(return_value, &x);
	return;
}

/**
 * The same value captured by a closure.
 */
PHP_METHOD(Stub_Issue2654, captureInClosure)
{
	zval x, capture, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_NULL(&x);
	ZVAL_UNDEF(&capture);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_45__closure_ce);
	zephir_update_property_zval(&_0, SL("x"), &x);
	ZEPHIR_INIT_VAR(&capture);
	zephir_create_closure_bound(&capture, &_0, NULL, stub_45__closure_ce, SL("__invoke"));
	ZEPHIR_CALL_USER_FUNC(return_value, &capture);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2654, emptyOnUnassigned)
{
	zval x;

	ZVAL_NULL(&x);
	RETURN_BOOL(ZEPHIR_IS_EMPTY(&x));
}

/**
 * `-null` is 0 in PHP. Reaching zephir_negate() with an IS_UNDEF zval falls
 * through to convert_scalar_to_number(), which asserts.
 */
PHP_METHOD(Stub_Issue2654, negateUnassigned)
{
	zval x;

	ZVAL_NULL(&x);
	zephir_negate(&x);
	RETURN_CCTORW(&x);
}

PHP_METHOD(Stub_Issue2654, concatUnassigned)
{
	zval x;

	ZVAL_NULL(&x);
	ZEPHIR_CONCAT_SV(return_value, "tail", &x);
	return;
}

/**
 * A generator step saves and restores its locals across every resume.
 */
PHP_METHOD(Stub_Issue2654, yieldUnassigned)
{
	zval *this_ptr = getThis();
	zephir_generator_create(return_value, this_ptr, stub_issue2654_ce, zep_Stub_Issue2654_zephir_gen_step_yieldUnassigned, 0);
	return;
}

/**
 * Assigned locals must keep their existing behaviour: this one is the
 * control for the whole fix.
 */
PHP_METHOD(Stub_Issue2654, writeVar)
{
	zval y;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&y);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&y);
	ZVAL_STRING(&y, "assigned");
	RETURN_CCTOR(&y);
}

/**
 * Omitting the argument routes `b` through its `b_sub` companion, whose
 * IS_UNDEF is what makes ZEPHIR_CPY_WRT register it with the memory frame.
 */
PHP_METHOD(Stub_Issue2654, optionalParam)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b = NULL, b_sub, __$null, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &b);
	if (!b) {
		b = &b_sub;
		ZEPHIR_CPY_WRT(b, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(b);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "prefix", b);
	ZEPHIR_CPY_WRT(b, &_0);
	RETVAL_ZVAL(b, 1, 0);
	RETURN_MM();
}

/**
 * Reads an unassigned local `iterations` times and reports how much memory
 * the run retained. Nothing is allocated inside the measured window, so a
 * non-zero result is a leak.
 */
PHP_METHOD(Stub_Issue2654, leakProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL, *_2 = NULL, *_3 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL, *_7 = NULL;
	zval *iterations_param = NULL, before, after, sink;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&sink);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&sink);
	ZVAL_NULL(&sink);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "readvar", &_1, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "readstring", &_2, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "readarray", &_3, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "passasargument", &_4, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "captureinclosure", &_5, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "concatunassigned", &_6, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "optionalparam", &_7, 0);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The positive control for leakProbe(): it retains every value it produces,
 * so it must report growth. A control that replaces what it retains reads
 * zero and proves nothing.
 */
PHP_METHOD(Stub_Issue2654, leakProbeControl)
{
	zval _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after, _1$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, i);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZEPHIR_CONCAT_SV(&_2$$3, "retained-", &_1$$3);
		zephir_update_property_array_append(this_ptr, SL("retained"), &_2$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

void zep_Stub_Issue2654_zephir_gen_step_yieldUnassigned(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, x;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_NULL(&x);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 1);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &x);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	zephir_generator_yield(zephir_gen, NULL, &x, 1);
	zephir_generator_slot_set(zephir_gen, 0, &x);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

zend_object *zephir_init_properties_Stub_Issue2654(zend_class_entry *class_type)
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
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("retained"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("retained"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

