
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
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/object.h"


/**
 * FortyTwo
 */
ZEPHIR_INIT_CLASS(Stub_FortyTwo)
{
	ZEPHIR_REGISTER_CLASS(Stub, FortyTwo, stub, fortytwo, stub_fortytwo_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_FortyTwo, proof)
{
	zend_bool _4$$3;
	zval _0;
	zval box, side, _1, *_2, *_3, _7$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i = 0, j = 0, _5$$3, _6$$3;

	ZVAL_UNDEF(&box);
	ZVAL_UNDEF(&side);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&box);
	zephir_create_array(&box, 15, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	zephir_create_array(&_0, 3, 0);
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
	if (Z_TYPE_P(&box) == IS_STRING) {
		ZEPHIR_INIT_NVAR(&_1);
		zephir_string_to_char_array(&_1, &box);
		_2 = &_1;
	} else {
		_2 = &box;
	}
	zephir_is_iterable(_2, 0, "stub/fortytwo.zep", 32);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_2), _3)
	{
		ZEPHIR_INIT_NVAR(&side);
		ZVAL_COPY(&side, _3);
		j = 0;
		_6$$3 = 2;
		_5$$3 = 0;
		_4$$3 = 0;
		if (_5$$3 <= _6$$3) {
			while (1) {
				if (_4$$3) {
					_5$$3++;
					if (!(_5$$3 <= _6$$3)) {
						break;
					}
				} else {
					_4$$3 = 1;
				}
				i = _5$$3;
				zephir_array_fetch_long(&_7$$4, &side, i, PH_NOISY | PH_READONLY, "stub/fortytwo.zep", 26);
				j += zephir_get_numberval(&_7$$4);
			}
		}
		if (j != 42) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(stub_exception_ce, "not true", "stub/fortytwo.zep", 29);
			return;
		}
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&side);
	ZEPHIR_MM_RESTORE();
}

