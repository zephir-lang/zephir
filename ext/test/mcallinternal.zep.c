
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * Method calls
 */
ZEPHIR_INIT_CLASS(Test_McallInternal) {

	ZEPHIR_REGISTER_CLASS(Test, McallInternal, test, mcallinternal, test_mcallinternal_method_entry, 0);

	return SUCCESS;

}

void zep_Test_McallInternal_a(int ht, zval *return_value, zval *this_ptr, int return_value_used) {

	zval _0;
		ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));

}

void zep_Test_McallInternal_b(int ht, zval *return_value, zval *this_ptr, int return_value_used, zval *a_ext , zval *b_ext ) {

	zval *a, a_sub, *b, b_sub;
		ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	a = a_ext;

	b = b_ext;





}

void zep_Test_McallInternal_c(int ht, zval *return_value, zval *this_ptr, int return_value_used, zval *a_param_ext, zval *b_param_ext) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;

	a_param = a_param_ext;

	b_param = b_param_ext;


	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_LONG((a + b));

}

PHP_METHOD(Test_McallInternal, e) {

	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));

}

PHP_METHOD(Test_McallInternal, d) {

	zval _3$$3;
	zend_bool _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a, i = 0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_3$$3);

	ZEPHIR_MM_GROW();

	a = 0;
	_2 = 1000000;
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
			ZEPHIR_CALL_INTERNAL_METHOD_P0(&_3$$3, this_ptr, zep_Test_McallInternal_a);
			zephir_check_call_status();
			a += zephir_get_intval(&_3$$3);
		}
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_McallInternal, f) {

	zval _3$$3;
	zend_bool _0;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, a, i = 0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_3$$3);

	ZEPHIR_MM_GROW();

	a = 0;
	_2 = 1000000;
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
			ZEPHIR_CALL_METHOD(&_3$$3, this_ptr, "e", &_4, 0);
			zephir_check_call_status();
			a += zephir_get_intval(&_3$$3);
		}
	}
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_McallInternal, g) {

	zval _3$$3, _4$$3, _5$$3;
	zend_bool _0;
	long i = 0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);

	ZEPHIR_MM_GROW();

	a = 0;
	_2 = 1000;
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
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, i);
			ZEPHIR_CALL_INTERNAL_METHOD_P2(&_3$$3, this_ptr, zep_Test_McallInternal_c, &_4$$3, &_5$$3);
			zephir_check_call_status();
			a += zephir_get_intval(&_3$$3);
		}
	}
	RETURN_MM_LONG(a);

}

void zep_Test_McallInternal_other(int ht, zval *return_value, zval *this_ptr, int return_value_used, zval *a_param_ext, zval *b_param_ext) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;

	a_param = a_param_ext;

	b_param = b_param_ext;


	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_DOUBLE(zephir_safe_div_long_long(a, b TSRMLS_CC));

}

PHP_METHOD(Test_McallInternal, callFibonacci) {

	zval _3$$3, _4$$3, _5$$3;
	zend_bool _0;
	long i;
	double p;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);

	ZEPHIR_MM_GROW();

	p = (double) (0);
	i = 0;
	_2 = 10000000;
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
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, (i + 1));
			ZEPHIR_CALL_INTERNAL_METHOD_P2(&_3$$3, this_ptr, zep_Test_McallInternal_other, &_4$$3, &_5$$3);
			zephir_check_call_status();
			p += zephir_get_doubleval(&_3$$3);
		}
	}
	RETURN_MM_DOUBLE(p);

}

