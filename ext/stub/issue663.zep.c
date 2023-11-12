
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Issue663)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue663, stub, issue663, stub_issue663_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue663, is_array_assoc)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL;
	zval *arr, arr_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(arr)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arr);


	_0 = !(Z_TYPE_P(arr) == IS_ARRAY);
	if (!(_0)) {
		_0 = ZEPHIR_IS_EMPTY(arr);
	}
	if (_0) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_RETURN_CALL_STATIC("is_array_assoc_internal", &_1, 54, arr);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue663, is_array_assoc_internal)
{
	zend_bool _0;
	zend_long count = 0, i = 0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *arr_param = NULL;
	zval arr;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(arr)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arr_param);
	zephir_get_arrval(&arr, arr_param);


	count = zephir_fast_count_int(&arr);
	_2 = (count - 1);
	_1 = 0;
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
			if (!(zephir_array_isset_long(&arr, i))) {
				RETURN_MM_BOOL(1);
			}
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_Issue663, is_array_indexed)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *arr, arr_sub, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr_sub);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(arr)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arr);


	_0 = !(Z_TYPE_P(arr) == IS_ARRAY);
	if (!(_0)) {
		_0 = ZEPHIR_IS_EMPTY(arr);
	}
	if (_0) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_CALL_STATIC(&_1, "is_array_assoc_internal", &_2, 54, arr);
	zephir_check_call_status();
	RETURN_MM_BOOL(!zephir_is_true(&_1));
}

