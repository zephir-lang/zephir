
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
#include "kernel/string_type.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Flow) {

	ZEPHIR_REGISTER_CLASS(Test, Flow, flow, test_flow_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Flow, testIf1) {

	long a;

	a = 1;

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf2) {

	long a;

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

	long a;

	a = 1;

	if (a) {

		RETURN_LONG(1);

	} else {

		RETURN_LONG(0);

	}

}

PHP_METHOD(Test_Flow, testIf8) {

	long a;

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
	long a;

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
	long a;

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
	long a;

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

PHP_METHOD(Test_Flow, testLoop1) {

	zval a;

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

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testLoop3) {

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 3)) {

			a = a + 1;

			continue;
		}
		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testLoop4) {

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 3)) {

			a = a + 1;

			continue;
		}
		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile1) {

	long a;

	a = 5;

	while (a) {

		a = a - 1;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile2) {

	long a;

	a = 5;

	while (a) {

		a = a - 1;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile3) {

	double c, _0;
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			_0 = zephir_get_doubleval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_DOUBLE(&b, _0);

		}

		a = a - 1;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testWhile4) {

	double c, _0;
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			_0 = zephir_get_doubleval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_DOUBLE(&b, _0);

		}

		a = a - 1;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testWhile5) {

	long a;

	a = 5;

	while (a) {

		a--;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile6) {

	long a;

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
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			ZEPHIR_SINIT_NVAR(b);
			zephir_decrement(&b);

		}

		a--;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testFor1) {

	HashTable *_5;
	HashPosition _4;
	long c;
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

		c += zephir_get_doubleval(v);

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
		ZEPHIR_GET_HKEY(k, _5, _4);
		ZEPHIR_GET_HVALUE(v, _6);

		c += zephir_get_doubleval(k);

	}

	RETURN_MM_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testFor3) {

	HashTable *_5;
	HashPosition _4;
	long c;
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

	zephir_is_iterable(b, &_5, &_4, 0, 1);
	for (
		; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		; zend_hash_move_backwards_ex(_5, &_4)
	) {
		ZEPHIR_GET_HVALUE(v, _6);

		c += zephir_get_doubleval(v);

	}

	RETURN_MM_LONG(c);


}

PHP_METHOD(Test_Flow, testFor10) {

	long v, _0;
	zephir_str *b = NULL;
	zval *a, *_1 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
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

PHP_METHOD(Test_Flow, testFor11) {

	long k, v, _0;
	zephir_str *b = NULL;
	zval *a, *_1 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
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

