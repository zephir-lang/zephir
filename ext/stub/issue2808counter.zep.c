
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


/**
 * The same operations on `this`, whose accesses use the inline cache, with
 * declared typed properties.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
ZEPHIR_INIT_CLASS(Stub_Issue2808Counter)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2808Counter, stub, issue2808counter, stub_issue2808counter_method_entry, 0);

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 0);
		zephir_declare_typed_property(stub_issue2808counter_ce, SL("counter"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "", sizeof("") - 1);
		zephir_declare_typed_property(stub_issue2808counter_ce, SL("text"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_STRING, NULL, 0);
	}

	zend_declare_property_long(stub_issue2808counter_ce, SL("plain"), 0, ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2808Counter, increment)
{
	zval *this_ptr = getThis();

	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	zephir_property_incdec(this_ptr, _zephir_prop_0, 89, 1);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, decrement)
{
	zval *this_ptr = getThis();

	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	zephir_property_incdec(this_ptr, _zephir_prop_0, 89, 0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, addCounter)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_property_assign_op(this_ptr, _zephir_prop_0, 89, v, add_function);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, appendText)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("text", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_property_assign_op(this_ptr, _zephir_prop_0, 90, v, concat_function);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, addPlain)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("plain", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_property_assign_op(this_ptr, _zephir_prop_0, 91, v, add_function);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, appendPlain)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("plain", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_property_assign_op(this_ptr, _zephir_prop_0, 91, v, concat_function);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2808Counter, incrementPlain)
{
	zval *this_ptr = getThis();

	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("plain", 5, 1);
	}
	zephir_property_incdec(this_ptr, _zephir_prop_0, 91, 1);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

