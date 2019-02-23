
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Optimizers_ACos) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, ACos, test, optimizers_acos, test_optimizers_acos_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Optimizers_ACos, testInt) {

	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 12, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_ACos, testVar) {

	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 12, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_ACos, testIntValue1) {

	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZVAL_LONG(&_0, 0);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 12, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_ACos, testIntParameter) {

	zval *a_param = NULL, _0;
	zend_long a, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 12, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_ACos, testVarParameter) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);



	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 12, a);
	zephir_check_call_status();
	RETURN_MM();

}

