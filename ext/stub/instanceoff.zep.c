
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
#include "kernel/object.h"
#include "kernel/fcall.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Stub_Instanceoff)
{
	ZEPHIR_REGISTER_CLASS(Stub, Instanceoff, stub, instanceoff, stub_instanceoff_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf1)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_BOOL(zephir_is_instance_of(&a, SL("stdClass")));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf2)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init_ex(&a, stub_instanceoff_ce);
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM_BOOL(zephir_instance_of_ev(&a, stub_instanceoff_ce));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf3)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_BOOL(zephir_instance_of_ev(&a, stub_unknownclass_ce));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf4)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	if (zephir_zval_is_traversable(a)) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf5)
{
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	if (zephir_instance_of_ev(a, stub_instanceoff_ce)) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf6)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init_ex(&a, zend_get_called_scope(execute_data));
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM_BOOL(zephir_instance_of_ev(&a, stub_instanceoff_ce));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf7)
{
	zval *test, test_sub;

	ZVAL_UNDEF(&test_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT(test)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &test);
	RETURN_BOOL(zephir_instance_of_ev(test, stub_instanceoff_ce));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf8)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval test_zv, a;
	zend_string *test = NULL;

	ZVAL_UNDEF(&test_zv);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(test)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&test_zv);
	ZVAL_STR_COPY(&test_zv, test);
	ZEPHIR_INIT_VAR(&a);
	object_init_ex(&a, zend_get_called_scope(execute_data));
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM_BOOL(zephir_is_instance_of(&a, Z_STRVAL_P(&test_zv), Z_STRLEN_P(&test_zv)));
}

PHP_METHOD(Stub_Instanceoff, testInstanceOf9)
{
	zend_string *test = NULL;
	zval *a, a_sub, test_zv;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&test_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT(a)
		Z_PARAM_STR(test)
	ZEND_PARSE_PARAMETERS_END();
	a = ZEND_CALL_ARG(execute_data, 1);
	ZVAL_STR(&test_zv, test);
	RETURN_BOOL(zephir_is_instance_of(a, Z_STRVAL_P(&test_zv), Z_STRLEN_P(&test_zv)));
}

