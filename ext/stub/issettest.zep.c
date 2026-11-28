
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
 * Unset statement tests
 */
ZEPHIR_INIT_CLASS(Stub_IssetTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, IssetTest, stub, issettest, stub_issettest_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_assoc_stringl_ex(&_zc0, SL("a"), SL("true"));
		zephir_declare_property_array(stub_issettest_ce, SL("s"), &_zc0, ZEND_ACC_PUBLIC);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_IssetTest, testIssetArray1)
{
	zend_bool _0;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	_0 = zephir_array_isset_value(a, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetArray2)
{
	zend_bool _0;
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	_0 = zephir_array_isset_value_long(a, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetArray3)
{
	zend_bool _0;
	zend_string *b = NULL;
	zval *a, a_sub, b_zv;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	a = ZEND_CALL_ARG(execute_data, 1);
	ZVAL_STR(&b_zv, b);
	_0 = zephir_array_isset_value(a, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetArray4)
{
	zend_bool _0;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	_0 = zephir_array_isset_value_long(a, 0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetArray5)
{
	zend_bool _0;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	_0 = zephir_array_isset_value_string(a, SL("a"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetProperty1)
{
	zend_bool _0;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("b", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	_0 = zephir_isset_property_value_fast(a, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetProperty2)
{
	zend_bool _0;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	_0 = zephir_isset_property_value_zval(a, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetProperty3)
{
	zend_bool _0;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("b", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	_0 = zephir_isset_property_value_fast(a, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty1)
{
	zend_bool _0;
	zval g;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&g);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("s", 1, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&g);
	zephir_read_property_cached(&g, this_ptr, _zephir_prop_0, 51, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_0 = zephir_array_isset_value_string(&g, SL("a"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty2)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *inp, inp_sub, g;

	ZVAL_UNDEF(&inp_sub);
	ZVAL_UNDEF(&g);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("s", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(inp)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &inp);
	zephir_memory_observe(&g);
	zephir_read_property_cached(&g, inp, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_0 = zephir_array_isset_value_string(&g, SL("a"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

