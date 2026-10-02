
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
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * Reading `a[k]` raises PHP 8's diagnostics, not PHP 5's: a missing key is
 * the warning `Undefined array key`, a null or scalar container is the
 * warning `Trying to access array offset on ...`, an offset is converted with
 * PHP's own deprecations and TypeError, and an object without ArrayAccess is
 * read through its own handler. Every read form the compiler emits is here;
 * the test compares each against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_ArrayRead)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayRead, stub, arrayread, stub_arrayread_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ArrayRead, read)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &k);
	zephir_memory_observe(&_0);
	zephir_array_fetch(&_0, a, k, PH_NOISY, "stub/arrayread.zep", 15);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readFromArray)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *k, k_sub, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &k);
	zephir_get_arrval(&a, a_param);
	zephir_array_fetch(&_0, &a, k, PH_NOISY | PH_READONLY, "stub/arrayread.zep", 20);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readStringLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, a, SL("zz"), PH_NOISY, "stub/arrayread.zep", 25);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readIntLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, a, 7, PH_NOISY, "stub/arrayread.zep", 30);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long k;
	zval *a, a_sub, *k_param = NULL, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &k_param);
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, a, k, PH_NOISY, "stub/arrayread.zep", 35);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *k = NULL;
	zval *a, a_sub, k_zv, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&k_zv);
	ZVAL_STR_COPY(&k_zv, k);
	zephir_memory_observe(&_0);
	zephir_array_fetch(&_0, a, &k_zv, PH_NOISY, "stub/arrayread.zep", 40);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ArrayRead, readNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *i, i_sub, *j, j_sub, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &i, &j);
	zephir_memory_observe(&_0);
	zephir_array_fetch(&_0, a, i, PH_NOISY, "stub/arrayread.zep", 45);
	zephir_memory_observe(&_1);
	zephir_array_fetch(&_1, &_0, j, PH_NOISY, "stub/arrayread.zep", 45);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_ArrayRead, readNestedLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, a, SL("zz"), PH_NOISY, "stub/arrayread.zep", 50);
	zephir_memory_observe(&_1);
	zephir_array_fetch_string(&_1, &_0, SL("yy"), PH_NOISY, "stub/arrayread.zep", 50);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_ArrayRead, readIntoLocal)
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
	zephir_array_fetch(&value, a, k, PH_NOISY, "stub/arrayread.zep", 57);
	RETURN_CCTOR(&value);
}

