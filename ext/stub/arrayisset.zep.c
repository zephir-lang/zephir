
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * `isset a[k]`, `empty(a[k])` and `fetch v, a[k]` convert the offset as PHP's
 * isset does: the same deprecations and warnings as a read, a TypeError naming
 * "isset or empty" for an illegal offset, and an object without ArrayAccess
 * asked through its own handler. The test compares each against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_ArrayIsset)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayIsset, stub, arrayisset, stub_arrayisset_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ArrayIsset, issetVar)
{
	zval *a, a_sub, *k, k_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &k);
	RETURN_BOOL(zephir_array_isset_value(a, k));
}

PHP_METHOD(Stub_ArrayIsset, emptyVar)
{
	zval *a, a_sub, *k, k_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &k);
	RETURN_BOOL(zephir_isempty_dim(a, k));
}

PHP_METHOD(Stub_ArrayIsset, fetchVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, value;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &k);
	zephir_memory_observe(&value);
	RETURN_MM_BOOL(zephir_array_isset_fetch(&value, a, k, 0));
}

PHP_METHOD(Stub_ArrayIsset, issetStringLiteral)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(zephir_array_isset_value_string(a, SL("zz")));
}

PHP_METHOD(Stub_ArrayIsset, issetIntLiteral)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(zephir_array_isset_value_long(a, 1));
}

PHP_METHOD(Stub_ArrayIsset, issetNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *i, i_sub, *j, j_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &i, &j);
	zephir_memory_observe(&_0);
	zephir_array_fetch(&_0, a, i, 0, "stub/arrayisset.zep", 40);
	RETURN_MM_BOOL(zephir_array_isset_value(&_0, j));
}

