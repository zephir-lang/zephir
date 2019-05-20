
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
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/iterator.h"
#include "kernel/math.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Flow) {

	ZEPHIR_REGISTER_CLASS(Test, Flow, test, flow, test_flow_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Flow, testIf1) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf2) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 0;
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf3) {

	double a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = (double) (1);
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf4) {

	double a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = (double) (0);
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf5) {

	zend_bool a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = ((1) ? 1 : 0);
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf6) {

	zend_bool a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = ((0) ? 1 : 0);
	if (a) {
		RETURN_MM_LONG(1);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf7) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	if (a) {
		RETURN_MM_LONG(1);
	} else {
		RETURN_MM_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf8) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 0;
	if (a) {
		RETURN_MM_LONG(1);
	} else {
		RETURN_MM_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf9) {

	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = ((1) ? 1 : 0);
			if (c) {
				RETURN_MM_LONG(1);
			}
		}
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf10) {

	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = ((1) ? 1 : 0);
			if (c) {
				RETURN_MM_LONG(654);
			} else {
				RETURN_MM_LONG(-1);
			}
		} else {
			RETURN_MM_LONG(-2);
		}
	} else {
		RETURN_MM_LONG(-3);
	}

}

PHP_METHOD(Test_Flow, testIf12) {

	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = ((1) ? 1 : 0);
			if (c) {
				RETURN_MM_LONG(987);
			} else {
				RETURN_MM_LONG(0);
			}
		} else {
			RETURN_MM_LONG(0);
		}
	} else {
		RETURN_MM_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf13) {

	zend_long a = 0, b = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	b = 2;
	if ((a + b)) {
		RETURN_MM_LONG(-12);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf14) {

	zend_long a = 0, b = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	b = 2;
	if ((a + b)) {
		RETURN_MM_LONG(74);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf15) {

	zend_long a = 0, b = 0, c = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	b = 2;
	c = 3;
	if (((a + b) + c)) {
		RETURN_MM_LONG(89);
	}
	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Flow, testIf16) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(1, 1, 0, &a);



	if (ZEPHIR_IS_EMPTY(a)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Flow, testIf17) {

	zval *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 1, 0, &b);



	if (Z_TYPE_P(b) != IS_NULL) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Flow, testLoop1) {

	zend_bool a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	while (1) {
		if (a) {
			break;
		}
	}
	RETURN_MM_BOOL(a);

}

PHP_METHOD(Test_Flow, testLoop2) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	while (1) {
		a = (a + 1);
		if (a == 5) {
			break;
		}
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testLoop3) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	while (1) {
		a = (a + 1);
		if (a == 3) {
			a = (a + 1);
			continue;
		}
		if (a == 5) {
			break;
		}
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testLoop4) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	while (1) {
		a = (a + 1);
		if (a == 3) {
			a = (a + 1);
			continue;
		}
		if (a == 5) {
			break;
		}
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile1) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a = (a - 1);
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile2) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a = (a - 1);
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile3) {

	double c = 0;
	zend_long a = 0, b = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	b = 0;
	while (1) {
		if (!(a)) {
			break;
		}
		b = 5;
		while (1) {
			if (!(b)) {
				break;
			}
			b = (b - 1);
		}
		a = (a - 1);
	}
	c = (double) ((a + b));
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile4) {

	double c = 0;
	zend_long a = 0, b = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	b = 0;
	while (1) {
		if (!(a)) {
			break;
		}
		b = 5;
		while (1) {
			if (!(b)) {
				break;
			}
			b = (b - 1);
		}
		a = (a - 1);
	}
	c = (double) ((a + b));
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile5) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile6) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile7) {

	double a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = (double) (5);
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_MM_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile8) {

	double a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = (double) (5);
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_MM_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile9) {

	double c = 0;
	zend_long a = 0, b = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	b = 0;
	while (1) {
		if (!(a)) {
			break;
		}
		b = 5;
		while (1) {
			if (!(b)) {
				break;
			}
			b--;
		}
		a--;
	}
	c = (double) ((a + b));
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile10) {

	double c = 0;
	zend_long b = 0;
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(1, 1, 0, &a);

	ZEPHIR_SEPARATE_PARAM(a);


	b = 0;
	while (1) {
		if (!(zephir_is_true(a))) {
			break;
		}
		b = 5;
		while (1) {
			if (!(b)) {
				break;
			}
			b--;
		}
		ZEPHIR_SEPARATE(a);
		zephir_decrement(a);
	}
	c = (double) ((zephir_get_numberval(a) + b));
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile11) {

	double c = 0;
	zend_long b = 0;
	zval *a, a_sub, *d, d_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&d_sub);

	zephir_fetch_params(1, 2, 0, &a, &d);

	ZEPHIR_SEPARATE_PARAM(a);


	b = 0;
	while (1) {
		if (!(zephir_is_true(a))) {
			break;
		}
		b = zephir_get_numberval(d);
		while (1) {
			if (!(b)) {
				break;
			}
			b--;
		}
		ZEPHIR_SEPARATE(a);
		zephir_decrement(a);
	}
	c = (double) ((zephir_get_numberval(a) + b));
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile12) {

	zval _0;
	zval b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);


	ZEPHIR_CONCAT_SS(&_0, "+", "10");
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZEPHIR_CPY_WRT(&b, &_0);
	while (1) {
		if (!(ZEPHIR_GT_LONG(&b, 5))) {
			break;
		}
		ZEPHIR_SEPARATE(&b);
		zephir_decrement(&b);
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testWhile13) {

	zval _0;
	zend_long a;
	zval b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);


	a = 5;
	ZEPHIR_CONCAT_SS(&_0, "+", "10");
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZEPHIR_CPY_WRT(&b, &_0);
	while (1) {
		if (!(ZEPHIR_GT_LONG(&b, a))) {
			break;
		}
		ZEPHIR_SEPARATE(&b);
		zephir_decrement(&b);
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testDoWhile1) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	do {
	} while (1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testWhileNextTest) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL, *_3 = NULL;
	zval *variable, variable_sub, returnValue, _0, _2$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&variable_sub);
	ZVAL_UNDEF(&returnValue);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);

	zephir_fetch_params(1, 1, 0, &variable);



	array_init(&returnValue);
	ZEPHIR_MM_ADD_ENTRY(&returnValue);
	while (1) {
		ZEPHIR_MAKE_REF(variable);
		ZEPHIR_CALL_FUNCTION(&_0, "next", &_1, 41, variable);
		ZEPHIR_UNREF(variable);
		zephir_check_call_status();
		if (!(zephir_is_true(&_0))) {
			break;
		}
		ZEPHIR_CALL_FUNCTION(&_2$$3, "current", &_3, 42, variable);
		zephir_check_call_status();
		zephir_array_append(&returnValue, &_2$$3, PH_SEPARATE, "test/flow.zep", 420);
	}
	RETURN_MM_CTOR(&returnValue);

}

