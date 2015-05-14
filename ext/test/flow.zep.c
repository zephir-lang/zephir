
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
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/iterator.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Flow) {

	ZEPHIR_REGISTER_CLASS(Test, Flow, test, flow, test_flow_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Flow, testIf1) {

	int a;


	a = 1;
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf2) {

	int a;


	a = 0;
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf3) {

	double a;


	a = (double) (1);
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf4) {

	double a;


	a = (double) (0);
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf5) {

	zend_bool a;


	a = (1) ? 1 : 0;
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf6) {

	zend_bool a;


	a = (0) ? 1 : 0;
	if (a) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf7) {

	int a;


	a = 1;
	if (a) {
		RETURN_LONG(1);
	} else {
		RETURN_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf8) {

	int a;


	a = 0;
	if (a) {
		RETURN_LONG(1);
	} else {
		RETURN_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf9) {

	zend_bool c;
	double b;
	int a;


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = (1) ? 1 : 0;
			if (c) {
				RETURN_LONG(1);
			}
		}
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf10) {

	zend_bool c;
	double b;
	int a;


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = (1) ? 1 : 0;
			if (c) {
				RETURN_LONG(654);
			} else {
				RETURN_LONG(-1);
			}
		} else {
			RETURN_LONG(-2);
		}
	} else {
		RETURN_LONG(-3);
	}

}

PHP_METHOD(Test_Flow, testIf12) {

	zend_bool c;
	double b;
	int a;


	a = 1;
	if (a) {
		b = (double) (1);
		if (b) {
			c = (1) ? 1 : 0;
			if (c) {
				RETURN_LONG(987);
			} else {
				RETURN_LONG(0);
			}
		} else {
			RETURN_LONG(0);
		}
	} else {
		RETURN_LONG(0);
	}

}

PHP_METHOD(Test_Flow, testIf13) {

	int a, b;


	a = 1;
	b = 2;
	if ((a + b)) {
		RETURN_LONG(-12);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf14) {

	int a, b;


	a = 1;
	b = 2;
	if ((a + b)) {
		RETURN_LONG(74);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf15) {

	int a, b, c;


	a = 1;
	b = 2;
	c = 3;
	if (((a + b) + c)) {
		RETURN_LONG(89);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf16) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	if (ZEPHIR_IS_EMPTY(a)) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);

}

PHP_METHOD(Test_Flow, testIf17) {

	zval *b;

	zephir_fetch_params(0, 1, 0, &b);



	if (Z_TYPE_P(b) != IS_NULL) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);

}

