
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
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_Scall) {

	ZEPHIR_REGISTER_CLASS_EX(Test, Scall, test, scall, test_scallparent_ce, test_scall_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Scall, testMethod1) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello public");

}

PHP_METHOD(Test_Scall, testMethod2) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello protected");

}

PHP_METHOD(Test_Scall, testMethod3) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello private");

}

PHP_METHOD(Test_Scall, testMethod4) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	return;

}

PHP_METHOD(Test_Scall, testMethod5) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	return;

}

PHP_METHOD(Test_Scall, testMethod6) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	return;

}

PHP_METHOD(Test_Scall, testMethod7) {

	zval *this_ptr = getThis();


	object_init(return_value);
	return;

}

PHP_METHOD(Test_Scall, testCall1) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall2) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall3) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod3", &_0, 65);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall4) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall5) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall6) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod6", &_0, 66, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall7) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall8) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall9) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod3", &_0, 65);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall10) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall11) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall12) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_SELF("testmethod6", &_0, 66, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall13) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_PARENT(test_scall_ce, getThis(), "testmethod1", &_0, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall14) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_PARENT(test_scall_ce, getThis(), "testmethod2", &_0, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall15) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_SELF("testmethod7", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testMethod16) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;
	zval *this_ptr = getThis();


	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_LONG((a + b));

}

PHP_METHOD(Test_Scall, testCall17) {

	zend_bool _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_4 = NULL;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &k_param, &p);

	k = zephir_get_intval(k_param);


	j = 0;
	_2 = k;
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
			i = _1;
			ZEPHIR_CALL_CE_STATIC(&_3$$3, test_scallexternal_ce, "testmethod3", &_4, 0, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);

}

PHP_METHOD(Test_Scall, testCall18) {

	zend_bool _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_4 = NULL;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &k_param, &p);

	k = zephir_get_intval(k_param);


	j = 0;
	_2 = k;
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
			i = _1;
			ZEPHIR_CALL_SELF(&_3$$3, "testmethod16", &_4, 0, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);

}

PHP_METHOD(Test_Scall, testMethodStatic) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello Scall");

}

PHP_METHOD(Test_Scall, interpolatedStaticReturn) {

	zend_class_entry *_0;
	zval className, methodName;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&methodName);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&className);
	ZVAL_STRING(&className, "Test\\Scall");
	ZEPHIR_INIT_VAR(&methodName);
	ZVAL_STRING(&methodName, "testMethodStatic");
	_0 = zephir_fetch_class(&className TSRMLS_CC);
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, methodName, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, interpolatedStaticEcho) {

	zend_class_entry *_1;
	zval _0;
	zval className, methodName;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&methodName);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&className);
	ZVAL_STRING(&className, "Test\\Scall");
	ZEPHIR_INIT_VAR(&methodName);
	ZVAL_STRING(&methodName, "testMethodStatic");
	_1 = zephir_fetch_class(&className TSRMLS_CC);
	ZEPHIR_CALL_CE_STATIC_ZVAL(&_0, _1, methodName, NULL, 0);
	zephir_check_call_status();
	zend_print_zval(&_0, 0);
	ZEPHIR_MM_RESTORE();

}

