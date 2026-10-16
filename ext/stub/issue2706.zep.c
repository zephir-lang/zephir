
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/string.h"


/**
 * Array keys PHP derives from a non-string offset.
 *
 * PHP turns a `null` offset into the empty string. `unset` and
 * array_key_exists() looked for a one byte NUL key instead, so they missed
 * the element every other access had written. Both also converted float,
 * bool and resource keys and rejected illegal ones on their own terms.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
ZEPHIR_INIT_CLASS(Stub_Issue2706)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2706, stub, issue2706, stub_issue2706_method_entry, 0);

	/**
	 * Set after array_key_exists() returns, so a test can tell whether the
	 * method went on after it threw.
	 */
	zend_declare_property_bool(stub_issue2706_ce, SL("reached"), 0, ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2706_ce, SL("container"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2706_ce, SL("shared"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	stub_issue2706_ce->create_object = zephir_init_properties_Stub_Issue2706;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2706, write)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, a, _0;

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key);
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "written");
	zephir_array_update_zval(&a, key, &_0, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2706, readBack)
{
	zend_bool _1, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, a, _0, _2, _4;

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key);
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "written");
	zephir_array_update_zval(&a, key, &_0, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_create_array(return_value, 3, 0);
	_1 = zephir_array_isset_value(&a, key);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, _1);
	zephir_array_fast_append(return_value, &_2);
	_3 = zephir_array_key_exists(&a, key);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_BOOL(&_2, _3);
	zephir_array_fast_append(return_value, &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, &a, key, PH_NOISY, "stub/issue2706.zep", 40);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_4);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, unsetKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_unset(a, key, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, unsetPath)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "q");
	{ zval *zephir_unset_offsets[] = { key, &_0 }; zephir_array_unset_path(a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyExists)
{
	zend_bool _0;
	zval *key, key_sub, *a, a_sub;

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &key, &a);
	_0 = zephir_array_key_exists(a, key);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyExistsOr)
{
	zval *left_param = NULL, *key, key_sub, *a, a_sub;
	zend_bool left, _0, _1;

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_BOOL(left)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &left_param, &key, &a);
	_0 = left;
	if (!(_0)) {
		_1 = zephir_array_key_exists(a, key);
		if (UNEXPECTED(EG(exception))) {
			return;
		}
		_0 = _1;
	}
	RETURN_BOOL(_0);
}

/**
 * Each of these reaches a kernel call that can throw: an illegal key,
 * a container that is not an array, or an ArrayAccess method throwing.
 * `reached` stays false only if the method stopped at that call.
 */
PHP_METHOD(Stub_Issue2706, writeStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub, __$true, __$false, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	ZEPHIR_SEPARATE_PARAM(a);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_zval(a, key, &_0, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2706, writeMultiStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub, __$true, __$false, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	ZEPHIR_SEPARATE_PARAM(a);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(a, &_0, SL("zs"), 3, key, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2706, appendStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, __$true, __$false, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_SEPARATE_PARAM(a);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_append(a, &_0, PH_SEPARATE, "stub/issue2706.zep", 89);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2706, readStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub, __$true, __$false, value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&value);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	zephir_memory_observe(&value);
	zephir_array_fetch(&value, a, key, PH_NOISY, "stub/issue2706.zep", 98);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2706, issetStops)
{
	zend_bool found = 0, _0;
	zval *a, a_sub, *key, key_sub, __$true, __$false;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &key);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	_0 = zephir_array_isset_value(a, key);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	found = _0;
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_BOOL(found);
}

PHP_METHOD(Stub_Issue2706, fetchStops)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *key, key_sub, __$true, __$false, value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&value);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &key);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_fetch(&value, a, key, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		if (1) {
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
		} else {
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
		}
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2706, emptyStops)
{
	zend_bool result = 0, _0;
	zval *a, a_sub, *key, key_sub, __$true, __$false;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &key);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	_0 = zephir_isempty_dim(a, key);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	result = _0;
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_BOOL(result);
}

PHP_METHOD(Stub_Issue2706, literalStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, *value, value_sub, __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &key, &value);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	zephir_array_update_zval(&a, key, value, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2706, propertyWriteStops)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, __$true, __$false, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array(this_ptr, SL("container"), key, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2706, stringOffsetStops)
{
	unsigned char _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval s_zv, *key, key_sub, __$true, __$false, c, _1;
	zend_string *s = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&key_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	key = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	ZEPHIR_INIT_VAR(&_1);
	zephir_string_offset_read_zval(&_1, &s_zv, key, PH_NOISY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&c, &_1);
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&c);
}