PHP_METHOD(Test_Flow, testLoop1) {

	zend_bool a;


	a = 1;
	while (1) {
		if (a) {
			break;
		}
	}
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Flow, testLoop2) {

	int a;


	a = 1;
	while (1) {
		a = (a + 1);
		if (a == 5) {
			break;
		}
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testLoop3) {

	int a;


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
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testLoop4) {

	int a;


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
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile1) {

	int a;


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a = (a - 1);
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile2) {

	int a;


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a = (a - 1);
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile3) {

	double c;
	int a, b;


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
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile4) {

	double c;
	int a, b;


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
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile5) {

	int a;


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile6) {

	int a;


	a = 5;
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile7) {

	double a;


	a = (double) (5);
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile8) {

	double a;


	a = (double) (5);
	while (1) {
		if (!(a)) {
			break;
		}
		a--;
	}
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile9) {

	double c;
	int a, b;


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
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile10) {

	double c;
	int b;
	zval *a;

	ZEPHIR_MM_GROW();
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

	double c;
	int b;
	zval *a, *d;

	ZEPHIR_MM_GROW();
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

	zval *_0;
	zval *b = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SS(_0, "+", "10");
	ZEPHIR_CPY_WRT(b, _0);
	while (1) {
		if (!(ZEPHIR_GT_LONG(b, 5))) {
			break;
		}
		ZEPHIR_SEPARATE(b);
		zephir_decrement(b);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testWhile13) {

	zval *_0;
	int a = 5;
	zval *b = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SS(_0, "+", "10");
	ZEPHIR_CPY_WRT(b, _0);
	while (1) {
		if (!(ZEPHIR_GT_LONG(b, a))) {
			break;
		}
		ZEPHIR_SEPARATE(b);
		zephir_decrement(b);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testDoWhile1) {


	do {
	} while (1);

}

PHP_METHOD(Test_Flow, testWhileNextTest) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL, *_3 = NULL;
	zval *variable, *returnValue, *_0 = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &variable);



	ZEPHIR_INIT_VAR(returnValue);
	array_init(returnValue);
	while (1) {
		Z_SET_ISREF_P(variable);
		ZEPHIR_CALL_FUNCTION(&_0, "next", &_1, 41, variable);
		Z_UNSET_ISREF_P(variable);
		zephir_check_call_status();
		if (!(zephir_is_true(_0))) {
			break;
		}
		Z_SET_ISREF_P(variable);
		ZEPHIR_CALL_FUNCTION(&_2, "current", &_3, 42, variable);
		Z_UNSET_ISREF_P(variable);
		zephir_check_call_status();
		zephir_array_append(&returnValue, _2, PH_SEPARATE, "test/flow.zep", 420);
	}
	RETURN_CCTOR(returnValue);

}

PHP_METHOD(Test_Flow, testWhileDoNextTest) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL, *_2 = NULL;
	zval *variable, *returnValue, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &variable);

	ZEPHIR_INIT_VAR(returnValue);
	array_init(returnValue);


	do {
		Z_SET_ISREF_P(variable);
		ZEPHIR_CALL_FUNCTION(&_0, "current", &_1, 42, variable);
		Z_UNSET_ISREF_P(variable);
		zephir_check_call_status();
		zephir_array_append(&returnValue, _0, PH_SEPARATE, "test/flow.zep", 430);
		Z_SET_ISREF_P(variable);
		ZEPHIR_CALL_FUNCTION(&_0, "next", &_2, 41, variable);
		Z_UNSET_ISREF_P(variable);
		zephir_check_call_status();
	} while (zephir_is_true(_0));
	RETURN_CCTOR(returnValue);

}

PHP_METHOD(Test_Flow, testFor1) {

	HashTable *_2;
	HashPosition _1;
	int c;
	zval *v = NULL, *b, *_0 = NULL, **_3;

	ZEPHIR_MM_GROW();

	c = 0;
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(b, _0);
	zephir_is_iterable(b, &_2, &_1, 0, 0, "test/flow.zep", 443);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(v, _3);
		c += zephir_get_numberval(v);
	}
	RETURN_MM_LONG(c);

}

