
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


ZEPHIR_INIT_CLASS(Test_Sort) {

	ZEPHIR_REGISTER_CLASS(Test, Sort, test, sort, test_sort_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Sort, quick) {

	zephir_fcall_cache_entry *_7 = NULL;
	zend_bool _1;
	int i, length, pivot, item, _2, _3, ZEPHIR_LAST_CALL_STATUS;
	zval *arr_param = NULL, *left, *right, *_0, *_4 = NULL, *_5 = NULL, *_6 = NULL, *_9 = NULL;
	zval *arr = NULL, *_8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arr_param);

	zephir_get_arrval(arr, arr_param);


	length = zephir_fast_count_int(arr TSRMLS_CC);
	if (length <= 1) {
		RETURN_CTOR(arr);
	}
	ZEPHIR_OBS_VAR(_0);
	zephir_array_fetch_long(&_0, arr, 0, PH_NOISY, "test/sort.zep", 18 TSRMLS_CC);
	pivot = zephir_get_intval(_0);
	ZEPHIR_INIT_VAR(left);
	array_init(left);
	ZEPHIR_INIT_VAR(right);
	array_init(right);
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
			ZEPHIR_OBS_NVAR(_4);
			zephir_array_fetch_long(&_4, arr, i, PH_NOISY, "test/sort.zep", 23 TSRMLS_CC);
			item = zephir_get_intval(_4);
			if (item < pivot) {
				ZEPHIR_INIT_NVAR(_5);
				ZVAL_LONG(_5, item);
				zephir_array_append(&left, _5, PH_SEPARATE, "test/sort.zep", 25);
			} else {
				ZEPHIR_INIT_NVAR(_5);
				ZVAL_LONG(_5, item);
				zephir_array_append(&right, _5, PH_SEPARATE, "test/sort.zep", 27);
			}
		}
	}
	ZEPHIR_CALL_METHOD(&_6, this_ptr, "quick", &_7, 68, left);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_8);
	zephir_create_array(_8, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_5);
	ZVAL_LONG(_5, pivot);
	zephir_array_fast_append(_8, _5);
	ZEPHIR_CALL_METHOD(&_9, this_ptr, "quick", &_7, 68, right);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_FUNCTION("array_merge", NULL, 69, _6, _8, _9);
	zephir_check_call_status();
	RETURN_MM();

}

