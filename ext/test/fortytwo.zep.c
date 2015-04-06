
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
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/exception.h"


/**
 * FortyTwo
 */
ZEPHIR_INIT_CLASS(Test_FortyTwo) {

	ZEPHIR_REGISTER_CLASS(Test, FortyTwo, test, fortytwo, test_fortytwo_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_FortyTwo, proof) {

	zend_bool _5;
	HashTable *_3;
	HashPosition _2;
	zval *_0 = NULL;
	int i, j, _6, _7;
	zval *box, *side = NULL, *_1 = NULL, **_4, *_8;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(box);
	zephir_create_array(box, 15, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 10);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 24);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 8);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 8);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 15);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 19);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 19);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 17);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 6);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 6);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 16);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 20);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 20);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 13);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 9);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 9);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 11);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 22);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 22);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 18);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 18);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 20);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 5);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 21);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 16);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 16);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 23);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 23);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 7);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 12);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 12);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 25);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 5);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 24);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 7);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 11);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 11);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 27);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 11);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 27);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(box, _0);
	zephir_is_iterable(box, &_3, &_2, 0, 0, "test/fortytwo.zep", 32);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(side, _4);
		j = 0;
		_7 = 2;
		_6 = 0;
		_5 = 0;
		if (_6 <= _7) {
			while (1) {
				if (_5) {
					_6++;
					if (!(_6 <= _7)) {
						break;
					}
				} else {
					_5 = 1;
				}
				i = _6;
				zephir_array_fetch_long(&_8, side, i, PH_NOISY | PH_READONLY, "test/fortytwo.zep", 26 TSRMLS_CC);
				j += zephir_get_numberval(_8);
			}
		}
		if (j != 42) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_exception_ce, "not true", "test/fortytwo.zep", 29);
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

