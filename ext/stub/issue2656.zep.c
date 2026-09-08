
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"


/**
 * kernel/array.c's ArrayAccess branches got three lifetimes wrong.
 *
 * zephir_array_isset_string_fetch() released the zend_string it built for the
 * offset between the offsetExists() and the offsetGet() call, so offsetGet()
 * was handed a freed string. The failure is not a crash: the very next
 * allocation is ZVAL_STRINGL(&method, "offsetget", 9) inside
 * zephir_call_class_method_aparams(), which lands in the same ZEND_MM bin and
 * pops the slot the offset just vacated, so the object silently receives the
 * key "offsetget".
 *
 * All six isset helpers additionally dropped the offsetExists() return value
 * on the floor, and none of them held a reference to the container across the
 * two userland calls. PHP's own zend_std_read_dimension() owns the offset, the
 * container and the offsetExists() result; these helpers now do the same.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */
ZEPHIR_INIT_CLASS(Stub_Issue2656)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2656, stub, issue2656, stub_issue2656_method_entry, 0);

	zend_declare_property_null(stub_issue2656_ce, SL("container"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2656_ce, SL("retained"), ZEND_ACC_PROTECTED);
	stub_issue2656_ce->create_object = zephir_init_properties_Stub_Issue2656;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2656, setContainer)
{
	zval *container, container_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&container_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &container);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 59, container);
}

/**
 * The reported shape. `let value = null` is a second mutation on purpose:
 * it keeps the fetch off the read-only path, so this method measures the
 * offset lifetime and nothing else.
 *
 * The key is nine characters on purpose. _ZSTR_STRUCT_SIZE(9) is 34, the
 * same 40-byte bin as "offsetget", which is what makes the corruption
 * deterministic on every supported PHP version. A shorter key only
 * reproduces on 8.4+ and would pass silently on 8.0 to 8.2.
 */
PHP_METHOD(Stub_Issue2656, fetchLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_INIT_VAR(&value);
	ZVAL_NULL(&value);
	ZEPHIR_OBS_NVAR(&value);
	if (zephir_array_isset_string_fetch(&value, container, SL("cache.key"), 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_Issue2656, fetchLiteralMissing)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_INIT_VAR(&value);
	ZVAL_NULL(&value);
	ZEPHIR_OBS_NVAR(&value);
	if (zephir_array_isset_string_fetch(&value, container, SL("absent.k1"), 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_BOOL(0);
}

/**
 * empty() on an array-access reaches the same helper through
 * zephir_isempty_dim_string().
 */
PHP_METHOD(Stub_Issue2656, emptyLiteral)
{
	zval *container, container_sub;

	ZVAL_UNDEF(&container_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &container);
	RETURN_BOOL(zephir_isempty_dim_string(container, SL("cache.key")));
}

PHP_METHOD(Stub_Issue2656, issetLiteral)
{
	zval *container, container_sub;

	ZVAL_UNDEF(&container_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &container);
	RETURN_BOOL(zephir_array_isset_value_string(container, SL("cache.key")));
}

PHP_METHOD(Stub_Issue2656, fetchLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_INIT_VAR(&value);
	ZVAL_NULL(&value);
	ZEPHIR_OBS_NVAR(&value);
	if (zephir_array_isset_long_fetch(&value, container, 7, 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_Issue2656, issetLong)
{
	zval *container, container_sub;

	ZVAL_UNDEF(&container_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &container);
	RETURN_BOOL(zephir_array_isset_value_long(container, 7));
}

PHP_METHOD(Stub_Issue2656, fetchDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, *key, key_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(container)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &container, &key);
	ZEPHIR_INIT_VAR(&value);
	ZVAL_NULL(&value);
	ZEPHIR_OBS_NVAR(&value);
	if (zephir_array_isset_fetch(&value, container, key, 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_Issue2656, issetDynamic)
{
	zval *container, container_sub, *key, key_sub;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(container)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &container, &key);
	RETURN_BOOL(zephir_array_isset_value(container, key));
}

/**
 * The container is read from a property, which the emitter hands over
 * borrowed. A userland offsetExists() that clears that property drops the
 * last reference, so without an owned reference across both calls the
 * helper reads a freed object.
 */
PHP_METHOD(Stub_Issue2656, fetchThroughProperty)
{
	zval value, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&value);
	ZVAL_NULL(&value);
	ZEPHIR_OBS_NVAR(&value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 59, PH_NOISY_CC | PH_READONLY);
	if (zephir_array_isset_string_fetch(&value, &_0, SL("cache.key"), 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_BOOL(0);
}

/**
 * Every isset helper, `iterations` times. The recorder's offsetExists()
 * returns a fresh heap string, so an unreleased offsetExists() result
 * shows up as growth. Nothing in the loop retains anything.
 */
PHP_METHOD(Stub_Issue2656, leakProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL, *_2 = NULL, *_3 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL, *_7 = NULL, *_8 = NULL;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *container, container_sub, *key, key_sub, *iterations_param = NULL, before, after, sink;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&sink);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(container)
		Z_PARAM_ZVAL(key)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &container, &key, &iterations_param);
	ZEPHIR_INIT_VAR(&sink);
	ZVAL_NULL(&sink);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "fetchliteral", &_1, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "fetchliteralmissing", &_2, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "fetchlong", &_3, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "fetchdynamic", &_4, 0, container, key);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "issetliteral", &_5, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "issetlong", &_6, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "issetdynamic", &_7, 0, container, key);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "emptyliteral", &_8, 0, container);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The positive control for every probe in this class: it retains what it
 * allocates, so it must report growth. A control that replaces what it
 * retains reads zero and proves nothing.
 */
PHP_METHOD(Stub_Issue2656, leakProbeControl)
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
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "x");
		ZVAL_LONG(&_2$$3, 64);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "str_repeat", &_4, 29, &_1$$3, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_3$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * `unset obj->literal` routes through kernel/object.c
 * zephir_unset_property(), which built a zend_string for the name and
 * never released it, leaking on every call.
 */
PHP_METHOD(Stub_Issue2656, unsetPropertyProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after, obj, _1$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("probe", 5, 1);
	}

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
		ZEPHIR_INIT_NVAR(&obj);
		object_init(&obj);
		ZVAL_UNDEF(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_update_property_zval_cached(&obj, _zephir_prop_0, 0, &_1$$3);
		zephir_unset_property(&obj, "probe");
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * `throw` of a non-object routes through kernel/exception.c
 * zephir_throw_exception_debug(), which wraps the value in an Exception
 * and addrefs it for zend_throw_exception_object(). Exception::__construct
 * rejects an array, so the throw is skipped and both the instance and the
 * extra reference used to leak.
 */
PHP_METHOD(Stub_Issue2656, throwArrayProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after, bad, _1$$3, _2$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&bad);
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
		ZEPHIR_INIT_NVAR(&bad);
		zephir_create_array(&bad, 2, 0);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_array_fast_append(&bad, &_1$$3);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 2);
		zephir_array_fast_append(&bad, &_1$$3);

		/* try_start_1: */

			zephir_throw_exception_debug(&bad, "stub/issue2656.zep", 230);
			goto try_end_1;
		try_end_1:

		if (EG(exception)) {
			ZEPHIR_INIT_NVAR(&_1$$3);
			ZVAL_OBJ(&_1$$3, EG(exception));
			Z_ADDREF_P(&_1$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			if (zephir_is_instance_of(&_1$$3, SL("Throwable"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&_2$$3, &_1$$3);
			}
		}
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Issue2656(zend_class_entry *class_type)
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

