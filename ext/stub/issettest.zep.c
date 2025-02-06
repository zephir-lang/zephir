
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

	zend_declare_property_null(stub_issettest_ce, SL("s"), ZEND_ACC_PUBLIC);
	stub_issettest_ce->create_object = zephir_init_properties_Stub_IssetTest;

	return SUCCESS;
}

PHP_METHOD(Stub_IssetTest, testIssetArray1)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	RETURN_BOOL(zephir_array_isset(a, b));
}

PHP_METHOD(Stub_IssetTest, testIssetArray2)
{
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	RETURN_BOOL(zephir_array_isset_long(a, b));
}

PHP_METHOD(Stub_IssetTest, testIssetArray3)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b_param);
	zephir_get_strval(&b, b_param);
	RETURN_MM_BOOL(zephir_array_isset(a, &b));
}

PHP_METHOD(Stub_IssetTest, testIssetArray4)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(zephir_array_isset_long(a, 0));
}

PHP_METHOD(Stub_IssetTest, testIssetArray5)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(zephir_array_isset_string(a, SL("a")));
}

PHP_METHOD(Stub_IssetTest, testIssetProperty1)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(zephir_isset_property(a, SL("b")));
}

PHP_METHOD(Stub_IssetTest, testIssetProperty2)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	RETURN_BOOL(zephir_isset_property_zval(a, b));
}

PHP_METHOD(Stub_IssetTest, testIssetProperty3)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL((0 == 0));
}

PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty1)
{
	zval g;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&g);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&g);
	zephir_read_property(&g, this_ptr, ZEND_STRL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(&g, SL("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *inp, inp_sub, g;

	ZVAL_UNDEF(&inp_sub);
	ZVAL_UNDEF(&g);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(inp)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &inp);
	zephir_memory_observe(&g);
	zephir_read_property(&g, inp, ZEND_STRL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(&g, SL("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

zend_object *zephir_init_properties_Stub_IssetTest(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("s"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 1, 0);
			add_assoc_stringl_ex(&_1$$3, SL("a"), SL("true"));
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("s"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

