
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


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2746
 *
 * Converting a zval to an integer or a float must give PHP's `(int)` and
 * `(float)` result: an object runs its own cast handler or warns, a float
 * string beyond the int range saturates, and a resource reads as its
 * handle. Every conversion path the compiler emits is here, and the test compares
 * each against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2746)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2746, stub, issue2746, stub_issue2746_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2746, toLong)
{
	zend_long k;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	k = zephir_get_intval(a);
	RETURN_LONG(k);
}

PHP_METHOD(Stub_Issue2746, castInt)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_LONG(zephir_get_intval(a));
}

PHP_METHOD(Stub_Issue2746, intvalOf)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_LONG(zephir_get_intval(a));
}

PHP_METHOD(Stub_Issue2746, toDouble)
{
	double d;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	d = zephir_get_numberval(a);
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Issue2746, castDouble)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_DOUBLE(zephir_get_doubleval(a));
}

PHP_METHOD(Stub_Issue2746, doublevalOf)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_DOUBLE(zephir_get_doubleval(a));
}

PHP_METHOD(Stub_Issue2746, doubleParam)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Issue2746, optionalDoubleParam)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &a_param);
	if (!a_param) {
		a = 1.5;
	} else {
		}
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Issue2746, nullableDoubleParam)
{
	zval *a_param = NULL;
	double a;

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_DOUBLE_OR_NULL(a, is_null_true)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &a_param);
	if (!a_param) {
		a = 0;
	} else {
		}
	RETURN_DOUBLE(a);
}

