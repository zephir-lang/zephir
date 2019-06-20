
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


ZEPHIR_INIT_CLASS(Test_Optimizers_IsScalar) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, IsScalar, test, optimizers_isscalar, test_optimizers_isscalar_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Optimizers_IsScalar, testIntVar) {

	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = 1;
	ZVAL_LONG(&_0, a);
	RETURN_BOOL(zephir_is_scalar(&_0));

}

PHP_METHOD(Test_Optimizers_IsScalar, testDoubleVar) {

	zval _0;
	double a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = (double) (1);
	ZVAL_DOUBLE(&_0, a);
	RETURN_BOOL(zephir_is_scalar(&_0));

}

PHP_METHOD(Test_Optimizers_IsScalar, testBoolVar) {

	zval _0;
	zend_bool a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = 1;
	ZVAL_BOOL(&_0, (a ? 1 : 0));
	RETURN_BOOL(zephir_is_scalar(&_0));

}

PHP_METHOD(Test_Optimizers_IsScalar, testStringVar) {

	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "test string");
	RETURN_MM_BOOL(zephir_is_scalar(&a));

}

PHP_METHOD(Test_Optimizers_IsScalar, testEmptyArrayVar) {

	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_BOOL(zephir_is_scalar(&a));

}

PHP_METHOD(Test_Optimizers_IsScalar, testVar) {

	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = 1;
	ZVAL_LONG(&_0, a);
	RETURN_BOOL(zephir_is_scalar(&_0));

}

PHP_METHOD(Test_Optimizers_IsScalar, testVarParameter) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(zephir_is_scalar(a));

}

