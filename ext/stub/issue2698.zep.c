
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * Separating an array property must hand the separated array back to exactly
 * one owner.
 *
 * kernel/object.c's separation idiom is
 *
 *     ZVAL_DUP(&new_zv, &tmp);          // a fresh array at refcount 1
 *     ZVAL_COPY_VALUE(&tmp, &new_zv);   // tmp now holds that one reference
 *
 * and the zval_ptr_dtor(&tmp) at the end of each function is what releases it.
 * Three of the six sites dropped the reference with Z_TRY_DELREF() first, so
 * that dtor decremented zero instead of freeing, and the separated array plus
 * everything in it was retained for the life of the request.
 *
 * Every property here is TYPED on purpose. A typed default is persistent and
 * non-refcounted (issues #2608 and #2651), which is the only shape that reaches
 * the broken branch. An untyped `protected items = []` is declared null and
 * filled by the per-instance initializer, so it is already refcount 1 and
 * updates in place.
 *
 * The probes build a fresh instance per iteration because the defect fires only
 * while the property still holds its declared default; after the first write it
 * holds a refcount 1 array and later writes never separate.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2698
 */
ZEPHIR_INIT_CLASS(Stub_Issue2698)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2698, stub, issue2698, stub_issue2698_method_entry, 0);

	/** let this->items[key] = value -> zephir_update_property_array() */
	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_assoc_long_ex(&_zc0, SL("seed"), 1);
		zephir_declare_typed_property(stub_issue2698_ce, SL("items"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	/** let this->bag[] = value -> zephir_update_property_array_append() */
	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_next_index_stringl(&_zc0, SL("seed"));
		zephir_declare_typed_property(stub_issue2698_ce, SL("bag"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	/** let this->matrix[a][b] = value -> zephir_update_property_array_multi() */
	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		zval _zc1;
		array_init_size(&_zc1, 2);
		add_assoc_long_ex(&_zc1, SL("col"), 1);
		add_assoc_zval_ex(&_zc0, SL("row"), &_zc1);
		zephir_declare_typed_property(stub_issue2698_ce, SL("matrix"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	/** unset this->removable[key] -> zephir_unset_property_array() */
	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_long_ex(&_zc0, SL("a"), 1);
		add_assoc_long_ex(&_zc0, SL("b"), 2);
		zephir_declare_typed_property(stub_issue2698_ce, SL("removable"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	/** Untyped, so it is refcounted from the start: the control's sink. */
	zend_declare_property_null(stub_issue2698_ce, SL("retained"), ZEND_ACC_PROTECTED);
	stub_issue2698_ce->create_object = zephir_init_properties_Stub_Issue2698;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2698, writeOffset)
{
	zval key_zv, *value, value_sub;
	zend_string *key = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&key_zv, key);
	zephir_update_property_array(this_ptr, SL("items"), &key_zv, value);
}

PHP_METHOD(Stub_Issue2698, appendValue)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_array_append(this_ptr, SL("bag"), value);
}

PHP_METHOD(Stub_Issue2698, writeNested)
{
	zval outer_zv, inner_zv, *value, value_sub;
	zend_string *outer = NULL, *inner = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&outer_zv);
	ZVAL_UNDEF(&inner_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(outer)
		Z_PARAM_STR(inner)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 3);
	ZVAL_STR(&outer_zv, outer);
	ZVAL_STR(&inner_zv, inner);
	zephir_update_property_array_multi(this_ptr, SL("matrix"), value, SL("zz"), 2, &outer_zv, &inner_zv);
}

PHP_METHOD(Stub_Issue2698, removeOffset)
{
	zval key_zv, _0;
	zend_string *key = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("removable", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&key_zv, key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("removable"), &key_zv);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 71, PH_NOISY_CC | PH_READONLY);
	zephir_array_unset(&_0, &key_zv, PH_SEPARATE);
}

PHP_METHOD(Stub_Issue2698, getItems)
{

	RETURN_MEMBER_TYPED(getThis(), "items", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2698, getBag)
{

	RETURN_MEMBER_TYPED(getThis(), "bag", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2698, getMatrix)
{

	RETURN_MEMBER_TYPED(getThis(), "matrix", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2698, getRemovable)
{

	RETURN_MEMBER_TYPED(getThis(), "removable", IS_ARRAY);
}

/**
 * zephir_update_property_array(). Nothing in the loop retains anything, so
 * a reading that scales with `iterations` is the separated array leaking.
 */
PHP_METHOD(Stub_Issue2698, writeProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_3 = NULL;
	zval *iterations_param = NULL, before, after, subject, _1$$3, _2$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_NULL(&subject);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&subject);
		object_init_ex(&subject, stub_issue2698_ce);
		if (zephir_has_constructor(&subject)) {
			ZEPHIR_CALL_METHOD(NULL, &subject, "__construct", NULL, 0);
			zephir_check_call_status();
		}

		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "added");
		ZVAL_LONG(&_2$$3, 1);
		ZEPHIR_CALL_METHOD(NULL, &subject, "writeoffset", &_3, 72, &_1$$3, &_2$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * zephir_update_property_array_append(). This site was already correct: it
 * puts the reference back with Z_ADDREF(). The probe pins that, so a later
 * cleanup cannot break it.
 */
PHP_METHOD(Stub_Issue2698, appendProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL;
	zval *iterations_param = NULL, before, after, subject, _1$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_NULL(&subject);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&subject);
		object_init_ex(&subject, stub_issue2698_ce);
		if (zephir_has_constructor(&subject)) {
			ZEPHIR_CALL_METHOD(NULL, &subject, "__construct", NULL, 0);
			zephir_check_call_status();
		}

		ZVAL_LONG(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(NULL, &subject, "appendvalue", &_2, 73, &_1$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * zephir_update_property_array_multi(). Same six lines as writeProbe()'s
 * site, and no fixture reached it before this one.
 */
PHP_METHOD(Stub_Issue2698, nestedProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_4 = NULL;
	zval *iterations_param = NULL, before, after, subject, _1$$3, _2$$3, _3$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_NULL(&subject);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&subject);
		object_init_ex(&subject, stub_issue2698_ce);
		if (zephir_has_constructor(&subject)) {
			ZEPHIR_CALL_METHOD(NULL, &subject, "__construct", NULL, 0);
			zephir_check_call_status();
		}

		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "row");
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "added");
		ZVAL_LONG(&_3$$3, 1);
		ZEPHIR_CALL_METHOD(NULL, &subject, "writenested", &_4, 74, &_1$$3, &_2$$3, &_3$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * zephir_unset_property_array(). This one never released the separated
 * array at all: it has no zval_ptr_dtor() of its own.
 */
PHP_METHOD(Stub_Issue2698, unsetProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL;
	zval *iterations_param = NULL, before, after, subject, _1$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_NULL(&subject);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&subject);
		object_init_ex(&subject, stub_issue2698_ce);
		if (zephir_has_constructor(&subject)) {
			ZEPHIR_CALL_METHOD(NULL, &subject, "__construct", NULL, 0);
			zephir_check_call_status();
		}

		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "a");
		ZEPHIR_CALL_METHOD(NULL, &subject, "removeoffset", &_2, 75, &_1$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * zephir_unset_property_array() again, but through its REFCOUNTED branch:
 * the first unset leaves the property holding a refcount 1 array, handing
 * it to a local raises that to 2, and the second unset separates again.
 * That branch never put the dropped reference back with Z_ADDREF() the way
 * zephir_update_property_array()'s does, so it leaked as well.
 */
PHP_METHOD(Stub_Issue2698, unsetSharedProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL, *_3 = NULL;
	zval *iterations_param = NULL, before, after, subject, held, _1$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&held);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_NULL(&subject);
	ZEPHIR_INIT_VAR(&held);
	ZVAL_NULL(&held);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&subject);
		object_init_ex(&subject, stub_issue2698_ce);
		if (zephir_has_constructor(&subject)) {
			ZEPHIR_CALL_METHOD(NULL, &subject, "__construct", NULL, 0);
			zephir_check_call_status();
		}

		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "a");
		ZEPHIR_CALL_METHOD(NULL, &subject, "removeoffset", &_2, 75, &_1$$3);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&held, &subject, "getremovable", &_3, 76);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "b");
		ZEPHIR_CALL_METHOD(NULL, &subject, "removeoffset", &_2, 75, &_1$$3);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&held);
		ZVAL_NULL(&held);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The positive control for every probe above: it retains what it allocates,
 * so it must report growth. A control that replaces what it retains reads
 * zero and proves nothing.
 */
PHP_METHOD(Stub_Issue2698, leakProbeControl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_4 = NULL;
	zval *iterations_param = NULL, before, after, _1$$3, _2$$3, _3$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "x");
		ZVAL_LONG(&_2$$3, 64);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "str_repeat", &_4, 30, &_1$$3, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_3$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Issue2698(zend_class_entry *class_type)
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

