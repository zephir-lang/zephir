
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

	zend_bool _3$$3;
	zval _0;
	int i = 0, j = 0, _4$$3, _5$$3;
	zval box, side, _1, *_2, _6$$4;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&box);
	ZVAL_UNDEF(&side);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&box);
	zephir_create_array(&box, 15, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 10);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 24);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 8);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 8);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 15);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 19);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 19);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 17);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 6);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 6);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 16);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 20);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 20);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 13);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 9);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 9);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 11);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 22);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 22);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 18);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 18);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 20);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 21);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 16);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 16);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 23);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 23);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 7);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 12);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 12);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 25);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 24);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 7);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 11);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 11);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 27);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 11);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 27);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&box, &_0);
	zephir_is_iterable(&box, 0, "test/fortytwo.zep", 32);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&box), _2)
	{
		ZEPHIR_INIT_NVAR(&side);
		ZVAL_COPY(&side, _2);
		j = 0;
		_5$$3 = 2;
		_4$$3 = 0;
		_3$$3 = 0;
		if (_4$$3 <= _5$$3) {
			while (1) {
				if (_3$$3) {
					_4$$3++;
					if (!(_4$$3 <= _5$$3)) {
						break;
					}
				} else {
					_3$$3 = 1;
				}
				i = _4$$3;
				zephir_array_fetch_long(&_6$$4, &side, i, PH_NOISY | PH_READONLY, "test/fortytwo.zep", 26 TSRMLS_CC);
				j += zephir_get_numberval(&_6$$4);
			}
		}
		if (j != 42) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(test_exception_ce, "not true", "test/fortytwo.zep", 29);
			return;
		}
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&side);
	ZEPHIR_MM_RESTORE();

}