PHP_METHOD(Test_Flow, testFor2) {

	HashTable *_2;
	HashPosition _1;
	double c;
	zval *v = NULL, *k = NULL, *b, *_0 = NULL, **_3;

	ZEPHIR_MM_GROW();

	c = (double) (0);
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(b, _0);
	zephir_is_iterable(b, &_2, &_1, 0, 0, "test/flow.zep", 453);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(k, _2, _1);
		ZEPHIR_GET_HVALUE(v, _3);
		c += zephir_get_numberval(k);
	}
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testFor3) {

	HashTable *_2;
	HashPosition _1;
	zval *v = NULL, *b, *c, *_0 = NULL, **_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	array_init(c);
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(b, _0);
	zephir_is_iterable(b, &_2, &_1, 0, 1, "test/flow.zep", 464);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_backwards_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(v, _3);
		zephir_array_append(&c, v, PH_SEPARATE, "test/flow.zep", 462);
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor4) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor5) {

	zend_bool _0;
	int a, b, _1, _2;


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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor6) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor7) {

	zend_bool _0;
	int a, b, _1, _2;


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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor8) {

	zend_bool _0;
	int a, b, c, d, _1, _2;


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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor9) {

	zend_bool _0;
	int a, b, c, d, _1, _2;


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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor10) {

	zend_bool _0;
	int c, d, _1, _2;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor11) {

	zend_bool _0;
	zval *b;
	char a, _1, _2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
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
			zephir_concat_self_char(&b, a TSRMLS_CC);
		}
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor12) {

	zend_bool _0;
	zval *b;
	char a, _1, _2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
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
			zephir_concat_self_char(&b, a TSRMLS_CC);
		}
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor13) {

	zend_bool _0;
	zval *b;
	char a, _1, _2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
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
			zephir_concat_self_char(&b, a TSRMLS_CC);
		}
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor14) {

	zend_bool _0;
	zval *b;
	char a, _1, _2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
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
			zephir_concat_self_char(&b, a TSRMLS_CC);
		}
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor15) {

	zend_bool _0;
	zval *c_param = NULL, *d_param = NULL;
	int c, d, a, b, _1, _2;

	zephir_fetch_params(0, 2, 0, &c_param, &d_param);

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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor16) {

	int _2, _3;
	zend_bool _1;
	long _0;
	zval *a = NULL, *b = NULL, *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	array_init(c);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _0);
			ZEPHIR_INIT_NVAR(b);
			ZVAL_LONG(b, _2);
			zephir_array_update_zval(&c, a, &b, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor17) {

	int _2, _3;
	zend_bool _1;
	long _0;
	zval *a = NULL, *b = NULL, *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	array_init(c);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _0);
			ZEPHIR_INIT_NVAR(b);
			ZVAL_LONG(b, _2);
			zephir_array_update_zval(&c, a, &b, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor18) {

	zend_bool _1;
	long _0;
	zval *c, *_4 = NULL;
	int a, b, _2, _3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	array_init(c);
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
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, b);
			zephir_array_update_long(&c, a, &_4, PH_COPY | PH_SEPARATE, "test/flow.zep", 612);
		}
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor19) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor20) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor21) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor22) {

	int _1, _2;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_ADD_ASSIGN(b, a);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor23) {

	char _1, _2, _3;
	zend_bool _0;
	zval *b;
	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			_3 = (char) zephir_get_intval(a);
			zephir_concat_self_char(&b, _3 TSRMLS_CC);
		}
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor24) {

	HashTable *_3;
	HashPosition _2;
	zval *_0;
	zval *b;
	zval *a = NULL, *_1 = NULL, **_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_EMPTY_STRING(b);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 'a');
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 'b');
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 'c');
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 'd');
	zephir_array_fast_append(_0, _1);
	zephir_is_iterable(_0, &_3, &_2, 0, 0, "test/flow.zep", 674);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(a, _4);
		zephir_concat_self(&b, a TSRMLS_CC);
	}
	RETURN_CTOR(b);

}

PHP_METHOD(Test_Flow, testFor30) {

	long _0;
	int v;
	zval *b;
	zval *a, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	for (_0 = 0; _0 < Z_STRLEN_P(b); _0++) {
		v = ZEPHIR_STRING_OFFSET(b, _0);
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, v);
		zephir_array_append(&a, _1, PH_SEPARATE, "test/flow.zep", 682);
	}
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Flow, testFor31) {

	long _0;
	int k, v;
	zval *b;
	zval *a, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	for (_0 = 0; _0 < Z_STRLEN_P(b); _0++) {
		k = _0; 
		v = ZEPHIR_STRING_OFFSET(b, _0);
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, v);
		zephir_array_update_long(&a, k, &_1, PH_COPY | PH_SEPARATE, "test/flow.zep", 692);
	}
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Flow, testFor32) {

	HashTable *_1;
	HashPosition _0;
	long sum = 0;
	zval *e, *v = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &e);



	zephir_is_iterable(e, &_1, &_0, 0, 0, "test/flow.zep", 703);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(v, _2);
		sum += zephir_get_numberval(v);
	}
	RETURN_MM_LONG(sum);

}

