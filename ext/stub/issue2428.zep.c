
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
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "ext/spl/spl_array.h"
#include "kernel/fcall.h"


/**
 * Union return types — every member must survive into the compiled
 * extension's arginfo so PHP enforces and reflects the full union, exactly
 * like a hand-written PHP union return type.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2428
 */
ZEPHIR_INIT_CLASS(Stub_Issue2428)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2428, stub, issue2428, stub_issue2428_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2428, scalarUnion)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, _0;
	zend_bool flag;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&_0);
	if (flag) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_LONG(&_0, 1);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "x");
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2428, scalarUnionNullable)
{
	zval *n_param = NULL;
	zend_long n;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	RETURN_LONG(n);
}

PHP_METHOD(Stub_Issue2428, classUnion)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *flag_param = NULL, _0;
	zend_bool flag;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&_0);
	if (flag) {
		ZEPHIR_INIT_NVAR(&_0);
		object_init(&_0);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		object_init_ex(&_0, spl_ce_ArrayObject);
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 58);
		zephir_check_call_status();
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2428, classUnionNullable)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, _0;
	zend_long n;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&_0);
	if (n > 0) {
		ZEPHIR_INIT_NVAR(&_0);
		object_init(&_0);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_NULL(&_0);
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2428, classOrScalar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, _0;
	zend_bool flag;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&_0);
	if (flag) {
		ZEPHIR_INIT_NVAR(&_0);
		object_init(&_0);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_LONG(&_0, 5);
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2428, arrayOrObject)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, _0;
	zend_bool flag;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&_0);
	if (flag) {
		ZEPHIR_INIT_NVAR(&_0);
		array_init(&_0);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		object_init(&_0);
	}
	RETURN_CCTOR(&_0);
}

/**
 * Returns whatever it is given. Used to drive negative tests: the body
 * checker cannot see the dynamic value's type, so an out-of-union value
 * can reach the boundary at runtime.
 */
PHP_METHOD(Stub_Issue2428, dynamicReturn)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