PHP_METHOD(Test_Flow, testWhileDoNextTest) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL, *_3 = NULL;
	zval *variable, variable_sub, returnValue, _2, _0$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&variable_sub);
	ZVAL_UNDEF(&returnValue);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0$$3);

	zephir_fetch_params(1, 1, 0, &variable);



	array_init(&returnValue);
	ZEPHIR_MM_ADD_ENTRY(&returnValue);
	do {
		ZEPHIR_CALL_FUNCTION(&_0$$3, "current", &_1, 42, variable);
		zephir_check_call_status();
		zephir_array_append(&returnValue, &_0$$3, PH_SEPARATE, "test/flow.zep", 430);
		ZEPHIR_MAKE_REF(variable);
		ZEPHIR_CALL_FUNCTION(&_2, "next", &_3, 41, variable);
		ZEPHIR_UNREF(variable);
		zephir_check_call_status();
	} while (zephir_is_true(&_2));
	RETURN_MM_CTOR(&returnValue);

}

PHP_METHOD(Test_Flow, testFor1) {

	zval v, b, _0, *_1, _2;
	zend_long ZEPHIR_LAST_CALL_STATUS, c = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);


	c = 0;
	zephir_create_array(&b, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&b);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&b, &_0);
	zephir_is_iterable(&b, 0, "test/flow.zep", 443);
	if (Z_TYPE_P(&b) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&b), _1)
		{
			ZEPHIR_CPY_WRT(&v, _1);
			c += zephir_get_numberval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &b, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &b, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, &b, "current", NULL, 0);
			zephir_check_call_status();
				c += zephir_get_numberval(&v);
			ZEPHIR_CALL_METHOD(NULL, &b, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	RETURN_MM_LONG(c);

}

