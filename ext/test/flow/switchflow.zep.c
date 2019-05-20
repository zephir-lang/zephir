
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/math.h"


ZEPHIR_INIT_CLASS(Test_Flow_SwitchFlow) {

	ZEPHIR_REGISTER_CLASS(Test\\Flow, SwitchFlow, test, flow_switchflow, test_flow_switchflow_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch1) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	RETURN_MM_LONG(a);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch2) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	do {
		if (a == 10) {
			RETURN_MM_BOOL(1);
		}
	} while(0);

	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch3) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	do {
		if (a == 5) {
			RETURN_MM_BOOL(1);
		}
	} while(0);

	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch4) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	do {
		if (a == 5) {
			RETURN_MM_LONG(1);
		}
		RETURN_MM_LONG(2);
	} while(0);

	RETURN_MM_LONG(3);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch5) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	do {
		if (a == 10) {
			RETURN_MM_LONG(1);
		}
		RETURN_MM_LONG(2);
	} while(0);

	RETURN_MM_LONG(3);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch6) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 10;
	do {
		if (a == 10 || a == 4) {
			RETURN_MM_LONG(1);
		}
	} while(0);

	RETURN_MM_LONG(2);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch7) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 4;
	do {
		if (a == 10 || a == 4) {
			RETURN_MM_LONG(1);
		}
	} while(0);

	RETURN_MM_LONG(2);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch8) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	do {
		if (a == 10 || a == 4) {
			RETURN_MM_LONG(1);
		}
	} while(0);

	RETURN_MM_LONG(2);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch9) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_MM_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_MM_LONG(1);
		}
	} while(0);

	RETURN_MM_LONG(2);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch10) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_MM_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_MM_LONG(1);
		}
	} while(0);

	RETURN_MM_LONG(2);

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch11) {

	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	do {
		if (a == 2) {
			RETURN_MM_LONG(2);
		}
		RETURN_MM_LONG(1);
	} while(0);

	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch12) {

	zend_bool _0, _1, _2;
	zval *var1, var1_sub, *var2, var2_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&var1_sub);
	ZVAL_UNDEF(&var2_sub);

	zephir_fetch_params(1, 2, 0, &var1, &var2);



	do {
		_0 = 1;
		_1 = ZEPHIR_GT(var1, var2);
		if (_1) {
			_1 = ZEPHIR_GT_LONG(var2, 5);
		}
		if (_0 == _1) {
			RETURN_MM_LONG(1);
		}
		_2 = ZEPHIR_LT(var1, var2);
		if (_2) {
			_2 = ZEPHIR_LT_LONG(var1, 5);
		}
		if (_0 == _2) {
			RETURN_MM_LONG(2);
		}
		RETURN_MM_LONG(0);
	} while(0);

	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Flow_SwitchFlow, testSwitch13) {

	zval *a_param = NULL, _0, _1;
	long a, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	do {
		_2 = zephir_mt_rand(zephir_get_intval(&_0), zephir_get_intval(&_1) TSRMLS_CC);
		if (_2 == 100) {
			RETURN_MM_LONG(1);
		}
		RETURN_MM_LONG(0);
	} while(0);

	ZEPHIR_MM_RESTORE();

}