PHP_METHOD(Test_Flow, testFor33) {

	zend_object_iterator *_0;
	zval *e, *v = NULL, *result;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &e);

	ZEPHIR_INIT_VAR(result);
	array_init(result);


	_0 = zephir_get_iterator(e TSRMLS_CC);
	_0->funcs->rewind(_0 TSRMLS_CC);
	for (;_0->funcs->valid(_0 TSRMLS_CC) == SUCCESS && !EG(exception); _0->funcs->move_forward(_0 TSRMLS_CC)) {
		{
			zval **ZEPHIR_TMP_ITERATOR_PTR;
			_0->funcs->get_current_data(_0, &ZEPHIR_TMP_ITERATOR_PTR TSRMLS_CC);
			ZEPHIR_CPY_WRT(v, (*ZEPHIR_TMP_ITERATOR_PTR));
		}
		zephir_array_append(&result, v, PH_SEPARATE, "test/flow.zep", 710);
	}
	_0->funcs->dtor(_0 TSRMLS_CC);
	RETURN_CCTOR(result);

}

PHP_METHOD(Test_Flow, testFor34) {

	zend_object_iterator *_0;
	zval *e = NULL, *v = NULL, *result;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &e);

	if (!e) {
		e = ZEPHIR_GLOBAL(global_null);
	}
	ZEPHIR_INIT_VAR(result);
	array_init(result);


	if (Z_TYPE_P(e) == IS_OBJECT) {
		_0 = zephir_get_iterator(e TSRMLS_CC);
		_0->funcs->rewind(_0 TSRMLS_CC);
		for (;_0->funcs->valid(_0 TSRMLS_CC) == SUCCESS && !EG(exception); _0->funcs->move_forward(_0 TSRMLS_CC)) {
			{
				zval **ZEPHIR_TMP_ITERATOR_PTR;
				_0->funcs->get_current_data(_0, &ZEPHIR_TMP_ITERATOR_PTR TSRMLS_CC);
				ZEPHIR_CPY_WRT(v, (*ZEPHIR_TMP_ITERATOR_PTR));
			}
			zephir_array_append(&result, v, PH_SEPARATE, "test/flow.zep", 720);
		}
		_0->funcs->dtor(_0 TSRMLS_CC);
		RETURN_CCTOR(result);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testFor35Aux) {

	zval *hello;

	zephir_fetch_params(0, 1, 0, &hello);



	RETURN_BOOL(0);

}

PHP_METHOD(Test_Flow, testFor35) {

	zephir_fcall_cache_entry *_4 = NULL;
	int _1, _2, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *i = NULL, *_3 = NULL;

	ZEPHIR_MM_GROW();

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
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_STRING(_3, "hello", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "testfor35aux", &_4, 0, _3);
			zephir_check_temp_parameter(_3);
			zephir_check_call_status();
		}
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Flow, testFor36Aux) {

	zval *hello;

	zephir_fetch_params(0, 1, 0, &hello);



	zephir_create_array(return_value, 1, 0 TSRMLS_CC);
	zephir_array_fast_append(return_value, hello);
	return;

}

PHP_METHOD(Test_Flow, testFor36) {

	zephir_fcall_cache_entry *_4 = NULL;
	int _1, _2, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *i = NULL, *_3 = NULL;

	ZEPHIR_MM_GROW();

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
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_STRING(_3, "hello", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "testfor36aux", &_4, 0, _3);
			zephir_check_temp_parameter(_3);
			zephir_check_call_status();
		}
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Flow, testFor37) {

	zend_bool _0;
	int i = 0, _1, _2;


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
	RETURN_LONG(i);

}

PHP_METHOD(Test_Flow, testFor38) {

	zend_bool _1;
	long _0;
	zval *v = NULL;
	int i = 0, _2, _3;

	ZEPHIR_MM_GROW();

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
			ZEPHIR_INIT_NVAR(v);
			ZVAL_LONG(v, _2);
			i++;
		}
	}
	RETURN_MM_LONG(i);

}

