
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_MethodArgs) {

	ZEPHIR_REGISTER_CLASS(Test, MethodArgs, test, methodargs, test_methodargs_method_entry, 0);

	zend_declare_property_null(test_methodargs_ce, SL("a"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zephir_declare_class_constant_string(test_methodargs_ce, SL("GET"), "get");

	zephir_declare_class_constant_double(test_methodargs_ce, SL("MY_DOUBLE"), 1.32);

	zephir_declare_class_constant_bool(test_methodargs_ce, SL("MY_BOOL"), 1);

	zephir_declare_class_constant_long(test_methodargs_ce, SL("MY_INT"), 12345);

	return SUCCESS;

}

PHP_METHOD(Test_MethodArgs, setCallable) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	zephir_update_property_zval(this_ptr, SL("a"), a);

}

PHP_METHOD(Test_MethodArgs, setObject) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	zephir_update_property_zval(this_ptr, SL("a"), a);

}

PHP_METHOD(Test_MethodArgs, setCallableStrict) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);

	if (UNEXPECTED(zephir_is_callable(a TSRMLS_CC) != 1)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'a' must be of the type callable") TSRMLS_CC);
		RETURN_NULL();
	}


	zephir_update_property_zval(this_ptr, SL("a"), a);

}

PHP_METHOD(Test_MethodArgs, setObjectStrict) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);

	if (UNEXPECTED(Z_TYPE_P(a) != IS_OBJECT)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'a' must be of the type object") TSRMLS_CC);
		RETURN_NULL();
	}


	zephir_update_property_zval(this_ptr, SL("a"), a);

}

PHP_METHOD(Test_MethodArgs, setResourceStrict) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);

	if (UNEXPECTED(Z_TYPE_P(a) != IS_RESOURCE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'a' must be of the type resource") TSRMLS_CC);
		RETURN_NULL();
	}


	zephir_update_property_zval(this_ptr, SL("a"), a);

}

/**
 * @link https://github.com/phalcon/zephir/issues/336
 */
PHP_METHOD(Test_MethodArgs, methodOptionalValueWithDefaultStaticConstantAccess) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *method = NULL, method_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &method);

	if (!method) {
		method = &method_sub;
		ZEPHIR_INIT_VAR(method);
		ZVAL_STRING(method, "get");
	}


	RETVAL_ZVAL(method, 1, 0);
	RETURN_MM();

}

/**
 * For string parameter
 * @link https://github.com/phalcon/zephir/issues/696
 */
PHP_METHOD(Test_MethodArgs, methodOptionalStringValueWithDefaultStaticConstantAccess) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *parameter_param = NULL;
	zval parameter;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&parameter);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameter_param);

	if (!parameter_param) {
		ZEPHIR_INIT_VAR(&parameter);
		ZVAL_STRING(&parameter, "get");
	} else {
		zephir_get_strval(&parameter, parameter_param);
	}


	RETURN_CTOR(&parameter);

}

/**
 * For double parameter
 * @link https://github.com/phalcon/zephir/issues/696
 */
PHP_METHOD(Test_MethodArgs, methodOptionalDoubleValueWithDefaultStaticConstantAccess) {

	zval *parameter_param = NULL;
	double parameter;
	zval *this_ptr = getThis();


	zephir_fetch_params_without_memory_grow(0, 1, &parameter_param);

	if (!parameter_param) {
		parameter = 1.32;
	} else {
		parameter = zephir_get_doubleval(parameter_param);
	}


	RETURN_DOUBLE(parameter);

}

/**
 * For bool parameter
 * @link https://github.com/phalcon/zephir/issues/696
 */
PHP_METHOD(Test_MethodArgs, methodOptionalBoolValueWithDefaultStaticConstantAccess) {

	zval *parameter_param = NULL;
	zend_bool parameter;
	zval *this_ptr = getThis();


	zephir_fetch_params_without_memory_grow(0, 1, &parameter_param);

	if (!parameter_param) {
		parameter = 1;
	} else {
		parameter = zephir_get_boolval(parameter_param);
	}


	RETURN_BOOL(parameter);

}

/**
 * For int parameter
 * @link https://github.com/phalcon/zephir/issues/728
 */
PHP_METHOD(Test_MethodArgs, methodOptionalIntValueWithDefaultStaticConstantAccess) {

	zval *parameter_param = NULL;
	zend_long parameter;
	zval *this_ptr = getThis();


	zephir_fetch_params_without_memory_grow(0, 1, &parameter_param);

	if (!parameter_param) {
		parameter = 12345;
	} else {
		parameter = zephir_get_intval(parameter_param);
	}


	RETURN_LONG(parameter);

}

