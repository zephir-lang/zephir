
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


ZEPHIR_INIT_CLASS(Stub_Returns)
{
	ZEPHIR_REGISTER_CLASS(Stub, Returns, stub, returns, stub_returns_method_entry, 0);

	zend_declare_property_null(stub_returns_ce, SL("stringDefaultNull"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_returns_ce, SL("intDefaultNull"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_returns_ce, SL("arrayDefaultNull"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_returns_ce, SL("doubleDefaultNull"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_returns_ce, SL("validString"), "hello", ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_returns_ce, SL("nullableProperty"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Returns, testReturnCast1)
{

	RETURN_LONG((int) 5.0);
}

PHP_METHOD(Stub_Returns, testReturnCast2)
{

	RETURN_LONG(1);
}

PHP_METHOD(Stub_Returns, testReturnCast3)
{

	RETURN_LONG(0);
}

PHP_METHOD(Stub_Returns, testReturnCast4)
{

	if (1) {
		RETURN_LONG(1);
	} else {
		RETURN_LONG(0);
	}
}

PHP_METHOD(Stub_Returns, returnWithParameter)
{
	zval parameter_zv;
	zend_string *parameter = NULL;

	ZVAL_UNDEF(&parameter_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(parameter)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&parameter_zv, parameter);
	RETURN_STRING("Return back");
}

PHP_METHOD(Stub_Returns, returnWithoutParameter)
{

	RETURN_STRING("Return back");
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 */
PHP_METHOD(Stub_Returns, returnNullOnString)
{

	RETURN_MEMBER_TYPED(getThis(), "stringDefaultNull", IS_STRING);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 */
PHP_METHOD(Stub_Returns, returnNullOnInt)
{

	RETURN_MEMBER_TYPED(getThis(), "intDefaultNull", IS_LONG);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 */
PHP_METHOD(Stub_Returns, returnNullOnArray)
{

	RETURN_MEMBER_TYPED(getThis(), "arrayDefaultNull", IS_ARRAY);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 */
PHP_METHOD(Stub_Returns, returnNullOnDouble)
{

	RETURN_MEMBER_TYPED(getThis(), "doubleDefaultNull", IS_DOUBLE);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 * Happy path: the property already holds a string, so no TypeError.
 */
PHP_METHOD(Stub_Returns, returnValidString)
{

	RETURN_MEMBER_TYPED(getThis(), "validString", IS_STRING);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1991
 * Nullable return type — returning null must NOT throw.
 */
PHP_METHOD(Stub_Returns, returnNullOnNullableString)
{

	RETURN_MEMBER(getThis(), "nullableProperty");
}

