
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
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


ZEPHIR_INIT_CLASS(Stub_Compare)
{
	ZEPHIR_REGISTER_CLASS(Stub, Compare, stub, compare, stub_compare_method_entry, 0);

	zephir_declare_class_constant_double(stub_compare_ce, SL("PI"), 3.14);

	return SUCCESS;
}

PHP_METHOD(Stub_Compare, isLessInt)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_BOOL(a < b);
}

PHP_METHOD(Stub_Compare, isGreaterEqual)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_BOOL(a >= b);
}

PHP_METHOD(Stub_Compare, isLessDouble)
{
	zval *a_param = NULL, *b_param = NULL;
	double a, b;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_doubleval(a_param);
	b = zephir_get_doubleval(b_param);


	RETURN_BOOL(a < b);
}

PHP_METHOD(Stub_Compare, isLessThenPi)
{
	zval *a_param = NULL;
	double a;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	a = zephir_get_doubleval(a_param);


	RETURN_BOOL(a < 3.14);
}

PHP_METHOD(Stub_Compare, isMoreThenPi)
{
	zval *a_param = NULL;
	double a;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	a = zephir_get_doubleval(a_param);


	RETURN_BOOL(3.14 < a);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/411
 */
PHP_METHOD(Stub_Compare, testVarWithStringEquals)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL;
	zval str;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str_param);
	if (UNEXPECTED(Z_TYPE_P(str_param) != IS_STRING && Z_TYPE_P(str_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'str' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(str_param) == IS_STRING)) {
		zephir_get_strval(&str, str_param);
	} else {
		ZEPHIR_INIT_VAR(&str);
		ZVAL_EMPTY_STRING(&str);
	}


	if (ZEPHIR_IS_STRING_IDENTICAL(&str, "wrong testing")) {
		ZEPHIR_INIT_NVAR(&str);
		ZVAL_STRING(&str, "NOK");
	} else {
		if (ZEPHIR_IS_STRING_IDENTICAL(&str, "another testing")) {
			ZEPHIR_INIT_NVAR(&str);
			ZVAL_STRING(&str, "NOK");
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(&str, "testing")) {
				ZEPHIR_INIT_NVAR(&str);
				ZVAL_STRING(&str, "OK");
			} else {
				ZEPHIR_INIT_NVAR(&str);
				ZVAL_STRING(&str, "NOK");
			}
		}
	}
	RETURN_CTOR(&str);
}

PHP_METHOD(Stub_Compare, testVarEqualsNull)
{
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a);


	if (Z_TYPE_P(a) == IS_NULL) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Compare, testNullEqualsVar)
{
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a);


	if (Z_TYPE_P(a) == IS_NULL) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Compare, testNotIdenticalZeroVar)
{
	zend_long a;
	zval *this_ptr = getThis();




	a = 5;
	RETURN_BOOL(0 != a);
}

PHP_METHOD(Stub_Compare, testNotIdenticalZeroInt)
{
	zend_long a;
	zval *this_ptr = getThis();




	a = 5;
	RETURN_BOOL(0 != a);
}

PHP_METHOD(Stub_Compare, testNotIdenticalZeroLong)
{
	long a;
	zval *this_ptr = getThis();




	a = 5;
	RETURN_BOOL(0 != a);
}

