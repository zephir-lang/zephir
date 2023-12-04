
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Sort)
{
	ZEPHIR_REGISTER_CLASS(Stub, Sort, stub, sort, stub_sort_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Sort, quick)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_8 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i = 0, length = 0, pivot = 0, item = 0, _2, _3;
	zval *arr_param = NULL, left, right, _0, _7, _10, _11, _4$$4, _5$$5, _6$$6;
	zval arr, _9;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&left);
	ZVAL_UNDEF(&right);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_5$$5);
	ZVAL_UNDEF(&_6$$6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(arr)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &arr_param);
	zephir_get_arrval(&arr, arr_param);


	length = zephir_fast_count_int(&arr);
	if (length <= 1) {
		RETURN_CTOR(&arr);
	}
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, &arr, 0, PH_NOISY, "stub/sort.zep", 18);
	pivot = zephir_get_intval(&_0);
	ZEPHIR_INIT_VAR(&left);
	array_init(&left);
	ZEPHIR_INIT_VAR(&right);
	array_init(&right);
	_3 = (length - 1);
	_2 = 1;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			i = _2;
			ZEPHIR_OBS_NVAR(&_4$$4);
			zephir_array_fetch_long(&_4$$4, &arr, i, PH_NOISY, "stub/sort.zep", 23);
			item = zephir_get_intval(&_4$$4);
			if (item < pivot) {
				ZEPHIR_INIT_NVAR(&_5$$5);
				ZVAL_LONG(&_5$$5, item);
				zephir_array_append(&left, &_5$$5, PH_SEPARATE, "stub/sort.zep", 25);
			} else {
				ZEPHIR_INIT_NVAR(&_6$$6);
				ZVAL_LONG(&_6$$6, item);
				zephir_array_append(&right, &_6$$6, PH_SEPARATE, "stub/sort.zep", 27);
			}
		}
	}
	ZEPHIR_CALL_METHOD(&_7, this_ptr, "quick", &_8, 92, &left);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_9);
	zephir_create_array(&_9, 1, 0);
	ZEPHIR_INIT_VAR(&_10);
	ZVAL_LONG(&_10, pivot);
	zephir_array_fast_append(&_9, &_10);
	ZEPHIR_CALL_METHOD(&_11, this_ptr, "quick", &_8, 92, &right);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_FUNCTION("array_merge", NULL, 93, &_7, &_9, &_11);
	zephir_check_call_status();
	RETURN_MM();
}