PHP_METHOD(Stub_Issue2706, inArray)
{
	zend_bool _0;
	zval *needle, needle_sub, *haystack, haystack_sub;

	ZVAL_UNDEF(&needle_sub);
	ZVAL_UNDEF(&haystack_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(needle)
		Z_PARAM_ZVAL(haystack)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &needle, &haystack);
	_0 = zephir_fast_in_array(needle, haystack);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, inArrayStops)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *needle, needle_sub, *haystack, haystack_sub, __$true, __$false, found;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&needle_sub);
	ZVAL_UNDEF(&haystack_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&found);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(needle)
		Z_PARAM_ZVAL(haystack)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &needle, &haystack);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	_0 = zephir_fast_in_array(needle, haystack);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&found);
	ZVAL_BOOL(&found, _0);
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&found);
}

/**
 * `[key: value]` with a key that is not a literal, for every kind of
 * value. A dynamic key used to accept only a few value types.
 */
PHP_METHOD(Stub_Issue2706, literalVarKey)
{
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *sv = NULL;
	zend_bool bv;
	double dv;
	zend_long iv;
	zval *k, k_sub, *v, v_sub, *iv_param = NULL, *dv_param = NULL, *bv_param = NULL, sv_zv, __$true, __$null, _1;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&sv_zv);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
		Z_PARAM_LONG(iv)
		Z_PARAM_DOUBLE(dv)
		Z_PARAM_BOOL(bv)
		Z_PARAM_STR(sv)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	k = ZEND_CALL_ARG(execute_data, 1);
	v = ZEND_CALL_ARG(execute_data, 2);
	iv_param = ZEND_CALL_ARG(execute_data, 3);
	dv_param = ZEND_CALL_ARG(execute_data, 4);
	bv_param = ZEND_CALL_ARG(execute_data, 5);
	zephir_memory_observe(&sv_zv);
	ZVAL_STR_COPY(&sv_zv, sv);
	zephir_create_array(return_value, 11, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "s");
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, k, &__$true, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, k, &__$null, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, k, v, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_2, &_1);
	zephir_array_update_zval(&_0, k, &_2, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, iv);
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, dv);
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_BOOL(&_1, bv);
	zephir_array_update_zval(&_0, k, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, k, &sv_zv, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, literalStringKey)
{
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool bv;
	double dv;
	zend_long iv;
	zval k_zv, *v, v_sub, *iv_param = NULL, *dv_param = NULL, *bv_param = NULL, sv_zv, __$true, __$null, _1;
	zend_string *k = NULL, *sv = NULL;

	ZVAL_UNDEF(&k_zv);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&sv_zv);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_STR(k)
		Z_PARAM_ZVAL(v)
		Z_PARAM_LONG(iv)
		Z_PARAM_DOUBLE(dv)
		Z_PARAM_BOOL(bv)
		Z_PARAM_STR(sv)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	v = ZEND_CALL_ARG(execute_data, 2);
	iv_param = ZEND_CALL_ARG(execute_data, 3);
	dv_param = ZEND_CALL_ARG(execute_data, 4);
	bv_param = ZEND_CALL_ARG(execute_data, 5);
	zephir_memory_observe(&k_zv);
	ZVAL_STR_COPY(&k_zv, k);
	zephir_memory_observe(&sv_zv);
	ZVAL_STR_COPY(&sv_zv, sv);
	zephir_create_array(return_value, 11, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "s");
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, &k_zv, &__$true, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, &k_zv, &__$null, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, &k_zv, v, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_2, &_1);
	zephir_array_update_zval(&_0, &k_zv, &_2, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, iv);
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, dv);
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_BOOL(&_1, bv);
	zephir_array_update_zval(&_0, &k_zv, &_1, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_zval(&_0, &k_zv, &sv_zv, PH_COPY);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, literalIntKey)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *sv = NULL;
	zend_bool bv;
	double dv;
	zval *k_param = NULL, *v, v_sub, *iv_param = NULL, *dv_param = NULL, *bv_param = NULL, sv_zv, __$true, __$null, _2;
	zend_long k, iv;

	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&sv_zv);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_LONG(k)
		Z_PARAM_ZVAL(v)
		Z_PARAM_LONG(iv)
		Z_PARAM_DOUBLE(dv)
		Z_PARAM_BOOL(bv)
		Z_PARAM_STR(sv)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	k_param = ZEND_CALL_ARG(execute_data, 1);
	v = ZEND_CALL_ARG(execute_data, 2);
	iv_param = ZEND_CALL_ARG(execute_data, 3);
	dv_param = ZEND_CALL_ARG(execute_data, 4);
	bv_param = ZEND_CALL_ARG(execute_data, 5);
	zephir_memory_observe(&sv_zv);
	ZVAL_STR_COPY(&sv_zv, sv);
	zephir_create_array(return_value, 11, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_index_long(&_0, k, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_index_double(&_0, k, 1.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_index_stringl(&_0, k, SL("s"));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_long(&_0, k, &__$true, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_long(&_0, k, &__$null, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_long(&_0, k, v, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_array_fast_append(&_1, &_2);
	zephir_array_update_long(&_0, k, &_1, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, iv);
	zephir_array_update_long(&_0, k, &_2, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_DOUBLE(&_2, dv);
	zephir_array_update_long(&_0, k, &_2, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_BOOL(&_2, bv);
	zephir_array_update_long(&_0, k, &_2, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_update_long(&_0, k, &sv_zv, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * `double` and `bool` keys, held in a typed local or written as a literal.
 * PHP converts them to integer keys, deprecating a fractional float.
 */
PHP_METHOD(Stub_Issue2706, keyDoubleWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&a, &_0, &_1, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&a, &_0, &_1, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&a, &_0, &_1, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&a, &_0, &_1, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("zs"), 3, &_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("zs"), 3, &_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("zs"), 3, &_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("zs"), 3, &_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_assign_op(&a, &_1, add_function, SL("z"), 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_assign_op(&a, &_1, add_function, SL("z"), 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_assign_op(&a, &_1, add_function, SL("z"), 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_assign_op(&a, &_1, add_function, SL("z"), 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("za"), 2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("za"), 2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("za"), 2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("za"), 2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, k);
	zephir_array_fetch(&_0, &a, &_1, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 305);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyBoolRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, k);
	zephir_array_fetch(&_0, &a, &_1, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 310);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_array_fetch(&_0, &a, &_1, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 315);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, 1);
	zephir_array_fetch(&_0, &a, &_1, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 320);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyDoubleIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	_1 = zephir_array_isset_value(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyBoolIsset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_bool k, _1;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	_1 = zephir_array_isset_value(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	_1 = zephir_array_isset_value(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	_1 = zephir_array_isset_value(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyDoubleFetch)
{
	zend_bool found = 0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, x, _0, _2;
	double k;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	_1 = zephir_array_isset_fetch(&x, &a, &_0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _1;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, found);
	zephir_array_fast_append(return_value, &_2);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyBoolFetch)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, x, _0, _2;
	zend_bool k, found = 0, _1;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	_1 = zephir_array_isset_fetch(&x, &a, &_0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _1;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, found);
	zephir_array_fast_append(return_value, &_2);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleFetch)
{
	zend_bool found = 0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x, _0, _2;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	_1 = zephir_array_isset_fetch(&x, &a, &_0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _1;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, found);
	zephir_array_fast_append(return_value, &_2);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueFetch)
{
	zend_bool found = 0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x, _0, _2;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	_1 = zephir_array_isset_fetch(&x, &a, &_0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _1;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, found);
	zephir_array_fast_append(return_value, &_2);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyDoubleEmpty)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	_1 = zephir_isempty_dim(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyBoolEmpty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_bool k, _1;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	_1 = zephir_isempty_dim(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleEmpty)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	_1 = zephir_isempty_dim(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueEmpty)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	_1 = zephir_isempty_dim(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyDoubleUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	zephir_array_unset(&a, &_0, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	zephir_array_unset(&a, &_0, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_array_unset(&a, &_0, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_array_unset(&a, &_0, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyBoolUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyDoubleLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "v");
	zephir_array_update_zval(return_value, &_0, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyBoolLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "v");
	zephir_array_update_zval(return_value, &_0, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleLiteral)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "v");
	zephir_array_update_zval(return_value, &_0, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueLiteral)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "v");
	zephir_array_update_zval(return_value, &_0, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyDoublePropWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	double k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, k);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyBoolPropWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	zend_bool k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, k);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoublePropWrite)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralTruePropWrite)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyDoublePropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1, _2;
	double k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, k);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/issue2706.zep", 502);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyBoolPropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1, _2;
	zend_bool k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, k);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/issue2706.zep", 508);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoublePropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1, _2;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/issue2706.zep", 514);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTruePropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1, _2;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, 1);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/issue2706.zep", 520);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyDoublePropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	double k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyBoolPropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_bool k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoublePropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralTruePropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyDoubleStaticWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2, _3;
	double k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, k);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_3, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Issue2706, keyBoolStaticWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2, _3;
	zend_bool k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, k);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_3, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoubleStaticWrite)
{
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_3, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTrueStaticWrite)
{
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, 1);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_3, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_zval(&a, &_0, &_1, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("zs"), 3, &_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_assign_op(&a, &_1, add_function, SL("z"), 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_multi(&a, &_1, SL("za"), 2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_NULL(&_1);
	zephir_array_fetch(&_0, &a, &_1, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 605);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	_1 = zephir_array_isset_value(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullFetch)
{
	zend_bool found = 0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x, _0, _2;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	_1 = zephir_array_isset_fetch(&x, &a, &_0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _1;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, found);
	zephir_array_fast_append(return_value, &_2);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullEmpty)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	_1 = zephir_isempty_dim(&a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_array_unset(&a, &_0, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullLiteral)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "v");
	zephir_array_update_zval(return_value, &_0, &_1, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullPropWrite)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullPropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1, _2;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_NULL(&_2);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/issue2706.zep", 655);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullPropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullStaticWrite)
{
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_NULL(&_2);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_3, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

/**
 * `let a[k][] = v` on a parameter: an `array` parameter's dynamic type is
 * unknown at compile time, which only a `var` container should be checked for.
 */
PHP_METHOD(Stub_Issue2706, appendUnderArrayParam)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *k, k_sub, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &k);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&a, &_0, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, appendUnderVarParam)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &k);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(a, &_0, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, appendUnderDeepArrayParam)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&a, &_0, SL("sla"), 4, SL("x"), (zend_long) 1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

/**
 * `mixed`, `uint` and `ulong` keys: a `mixed` local is a zval as a `var`
 * is, a `uint`/`ulong` local a native integer as an `int` is.
 */
PHP_METHOD(Stub_Issue2706, keyMixedWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_zval(&a, k, &_0, PH_COPY | PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_long(&a, k, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_long(&a, k, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("zs"), 3, k, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("ls"), 3, (zend_long) k, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongWriteNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("ls"), 3, (zend_long) k, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_assign_op(&a, &_0, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_assign_op(&a, &_0, add_function, SL("l"), 1, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongAddAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_assign_op(&a, &_0, add_function, SL("l"), 1, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("la"), 2, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongAppendUnder)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("la"), 2, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_fetch(&_0, &a, k, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 772);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyUintRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_fetch_long(&_0, &a, k, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 777);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyUlongRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_fetch_long(&_0, &a, k, PH_NOISY | PH_READONLY, "stub/issue2706.zep", 782);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2706, keyMixedIsset)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_array_isset_value(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyUintIsset)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_array_isset_value_long(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyUlongIsset)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_array_isset_value_long(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyMixedFetch)
{
	zend_bool found = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, x, _1;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	_0 = zephir_array_isset_fetch(&x, &a, k, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _0;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, found);
	zephir_array_fast_append(return_value, &_1);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyUintFetch)
{
	zend_bool found = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, x, _1;
	zend_ulong k;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	_0 = zephir_array_isset_long_fetch(&x, &a, k, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _0;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, found);
	zephir_array_fast_append(return_value, &_1);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyUlongFetch)
{
	zend_bool found = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, x, _1;
	zend_ulong k;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_NULL(&x);
	ZEPHIR_OBS_NVAR(&x);
	_0 = zephir_array_isset_long_fetch(&x, &a, k, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	found = _0;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, found);
	zephir_array_fast_append(return_value, &_1);
	zephir_array_fast_append(return_value, &x);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyMixedEmpty)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_isempty_dim(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyUintEmpty)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_isempty_dim_long(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyUlongEmpty)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	_0 = zephir_isempty_dim_long(&a, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2706, keyMixedUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_unset(&a, k, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_unset_long(&a, k, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL;
	zend_ulong k;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_unset_long(&a, k, PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	{ zval *zephir_unset_offsets[] = { k, &_0 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUintUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyUlongUnsetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, k);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "x");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(&a, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2706, keyMixedLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval k_sub, _0;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k);
	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_zval(return_value, k, &_0, PH_COPY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2706, keyUintLiteral)
{
	zval *k_param = NULL;
	zend_ulong k;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &k_param);
	zephir_create_array(return_value, 1, 0);
	add_index_stringl(return_value, k, SL("v"));
	return;
}

PHP_METHOD(Stub_Issue2706, keyUlongLiteral)
{
	zval *k_param = NULL;
	zend_ulong k;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &k_param);
	zephir_create_array(return_value, 1, 0);
	add_index_stringl(return_value, k, SL("v"));
	return;
}

PHP_METHOD(Stub_Issue2706, keyMixedPropWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval k_sub, _0, _1;
	zval *k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array(this_ptr, SL("container"), k, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyUintPropWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, k);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyUlongPropWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, k);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("container"), &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyMixedPropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0, _1;
	zval *k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	zephir_array_fetch(&_1, &_0, k, PH_NOISY, "stub/issue2706.zep", 920);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyUintPropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	zephir_array_fetch_long(&_1, &_0, k, PH_NOISY, "stub/issue2706.zep", 926);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyUlongPropRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	zephir_array_fetch_long(&_1, &_0, k, PH_NOISY, "stub/issue2706.zep", 932);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2706, keyMixedPropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL;
	zval *k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyUintPropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyUlongPropUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "container");
}

PHP_METHOD(Stub_Issue2706, keyMixedStaticWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval k_sub, _0, _1, _2;
	zval *k;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2706, keyUintStaticWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("l"), 1, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2706, keyUlongStaticWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k_param = NULL, _0, _1, _2;
	zend_ulong k;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &k_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_issue2706_ce, SL("shared"), &_1, SL("l"), 1, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2706_ce, SL("shared"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2706, keyMixedPropIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval k_sub, *a_param = NULL, _0;
	zval *k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	_1 = zephir_array_isset_value(&_0, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyUintPropIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	_1 = zephir_array_isset_value_long(&_0, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyUlongPropIsset)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0;
	zend_ulong k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	_1 = zephir_array_isset_value_long(&_0, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2706, keyDoublePropIsset)
{
	zend_bool _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	double k;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, k);
	_2 = zephir_array_isset_value(&_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_2);
}

PHP_METHOD(Stub_Issue2706, keyBoolPropIsset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a;
	zval *k_param = NULL, *a_param = NULL, _0, _1;
	zend_bool k, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&a);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(k)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k_param, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, k);
	_2 = zephir_array_isset_value(&_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_2);
}

PHP_METHOD(Stub_Issue2706, keyLiteralDoublePropIsset)
{
	zend_bool _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	_2 = zephir_array_isset_value(&_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_2);
}

PHP_METHOD(Stub_Issue2706, keyLiteralTruePropIsset)
{
	zend_bool _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_BOOL(&_1, 1);
	_2 = zephir_array_isset_value(&_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_2);
}

PHP_METHOD(Stub_Issue2706, keyLiteralNullPropIsset)
{
	zend_bool _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0, _1;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &a);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_NULL(&_1);
	_2 = zephir_array_isset_value(&_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_2);
}

PHP_METHOD(Stub_Issue2706, keyExistsStops)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, *a, a_sub, __$true, __$false, found;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&a_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&found);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reached", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &key, &a);
	if (0) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	_0 = zephir_array_key_exists(a, key);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&found);
	ZVAL_BOOL(&found, _0);
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &__$false);
	}
	RETURN_CCTOR(&found);
}

zend_object *zephir_init_properties_Stub_Issue2706(zend_class_entry *class_type)
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
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("container"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("container"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

void zephir_init_static_properties_Stub_Issue2706()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2706_ce, ZEND_STRL("shared"), &_0);
	ZEPHIR_MM_RESTORE();
}