PHP_METHOD(Test_Flow, testFor2) {

	zend_string *_4;
	zend_ulong _3;
	double c = 0;
	zval v, k, b, _0, *_1, _2;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);


	c = (double) (0);
	zephir_create_array(&b, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&b);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&b, &_0);
	zephir_is_iterable(&b, 0, "test/flow.zep", 453);
	if (Z_TYPE_P(&b) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&b), _3, _4, _1)
		{
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&k, _4);
			} else {
				ZVAL_LONG(&k, _3);
			}
			ZEPHIR_CPY_WRT(&v, _1);
			c += zephir_get_numberval(&k);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &b, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &b, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, &b, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, &b, "current", NULL, 0);
			zephir_check_call_status();
				c += zephir_get_numberval(&k);
			ZEPHIR_CALL_METHOD(NULL, &b, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testFor3) {

	zval v, b, c, _0, *_1, _2;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);


	array_init(&c);
	ZEPHIR_MM_ADD_ENTRY(&c);
	zephir_create_array(&b, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&b);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&b, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&b, &_0);
	zephir_is_iterable(&b, 0, "test/flow.zep", 464);
	if (Z_TYPE_P(&b) == IS_ARRAY) {
		ZEND_HASH_REVERSE_FOREACH_VAL(Z_ARRVAL_P(&b), _1)
		{
			ZEPHIR_CPY_WRT(&v, _1);
			zephir_array_append(&c, &v, PH_SEPARATE, "test/flow.zep", 462);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &b, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &b, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, &b, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_append(&c, &v, PH_SEPARATE, "test/flow.zep", 462);
			ZEPHIR_CALL_METHOD(NULL, &b, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	RETURN_MM_CTOR(&c);

}

PHP_METHOD(Test_Flow, testFor4) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 10;
	_1 = 1;
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
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor5) {

	zend_bool _0;
	zend_long a = 0, b = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	b = 0;
	_2 = 10;
	_1 = 1;
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
			a = _1;
			b += a;
		}
	}
	RETURN_MM_LONG(b);

}

PHP_METHOD(Test_Flow, testFor6) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 10;
	_1 = _2;
	_0 = 0;
	if (_1 >= 1) {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= 1)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor7) {

	zend_bool _0;
	zend_long a = 0, b = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	b = 0;
	_2 = 10;
	_1 = _2;
	_0 = 0;
	if (_1 >= 1) {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= 1)) {
					break;
				}
			} else {
				_0 = 1;
			}
			a = _1;
			b += a;
		}
	}
	RETURN_MM_LONG(b);

}

PHP_METHOD(Test_Flow, testFor8) {

	zend_bool _0;
	zend_long a = 0, b = 0, c = 0, d = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	b = 0;
	c = 1;
	d = 10;
	_2 = d;
	_1 = c;
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
			a = _1;
			b += a;
		}
	}
	RETURN_MM_LONG(b);

}

PHP_METHOD(Test_Flow, testFor9) {

	zend_bool _0;
	zend_long a = 0, b = 0, c = 0, d = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	b = 0;
	c = 1;
	d = 10;
	_2 = d;
	_1 = _2;
	_0 = 0;
	if (_1 >= c) {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= c)) {
					break;
				}
			} else {
				_0 = 1;
			}
			a = _1;
			b += a;
		}
	}
	RETURN_MM_LONG(b);

}

