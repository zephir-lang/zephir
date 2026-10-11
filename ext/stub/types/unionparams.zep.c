
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
#include "kernel/object.h"


/**
 * Union-typed parameters are checked and coerced on entry exactly like a
 * PHP union parameter.
 */
ZEPHIR_INIT_CLASS(Stub_Types_UnionParams)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, UnionParams, stub, types_unionparams, stub_types_unionparams_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Types_UnionParams, number)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, scalar)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, floatOrString)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, intOrFalse)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, objectOrArray)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, classes)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, second)
{
	zval label_zv, *value, value_sub;
	zend_string *label = NULL;

	ZVAL_UNDEF(&label_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(label)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 2))) {
		RETURN_THROWS();
	}
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&label_zv, label);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, optional)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value = NULL, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &value);
	if (!value) {
		value = &value_sub;
		ZEPHIR_INIT_VAR(value);
		ZVAL_LONG(value, 5);
	}
	RETVAL_ZVAL(value, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Types_UnionParams, nullDefault)
{
	zval *value = NULL, value_sub, __$null;

	ZVAL_UNDEF(&value_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(0, 1, &value);
	if (!value) {
		value = &value_sub;
		value = &__$null;
	}
	RETVAL_ZVAL(value, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_UnionParams, staticNumber)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	if (value && UNEXPECTED(!zephir_verify_union_arg(value, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	RETVAL_ZVAL(value, 1, 0);
	return;
}

