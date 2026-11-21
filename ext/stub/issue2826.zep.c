
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


/**
 * `fetch` and `empty` read an element, so they must leave the array they read
 * exactly as it was, as PHP's own reads do.
 *
 * An empty array that is not reference counted, such as `[]` inside a PHP
 * constant array or inside a Zephir array constant, used to be replaced in the
 * source by a new array. That write lands in OPcache shared memory or in a
 * table that outlives the request.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2826
 */
ZEPHIR_INIT_CLASS(Stub_Issue2826)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2826, stub, issue2826, stub_issue2826_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		zval _zc1;
		array_init_size(&_zc1, 1);
		add_assoc_zval_ex(&_zc0, SL("key"), &_zc1);
		zval _zc2;
		array_init_size(&_zc2, 1);
		add_index_zval(&_zc0, 0, &_zc2);
		zephir_declare_class_constant_array(stub_issue2826_ce, SL("PARAMS"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2826, fetchDynamic)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, *key, key_sub, value;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(params)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &params, &key);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_fetch(&value, params, key, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, fetchLong)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_long_fetch(&value, params, 0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, fetchString)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_string_fetch(&value, params, SL("key"), 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, emptyDynamic)
{
	zend_bool _0;
	zval *params, params_sub, *key, key_sub;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(params)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &params, &key);
	_0 = zephir_isempty_dim(params, key);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, emptyLong)
{
	zend_bool _0;
	zval *params, params_sub;

	ZVAL_UNDEF(&params_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &params);
	_0 = zephir_isempty_dim_long(params, 0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, emptyString)
{
	zend_bool _0;
	zval *params, params_sub;

	ZVAL_UNDEF(&params_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &params);
	_0 = zephir_isempty_dim_string(params, SL("key"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2826, assignFetched)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value, _1$$3;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_string_fetch(&value, params, SL("key"), 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_array_update_string(&value, SL("y"), &_1$$3, PH_COPY | PH_SEPARATE);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2826, appendFetched)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value, _1$$3;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_string_fetch(&value, params, SL("key"), 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_array_append(&value, &_1$$3, PH_SEPARATE, "stub/issue2826.zep", 70);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2826, unsetFetched)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_string_fetch(&value, params, SL("key"), 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		zephir_array_unset_string(&value, SL("y"), PH_SEPARATE);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2826, assignNestedFetched)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *params, params_sub, value, _1$$3;

	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &params);
	zephir_memory_observe(&value);
	_0 = zephir_array_isset_string_fetch(&value, params, SL("key"), 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_array_update_multi(&value, &_1$$3, SL("ss"), 4, SL("a"), SL("b"));
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	RETURN_CCTOR(&value);
}

