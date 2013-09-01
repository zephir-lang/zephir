
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/string_type.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Flow) {

	ZEPHIR_REGISTER_CLASS(Test, Flow, flow, test_flow_method_entry, 0);


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
				RETURN_LONG(1);
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
				RETURN_LONG(1);
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
	if (a + b) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf14) {

	zval a = zval_used_for_init, b = zval_used_for_init, _0 = zval_used_for_init;

	ZEPHIR_SINIT_VAR(a);
	ZVAL_LONG(&a, 1);
	ZEPHIR_SINIT_VAR(b);
	ZVAL_LONG(&b, 2);
	ZEPHIR_SINIT_VAR(_0);
	zephir_add_function(&_0, &a, &b TSRMLS_CC);
	if (zend_is_true(&_0)) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testIf15) {

	zval a = zval_used_for_init, b = zval_used_for_init, c = zval_used_for_init, _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_SINIT_VAR(a);
	ZVAL_LONG(&a, 1);
	ZEPHIR_SINIT_VAR(b);
	ZVAL_LONG(&b, 2);
	ZEPHIR_SINIT_VAR(c);
	ZVAL_LONG(&c, 3);
	ZEPHIR_SINIT_VAR(_0);
	zephir_add_function(&_0, &a, &b TSRMLS_CC);
	ZEPHIR_SINIT_VAR(_1);
	zephir_add_function(&_1, &_0, &c TSRMLS_CC);
	if (zend_is_true(&_1)) {
		RETURN_LONG(1);
	}
	RETURN_LONG(0);

}

PHP_METHOD(Test_Flow, testLoop1) {

	zval a = zval_used_for_init;

	ZEPHIR_SINIT_VAR(a);
	ZVAL_BOOL(&a, 1);
	while (1) {
		if (zend_is_true(&a)) {
			break;
		}
	}
	RETURN_CCTORW(&a);

}

