
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
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Optimizers_Cos) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, Cos, test, optimizers_cos, test_optimizers_cos_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Optimizers_Cos, testInt) {

	zval _0;
	int a = 4, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("cos", NULL, 11, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_Cos, testVar) {

	zval _0;
	int a = 4, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("cos", NULL, 11, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_Cos, testIntValue1) {


	RETURN_DOUBLE(cos(4));

}

PHP_METHOD(Test_Optimizers_Cos, testIntValue2) {


	RETURN_DOUBLE(cos(16));

}

PHP_METHOD(Test_Optimizers_Cos, testIntParameter) {

	zval *a_param = NULL, _0;
	int a, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_intval(a_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("cos", NULL, 11, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Optimizers_Cos, testVarParameter) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);



	ZEPHIR_RETURN_CALL_FUNCTION("cos", NULL, 11, a);
	zephir_check_call_status();
	RETURN_MM();

}