PHP_METHOD(Test_Flow, testFor10) {

	zend_bool _0;
	zend_long c = 0, d = 0, _1, _2;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	c = 1;
	d = 10;
	_2 = d;
	_1 = c;
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
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor11) {

	zend_bool _0;
	zval b;
	char a = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	_2 = 'z';
	_1 = 'a';
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
			a = _1;
			ZEPHIR_MM_CONCAT_SELF_CHAR(&b, a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor12) {

	zend_bool _0;
	zval b;
	char a = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	_2 = 'z';
	_1 = _2;
	_0 = 0;
	if (_1 >= 'a') {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= 'a')) {
					break;
				}
			} else {
				_0 = 1;
			}
			a = _1;
			ZEPHIR_MM_CONCAT_SELF_CHAR(&b, a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor13) {

	zend_bool _0;
	zval b;
	char a = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	_2 = '9';
	_1 = '0';
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
			a = _1;
			ZEPHIR_MM_CONCAT_SELF_CHAR(&b, a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor14) {

	zend_bool _0;
	zval b;
	char a = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	_2 = '9';
	_1 = _2;
	_0 = 0;
	if (_1 >= '0') {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= '0')) {
					break;
				}
			} else {
				_0 = 1;
			}
			a = _1;
			ZEPHIR_MM_CONCAT_SELF_CHAR(&b, a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor15) {

	zend_bool _0;
	zval *c_param = NULL, *d_param = NULL;
	zend_long c, d, a = 0, b = 0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &c_param, &d_param);

	c = zephir_get_intval(c_param);
	d = zephir_get_intval(d_param);


	b = 0;
	_2 = d;
	_1 = _2;
	_0 = 0;
	if (_1 >= c) {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= c)) {
					break;
				}
			} else {
				_0 = 1;
			}
			a = _1;
			b += a;
		}
	}
	RETURN_MM_LONG(b);

}

PHP_METHOD(Test_Flow, testFor16) {

	zend_long _2, _3;
	zend_bool _1;
	long _0;
	zval a, b, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	array_init(&c);
	ZEPHIR_MM_ADD_ENTRY(&c);
	_3 = 10;
	_2 = 1;
	_0 = 0;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_0++;
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			ZVAL_LONG(&a, _0);
			ZVAL_LONG(&b, _2);
			zephir_array_update_zval(&c, &a, &b, PH_SEPARATE);
		}
	}
	RETURN_MM_CTOR(&c);

}

PHP_METHOD(Test_Flow, testFor17) {

	zend_long _2, _3;
	zend_bool _1;
	long _0;
	zval a, b, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	array_init(&c);
	ZEPHIR_MM_ADD_ENTRY(&c);
	_3 = 10;
	_2 = _3;
	_0 = 0;
	_1 = 0;
	if (_2 >= 1) {
		while (1) {
			if (_1) {
				_0++;
				_2--;
				if (!(_2 >= 1)) {
					break;
				}
			} else {
				_1 = 1;
			}
			ZVAL_LONG(&a, _0);
			ZVAL_LONG(&b, _2);
			zephir_array_update_zval(&c, &a, &b, PH_SEPARATE);
		}
	}
	RETURN_MM_CTOR(&c);

}

PHP_METHOD(Test_Flow, testFor18) {

	zend_bool _1;
	long _0;
	zval c, _4$$3;
	zend_long a = 0, b = 0, _2, _3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_4$$3);


	array_init(&c);
	ZEPHIR_MM_ADD_ENTRY(&c);
	_3 = 10;
	_2 = 1;
	_0 = 0;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_0++;
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			a = _0;
			b = _2;
			ZVAL_LONG(&_4$$3, b);
			zephir_array_update_long(&c, a, &_4$$3, PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		}
	}
	RETURN_MM_CTOR(&c);

}