PHP_METHOD(Test_Flow, testFor39) {

	zval _2;
	long _1;
	char _0;
	int i = 0;


	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "hello", 0);
	for (_1 = 0; _1 < Z_STRLEN_P(&_2); _1++) {
		_0 = ZEPHIR_STRING_OFFSET(&_2, _1);
		i++;
	}
	RETURN_LONG(i);

}

PHP_METHOD(Test_Flow, testFor40) {

	zephir_fcall_cache_entry *_4 = NULL;
	int _1, _2, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *a = NULL, *b, *_3 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);

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
			ZEPHIR_INIT_NVAR(a);
			ZVAL_LONG(a, _1);
			ZEPHIR_CALL_FUNCTION(&_3, "sqrt", &_4, 9, a);
			zephir_check_call_status();
			ZEPHIR_ADD_ASSIGN(b, _3);
		}
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testSwitch1) {

	int a;


	a = 10;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testSwitch2) {

	int a;


	a = 10;
	do {
		if (a == 10) {
			RETURN_BOOL(1);
		}
	} while(0);

	RETURN_BOOL(0);

}

PHP_METHOD(Test_Flow, testSwitch3) {

	int a;


	a = 10;
	do {
		if (a == 5) {
			RETURN_BOOL(1);
		}
	} while(0);

	RETURN_BOOL(0);

}

PHP_METHOD(Test_Flow, testSwitch4) {

	int a;


	a = 10;
	do {
		if (a == 5) {
			RETURN_LONG(1);
		}
		RETURN_LONG(2);
	} while(0);

	RETURN_LONG(3);

}

PHP_METHOD(Test_Flow, testSwitch5) {

	int a;


	a = 10;
	do {
		if (a == 10) {
			RETURN_LONG(1);
		}
		RETURN_LONG(2);
	} while(0);

	RETURN_LONG(3);

}

PHP_METHOD(Test_Flow, testSwitch6) {

	int a;


	a = 10;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Test_Flow, testSwitch7) {

	int a;


	a = 4;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Test_Flow, testSwitch8) {

	int a;


	a = 5;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Test_Flow, testSwitch9) {

	int a;


	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Test_Flow, testSwitch10) {

	int a;


	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Test_Flow, testSwitch11) {

	int a;


	a = 1;
	do {
		if (a == 2) {
			RETURN_LONG(2);
		}
		RETURN_LONG(1);
	} while(0);


}

PHP_METHOD(Test_Flow, testSwitch12) {

	zend_bool _0, _1, _2;
	zval *var1, *var2;

	zephir_fetch_params(0, 2, 0, &var1, &var2);



	do {
		_0 = 1;
		_1 = ZEPHIR_GT(var1, var2);
		if (_1) {
			_1 = ZEPHIR_GT_LONG(var2, 5);
		}
		if (_0 == _1) {
			RETURN_LONG(1);
		}
		_2 = ZEPHIR_LT(var1, var2);
		if (_2) {
			_2 = ZEPHIR_LT_LONG(var1, 5);
		}
		if (_0 == _2) {
			RETURN_LONG(2);
		}
		RETURN_LONG(0);
	} while(0);


}

PHP_METHOD(Test_Flow, testUnrechable1) {

	char e = '\0';
	zval *d;
	zend_bool c = 0;
	double b = 0.0;
	int a = 0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(d);
	ZVAL_STRING(d, "", 1);

	if (a) {
		php_printf("%s", "hello true");
	}
	if (b) {
		php_printf("%s", "hello true");
	}
	if (c) {
		php_printf("%s", "hello true");
	}
	if (zephir_is_true(d)) {
		php_printf("%s", "hello true");
	}
	if (e) {
		php_printf("%s", "hello true");
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow, testUnrechable2) {

	char e = 'A';
	zval *d;
	zend_bool c = 1;
	double b = 1.0;
	int a = 1;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(d);
	ZVAL_STRING(d, "hello", 1);

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
	if (zephir_is_true(d)) {
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

	int a = 0;


	if (3 < 1) {
		if (1 > 2) {
			a = 0;
		}
	}
	RETURN_LONG(a);

}