PHP_METHOD(Test_Flow, testLoop2) {

	int a;

	a = 1;
	while (1) {
		a = a + 1;
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
		a = a + 1;
		if (a == 3) {
			a = a + 1;
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
		a = a + 1;
		if (a == 3) {
			a = a + 1;
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
	while (a) {
		a = a - 1;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile2) {

	int a;

	a = 5;
	while (a) {
		a = a - 1;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile3) {

	double c;
	zval b = zval_used_for_init;
	int a, _0;

	a = 5;
	while (a) {
		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);
		while (zend_is_true(&b)) {
			_0 = zephir_get_numberval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_LONG(&b, _0);
		}
		a = a - 1;
	}
	c = (double) (a + zephir_get_intval(&b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile4) {

	double c;
	zval b = zval_used_for_init;
	int a, _0;

	a = 5;
	while (a) {
		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);
		while (zend_is_true(&b)) {
			_0 = zephir_get_numberval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_LONG(&b, _0);
		}
		a = a - 1;
	}
	c = (double) (a + zephir_get_intval(&b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testWhile5) {

	int a;

	a = 5;
	while (a) {
		a--;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile6) {

	int a;

	a = 5;
	while (a) {
		a--;
	}
	RETURN_LONG(a);

}

PHP_METHOD(Test_Flow, testWhile7) {

	double a;

	a = (double) (5);
	while (a) {
		a--;
	}
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile8) {

	double a;

	a = (double) (5);
	while (a) {
		a--;
	}
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Flow, testWhile9) {

	double c;
	zval b = zval_used_for_init;
	int a;

	a = 5;
	while (a) {
		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);
		while (zend_is_true(&b)) {
			zephir_decrement(&b);
		}
		a--;
	}
	c = (double) (a + zephir_get_intval(&b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testDoWhile1) {

	while (1) {
	}

}

PHP_METHOD(Test_Flow, testFor1) {

	HashTable *_5;
	HashPosition _4;
	int c;
	zval *v = NULL, *b, *_0, *_1, *_2, *_3, **_6;

	ZEPHIR_MM_GROW();
	c = 0;
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_append(&b, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 3);
	zephir_array_append(&b, _2, 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 4);
	zephir_array_append(&b, _3, 0);
	zephir_is_iterable(b, &_5, &_4, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		; zend_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HVALUE(v, _6);
		c += zephir_get_numberval(v);
	}
	RETURN_MM_LONG(c);

}

PHP_METHOD(Test_Flow, testFor2) {

	HashTable *_5;
	HashPosition _4;
	double c;
	zval *v = NULL, *k = NULL, *b, *_0, *_1, *_2, *_3, **_6;

	ZEPHIR_MM_GROW();
	c = (double) (0);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_append(&b, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 3);
	zephir_array_append(&b, _2, 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 4);
	zephir_array_append(&b, _3, 0);
	zephir_is_iterable(b, &_5, &_4, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		; zend_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HMKEY(k, _5, _4);
		ZEPHIR_GET_HVALUE(v, _6);
		c += zephir_get_numberval(k);
	}
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Flow, testFor3) {

	HashTable *_5;
	HashPosition _4;
	zval *v = NULL, *b, *c, *_0, *_1, *_2, *_3, **_6;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(c);
	array_init(c);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_append(&b, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 3);
	zephir_array_append(&b, _2, 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 4);
	zephir_array_append(&b, _3, 0);
	zephir_is_iterable(b, &_5, &_4, 0, 1);
	for (
		; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		; zend_hash_move_backwards_ex(_5, &_4)
	) {
		ZEPHIR_GET_HVALUE(v, _6);
		zephir_array_append(&c, v, PH_SEPARATE);
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor4) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(b);
	ZVAL_LONG(b, 0);
	_1 = 1;
	_0 = 0;
	while (1) {
		if (_0) {
			_1++;
			if (!(_1 <= 10)) {
				break;
			}
		} else {
			_0 = 1;
		}
		ZEPHIR_INIT_NVAR(a);
		ZVAL_LONG(a, _1);
		ZEPHIR_ADD_ASSIGN(b, a);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor5) {

	zend_bool _0;
	int a, b, _1;

	b = 0;
	_1 = 1;
	_0 = 0;
	while (1) {
		if (_0) {
			_1++;
			if (!(_1 <= 10)) {
				break;
			}
		} else {
			_0 = 1;
		}
		a = _1;
		b += a;
	}
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor6) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	_1 = 10;
	_0 = 0;
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
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor7) {

	zend_bool _0;
	int a, b, _1;

	b = 0;
	_1 = 10;
	_0 = 0;
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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor8) {

	zend_bool _0;
	int a, b, c, d, _1;

	b = 0;
	c = 1;
	d = 10;
	_1 = c;
	_0 = 0;
	while (1) {
		if (_0) {
			_1++;
			if (!(_1 <= d)) {
				break;
			}
		} else {
			_0 = 1;
		}
		a = _1;
		b += a;
	}
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor9) {

	zend_bool _0;
	int a, b, c, d, _1;

	b = 0;
	c = 1;
	d = 10;
	_1 = d;
	_0 = 0;
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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor10) {

	zend_bool _0;
	zval *a = NULL, *b = NULL, *c = NULL, *d = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	ZEPHIR_INIT_NVAR(c);
	ZVAL_LONG(c, 1);
	ZEPHIR_INIT_NVAR(d);
	ZVAL_LONG(d, 10);
	ZEPHIR_CPY_WRT(_1, c);
	_0 = 0;
	while (1) {
		if (_0) {
			ZEPHIR_SEPARATE(_1);
			zephir_increment(_1);
			if (!(ZEPHIR_LE(_1, d))) {
				break;
			}
		} else {
			_0 = 1;
		}
		ZEPHIR_CPY_WRT(a, _1);
		ZEPHIR_ADD_ASSIGN(b, a);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor11) {

	char _1;
	zend_bool _0;
	zephir_str *b = NULL;
	char a;

	zephir_str_assign(b, "", sizeof("")-1);
	_1 = 'a';
	_0 = 0;
	while (1) {
		if (_0) {
			_1++;
			if (!(_1 <= 'z')) {
				break;
			}
		} else {
			_0 = 1;
		}
		a = _1;
		zephir_str_append_char(b, a);
	}
	RETURN_STRING(b->str, 1);

}

PHP_METHOD(Test_Flow, testFor12) {

	char _1;
	zend_bool _0;
	zephir_str *b = NULL;
	char a;

	zephir_str_assign(b, "", sizeof("")-1);
	_1 = 'z';
	_0 = 0;
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
		zephir_str_append_char(b, a);
	}
	RETURN_STRING(b->str, 1);

}

PHP_METHOD(Test_Flow, testFor13) {

	char _1;
	zend_bool _0;
	zephir_str *b = NULL;
	char a;

	zephir_str_assign(b, "", sizeof("")-1);
	_1 = '0';
	_0 = 0;
	while (1) {
		if (_0) {
			_1++;
			if (!(_1 <= '9')) {
				break;
			}
		} else {
			_0 = 1;
		}
		a = _1;
		zephir_str_append_char(b, a);
	}
	RETURN_STRING(b->str, 1);

}

PHP_METHOD(Test_Flow, testFor14) {

	char _1;
	zend_bool _0;
	zephir_str *b = NULL;
	char a;

	zephir_str_assign(b, "", sizeof("")-1);
	_1 = '9';
	_0 = 0;
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
		zephir_str_append_char(b, a);
	}
	RETURN_STRING(b->str, 1);

}

PHP_METHOD(Test_Flow, testFor15) {

	zend_bool _0;
	zval *c_param = NULL, *d_param = NULL;
	int c, d, a, b, _1;

	zephir_fetch_params(0, 2, 0, &c_param, &d_param);

		c = zephir_get_intval(c_param);
		d = zephir_get_intval(d_param);


	b = 0;
	_1 = d;
	_0 = 0;
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
	RETURN_LONG(b);

}

PHP_METHOD(Test_Flow, testFor16) {

	zend_bool _1;
	int _0, _2;
	zval *a = NULL, *b = NULL, *c = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(c);
	array_init(c);
	_2 = 1;
	_0 = 0;
	_1 = 0;
	while (1) {
		if (_1) {
			_0++;
			_2++;
			if (!(_2 <= 10)) {
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
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor17) {

	zend_bool _1;
	int _0, _2;
	zval *a = NULL, *b = NULL, *c = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(c);
	array_init(c);
	_2 = 10;
	_0 = 0;
	_1 = 0;
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
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor18) {

	zend_bool _1;
	zval *c = NULL, *_3 = NULL;
	int a, b, _0, _2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(c);
	array_init(c);
	_2 = 1;
	_0 = 0;
	_1 = 0;
	while (1) {
		if (_1) {
			_0++;
			_2++;
			if (!(_2 <= 10)) {
				break;
			}
		} else {
			_1 = 1;
		}
		a = _0;
		b = _2;
		ZEPHIR_INIT_NVAR(_3);
		ZVAL_LONG(_3, b);
		zephir_array_update_long(&c, a, &_3, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Flow, testFor19) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	_1 = 1;
	_0 = 0;
	while (1) {
		if (_0) {
			_1 += 2;
			if (!(_1 <= 10)) {
				break;
			}
		} else {
			_0 = 1;
		}
		ZEPHIR_INIT_NVAR(a);
		ZVAL_LONG(a, _1);
		ZEPHIR_ADD_ASSIGN(b, a);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor20) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	_1 = 1;
	_0 = 0;
	while (1) {
		if (_0) {
			_1 += 2;
			if (!(_1 <= 10)) {
				break;
			}
		} else {
			_0 = 1;
		}
		ZEPHIR_INIT_NVAR(a);
		ZVAL_LONG(a, _1);
		ZEPHIR_ADD_ASSIGN(b, a);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor21) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	_1 = 10;
	_0 = 0;
	while (1) {
		if (_0) {
			_1 += -2;
			if (!(_1 <= 1)) {
				break;
			}
		} else {
			_0 = 1;
		}
		ZEPHIR_INIT_NVAR(a);
		ZVAL_LONG(a, _1);
		ZEPHIR_ADD_ASSIGN(b, a);
	}
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor22) {

	int _1;
	zend_bool _0;
	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(b);
	ZVAL_LONG(b, 0);
	_1 = 1;
	_0 = 0;
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
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_Flow, testFor23) {

	char _1;
	zend_bool _0;
	zephir_str *b = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_str_assign(b, "", sizeof("")-1);
	_1 = 'z';
	_0 = 0;
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
		zephir_str_append_char(b, (char) zephir_get_intval(a));
	}
	RETURN_MM_STRING(b->str, 1);

}

PHP_METHOD(Test_Flow, testFor30) {

	int v, _0;
	zephir_str *b = NULL;
	zval *a = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(a);
	array_init(a);
	zephir_str_assign(b, "hello", sizeof("hello")-1);
	for (_0 = 0; _0 < b->len; _0++) {
		v = b->str[_0]; 
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, v);
		zephir_array_append(&a, _1, PH_SEPARATE);
	}
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Flow, testFor31) {

	int k, v, _0;
	zephir_str *b = NULL;
	zval *a = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_NVAR(a);
	array_init(a);
	zephir_str_assign(b, "hello", sizeof("hello")-1);
	for (_0 = 0; _0 < b->len; _0++) {
		k = _0; 
		v = b->str[_0]; 
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, v);
		zephir_array_update_long(&a, k, &_1, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(a);

}