PHP_METHOD(Test_Flow, testFor19) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 10;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1 += 2;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor20) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 10;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1 += 2;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor21) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 1;
	_1 = 10;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1 += -2;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor22) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZVAL_LONG(&b, 0);
	_2 = 1;
	_1 = _2;
	_0 = 0;
	if (_1 >= 10) {
		while (1) {
			if (_0) {
				_1 -= 2;
				if (!(_1 >= 10)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			ZEPHIR_ADD_ASSIGN(&b, &a);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor23) {

	char _1, _2, _3$$3;
	zend_bool _0;
	zval b;
	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	_2 = 'z';
	_1 = _2;
	_0 = 0;
	if (_1 >= 'a') {
		while (1) {
			if (_0) {
				_1 -= 2;
				if (!(_1 >= 'a')) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZVAL_LONG(&a, _1);
			_3$$3 = (char) zephir_get_intval(&a);
			ZEPHIR_MM_CONCAT_SELF_CHAR(&b, _3$$3);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor24) {

	zval _0;
	zval b;
	zval a, _1, *_2, _3;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_ZVAL_STRING(&b, "");
	zephir_create_array(&_0, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZVAL_LONG(&_1, 'a');
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 'b');
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 'c');
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 'd');
	zephir_array_fast_append(&_0, &_1);
	zephir_is_iterable(&_0, 0, "test/flow.zep", 674);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _2)
		{
			ZEPHIR_CPY_WRT(&a, _2);
			ZEPHIR_MM_CONCAT_SELF(&b, &a);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_3, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&a, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_MM_CONCAT_SELF(&b, &a);
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testFor30) {

	long _0;
	zend_long v = 0;
	zval b;
	zval a, _1$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&b);


	array_init(&a);
	ZEPHIR_MM_ADD_ENTRY(&a);
	ZEPHIR_MM_ZVAL_STRING(&b, "hello");
	for (_0 = 0; _0 < Z_STRLEN_P(&b); _0++) {
		v = ZEPHIR_STRING_OFFSET(&b, _0);
		ZVAL_LONG(&_1$$3, v);
		zephir_array_append(&a, &_1$$3, PH_SEPARATE, "test/flow.zep", 682);
	}
	RETURN_MM_CTOR(&a);

}

PHP_METHOD(Test_Flow, testFor31) {

	long _0;
	zend_long k = 0, v = 0;
	zval b;
	zval a, _1$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&b);


	array_init(&a);
	ZEPHIR_MM_ADD_ENTRY(&a);
	ZEPHIR_MM_ZVAL_STRING(&b, "hello");
	for (_0 = 0; _0 < Z_STRLEN_P(&b); _0++) {
		k = _0; 
		v = ZEPHIR_STRING_OFFSET(&b, _0);
		ZVAL_LONG(&_1$$3, v);
		zephir_array_update_long(&a, k, &_1$$3, PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	}
	RETURN_MM_CTOR(&a);

}

PHP_METHOD(Test_Flow, testFor32) {

	long sum;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *e, e_sub, v, *_0, _1;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&e_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params(1, 1, 0, &e);



	sum = 0;
	zephir_is_iterable(e, 0, "test/flow.zep", 703);
	if (Z_TYPE_P(e) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(e), _0)
		{
			ZEPHIR_CPY_WRT(&v, _0);
			sum += zephir_get_numberval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, e, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_1, e, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_1)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, e, "current", NULL, 0);
			zephir_check_call_status();
				sum += zephir_get_numberval(&v);
			ZEPHIR_CALL_METHOD(NULL, e, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	RETURN_MM_LONG(sum);

}

PHP_METHOD(Test_Flow, testFor33) {

	zend_object_iterator *_0;
	zval *e, e_sub, v, result;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&e_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&result);

	zephir_fetch_params(1, 1, 0, &e);



	array_init(&result);
	ZEPHIR_MM_ADD_ENTRY(&result);
	_0 = zephir_get_iterator(e TSRMLS_CC);
	_0->funcs->rewind(_0 TSRMLS_CC);
	for (;_0->funcs->valid(_0 TSRMLS_CC) == SUCCESS && !EG(exception); _0->funcs->move_forward(_0 TSRMLS_CC)) {
		{
			ZEPHIR_ITERATOR_COPY(&v, _0);
		}
		zephir_array_append(&result, &v, PH_SEPARATE, "test/flow.zep", 710);
	}
	zend_iterator_dtor(_0);
	RETURN_MM_CTOR(&result);

}

PHP_METHOD(Test_Flow, testFor34) {

	zend_object_iterator *_0$$3;
	zval *e = NULL, e_sub, __$null, v, result;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&e_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&result);

	zephir_fetch_params(1, 0, 1, &e);

	if (!e) {
		e = &e_sub;
		e = &__$null;
	}


	array_init(&result);
	ZEPHIR_MM_ADD_ENTRY(&result);
	if (Z_TYPE_P(e) == IS_OBJECT) {
		_0$$3 = zephir_get_iterator(e TSRMLS_CC);
		_0$$3->funcs->rewind(_0$$3 TSRMLS_CC);
		for (;_0$$3->funcs->valid(_0$$3 TSRMLS_CC) == SUCCESS && !EG(exception); _0$$3->funcs->move_forward(_0$$3 TSRMLS_CC)) {
			{
				ZEPHIR_ITERATOR_COPY(&v, _0$$3);
			}
			zephir_array_append(&result, &v, PH_SEPARATE, "test/flow.zep", 720);
		}
		zend_iterator_dtor(_0$$3);
		RETURN_MM_CTOR(&result);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testFor35Aux) {

	zval *hello, hello_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&hello_sub);

	zephir_fetch_params(1, 1, 0, &hello);



	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Flow, testFor35) {

	zend_bool _0;
	zval i, _3$$3;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_3$$3);


	_2 = 100;
	_1 = 1;
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
			ZVAL_LONG(&i, _1);
			ZEPHIR_MM_ZVAL_STRING(&_3$$3, "hello");
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "testfor35aux", &_4, 0, &_3$$3);
			zephir_check_call_status();
		}
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Flow, testFor36Aux) {

	zval *hello, hello_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&hello_sub);

	zephir_fetch_params(1, 1, 0, &hello);



	zephir_create_array(return_value, 1, 0);
	zephir_array_fast_append(return_value, hello);
	RETURN_MM();

}

