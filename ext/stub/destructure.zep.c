
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * Destructuring assignment: let [a, b, c] = expr;
 *
 * @see https://github.com/zephir-lang/zephir/issues/2496
 */
ZEPHIR_INIT_CLASS(Stub_Destructure)
{
	ZEPHIR_REGISTER_CLASS(Stub, Destructure, stub, destructure, stub_destructure_method_entry, 0);

	zend_declare_property_long(stub_destructure_ce, SL("calls"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_destructure_ce, SL("data"), ZEND_ACC_PROTECTED);
	stub_destructure_ce->create_object = zephir_init_properties_Stub_Destructure;

	return SUCCESS;
}

PHP_METHOD(Stub_Destructure, basic)
{
	zval arr, a, b, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 19);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &arr, 1, PH_NOISY, "stub/destructure.zep", 19);
	zephir_memory_observe(&c);
	zephir_array_fetch_long(&c, &arr, 2, PH_NOISY, "stub/destructure.zep", 19);
	zephir_create_array(return_value, 3, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	zephir_array_fast_append(return_value, &c);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, skipped)
{
	zval arr, a, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 29);
	zephir_memory_observe(&c);
	zephir_array_fetch_long(&c, &arr, 2, PH_NOISY, "stub/destructure.zep", 29);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &c);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, trailingSkip)
{
	zval arr, a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 39);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Destructure, literal)
{
	zval _0;
	zval a, b, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &_0, 0, PH_NOISY, "stub/destructure.zep", 48);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &_0, 1, PH_NOISY, "stub/destructure.zep", 48);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, pair)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("calls", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 29, PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 29, &_1);
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 10);
	zephir_array_fast_append(return_value, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 20);
	zephir_array_fast_append(return_value, &_2);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, fromCall)
{
	zval a, b, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("calls", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "pair", NULL, 0);
	zephir_check_call_status();
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &_0, 0, PH_NOISY, "stub/destructure.zep", 64);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &_0, 1, PH_NOISY, "stub/destructure.zep", 64);
	zephir_create_array(return_value, 3, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	zephir_memory_observe(&_1);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 29, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_1);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, swap)
{
	zval _0;
	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_LONG(&a, 1);
	ZEPHIR_INIT_VAR(&b);
	ZVAL_LONG(&b, 2);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, &b);
	zephir_array_fast_append(&_0, &a);
	ZEPHIR_OBS_NVAR(&a);
	zephir_array_fetch_long(&a, &_0, 0, PH_NOISY, "stub/destructure.zep", 74);
	ZEPHIR_OBS_NVAR(&b);
	zephir_array_fetch_long(&b, &_0, 1, PH_NOISY, "stub/destructure.zep", 74);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, fewerTargets)
{
	zval arr, a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 84);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &arr, 1, PH_NOISY, "stub/destructure.zep", 84);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, missingSlot)
{
	zval arr, a, b, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 94);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &arr, 1, PH_NOISY, "stub/destructure.zep", 94);
	zephir_memory_observe(&c);
	zephir_array_fetch_long(&c, &arr, 2, PH_NOISY, "stub/destructure.zep", 94);
	zephir_create_array(return_value, 3, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	zephir_array_fast_append(return_value, &c);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, nestedValue)
{
	zval _0, _1;
	zval a, b, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 2);
	zephir_array_fast_append(&_1, &_2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 3);
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 4);
	zephir_array_fast_append(&_1, &_2);
	zephir_array_fast_append(&_0, &_1);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &_0, 0, PH_NOISY, "stub/destructure.zep", 103);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &_0, 1, PH_NOISY, "stub/destructure.zep", 103);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, typedTargets)
{
	zval _1, _2, _3;
	zval _0;
	zval s;
	zend_long i = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "x");
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fetch_long(&_2, &_0, 0, PH_NOISY | PH_READONLY, "stub/destructure.zep", 112);
	i = zephir_get_numberval(&_2);
	zephir_array_fetch_long(&_3, &_0, 1, PH_NOISY | PH_READONLY, "stub/destructure.zep", 112);
	zephir_get_strval(&s, &_3);
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, i);
	zephir_array_fast_append(return_value, &_1);
	zephir_array_fast_append(return_value, &s);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, keyedSource)
{
	zval arr, a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	add_index_stringl(&arr, 0, SL("z"));
	add_index_stringl(&arr, 1, SL("y"));
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 122);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &arr, 1, PH_NOISY, "stub/destructure.zep", 122);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, inLoop)
{
	zval _0;
	zval pairs, item, out, k, v, _1, *_2, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&pairs);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&pairs);
	zephir_create_array(&pairs, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&pairs, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&pairs, &_0);
	ZEPHIR_INIT_VAR(&out);
	array_init(&out);
	zephir_is_iterable(&pairs, 0, "stub/destructure.zep", 138);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&pairs), _2)
	{
		ZEPHIR_INIT_NVAR(&item);
		ZVAL_COPY(&item, _2);
		ZEPHIR_OBS_NVAR(&k);
		zephir_array_fetch_long(&k, &item, 0, PH_NOISY, "stub/destructure.zep", 134);
		ZEPHIR_OBS_NVAR(&v);
		zephir_array_fetch_long(&v, &item, 1, PH_NOISY, "stub/destructure.zep", 134);
		ZEPHIR_INIT_NVAR(&_3$$3);
		zephir_add_function(&_3$$3, &k, &v);
		zephir_array_append(&out, &_3$$3, PH_SEPARATE, "stub/destructure.zep", 135);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&item);
	RETURN_CCTOR(&out);
}

PHP_METHOD(Stub_Destructure, chained)
{
	zend_long c = 0;
	zval arr, a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &arr, 0, PH_NOISY, "stub/destructure.zep", 146);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &arr, 1, PH_NOISY, "stub/destructure.zep", 146);
	c = 9;
	zephir_create_array(return_value, 3, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Destructure, fromProperty)
{
	zval a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("data", 4, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 30, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&a);
	zephir_array_fetch_long(&a, &_0, 0, PH_NOISY, "stub/destructure.zep", 155);
	zephir_memory_observe(&b);
	zephir_array_fetch_long(&b, &_0, 1, PH_NOISY, "stub/destructure.zep", 155);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Destructure(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("data"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 2, 0);
			ZEPHIR_INIT_VAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 7);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 8);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("data"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