PHP_METHOD(Test_Flow, testFor36) {

	zend_bool _0;
	zval i, _3$$3;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_3$$3);


	_2 = 100;
	_1 = 1;
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
			ZVAL_LONG(&i, _1);
			ZEPHIR_MM_ZVAL_STRING(&_3$$3, "hello");
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "testfor36aux", &_4, 0, &_3$$3);
			zephir_check_call_status();
		}
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Flow, testFor37) {

	zend_bool _0;
	zend_long i, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	i = 0;
	_2 = 10;
	_1 = 1;
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
			_1 = _1;
			i++;
		}
	}
	RETURN_MM_LONG(i);

}

PHP_METHOD(Test_Flow, testFor38) {

	zend_bool _1;
	long _0;
	zval v;
	zend_long i, _2, _3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&v);


	i = 0;
	_3 = 10;
	_2 = 1;
	_0 = 0;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_0++;
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			_0 = _0;
			ZVAL_LONG(&v, _2);
			i++;
		}
	}
	RETURN_MM_LONG(i);

}

PHP_METHOD(Test_Flow, testFor39) {

	zval _2;
	long _1;
	char _0;
	zend_long i;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_2);


	i = 0;
	ZEPHIR_MM_ZVAL_STRING(&_2, "hello");
	for (_1 = 0; _1 < Z_STRLEN_P(&_2); _1++) {
		_0 = ZEPHIR_STRING_OFFSET(&_2, _1);
		i++;
	}
	RETURN_MM_LONG(i);

}

PHP_METHOD(Test_Flow, testFor40) {

	zend_long _1, _2;
	zend_bool _0;
	zval a, b, _3$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_3$$3);


	ZVAL_LONG(&b, 0);
	_2 = 10000000;
	_1 = 1;
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
			ZVAL_LONG(&a, _1);
			ZVAL_DOUBLE(&_3$$3, zephir_sqrt(&a TSRMLS_CC));
			ZEPHIR_ADD_ASSIGN(&b, &_3$$3);
		}
	}
	RETURN_MM_CTOR(&b);

}

PHP_METHOD(Test_Flow, testUnrechable1) {

	char e;
	zval d;
	zend_bool c;
	double b;
	zend_long a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&d);


	a = 0;
	b = 0.0;
	c = 0;
	ZEPHIR_MM_ZVAL_STRING(&d, "");
	e = '\0';
	if (a) {
		php_printf("%s", "hello true");
	}
	if (b) {
		php_printf("%s", "hello true");
	}
	if (c) {
		php_printf("%s", "hello true");
	}
	if (zephir_is_true(&d)) {
		php_printf("%s", "hello true");
	}
	if (e) {
		php_printf("%s", "hello true");
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testUnrechable2) {

	char e;
	zval d;
	zend_bool c;
	double b;
	zend_long a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&d);


	a = 1;
	b = 1.0;
	c = 1;
	ZEPHIR_MM_ZVAL_STRING(&d, "hello");
	e = 'A';
	if (a) {
		php_printf("%s", "hello true");
	} else {
		php_printf("%s", "hello false");
	}
	if (b) {
		php_printf("%s", "hello true");
	} else {
		php_printf("%s", "hello false");
	}
	if (c) {
		php_printf("%s", "hello true");
	} else {
		php_printf("%s", "hello false");
	}
	if (zephir_is_true(&d)) {
		php_printf("%s", "hello true");
	} else {
		php_printf("%s", "hello false");
	}
	if (e) {
		php_printf("%s", "hello true");
	} else {
		php_printf("%s", "hello false");
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testUnrechable3) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	if (3 < 1) {
		if (1 > 2) {
			a = 0;
		}
	}
	RETURN_MM_LONG(a);

}

