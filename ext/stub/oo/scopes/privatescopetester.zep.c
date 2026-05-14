
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_PrivateScopeTester)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Oo\\Scopes, PrivateScopeTester, stub, oo_scopes_privatescopetester, stub_oo_scopes_hasprivatemethod_ce, stub_oo_scopes_privatescopetester_method_entry, 0);

	zend_class_implements(stub_oo_scopes_privatescopetester_ce, 1, stub_oo_scopes_scopetesterinterface_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, run)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "callprivatemethod", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2057
 */
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, setPropertyObj)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *property = NULL;
	zval *obj, obj_sub, property_zv, *value, value_sub, _0;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(obj)
		Z_PARAM_STR(property)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	obj = ZEND_CALL_ARG(execute_data, 1);
	value = ZEND_CALL_ARG(execute_data, 3);
	ZEPHIR_SEPARATE_PARAM(obj);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	zephir_update_property_zval_zval(obj, &property_zv, value);
	zephir_memory_observe(&_0);
	zephir_read_property_zval(&_0, obj, &property_zv, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2057
 */
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, setPropertyNew)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval className_zv, property_zv, *value, value_sub, obj;
	zend_string *className = NULL, *property = NULL;

	ZVAL_UNDEF(&className_zv);
	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&obj);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(className)
		Z_PARAM_STR(property)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	value = ZEND_CALL_ARG(execute_data, 3);
	zephir_memory_observe(&className_zv);
	ZVAL_STR_COPY(&className_zv, className);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	ZEPHIR_INIT_VAR(&obj);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance(&obj, &className_zv);
	zephir_check_call_status();
	zephir_update_property_zval_zval(&obj, &property_zv, value);
	RETURN_CCTOR(&obj);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2057
 */
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, getObjVars)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &obj);
	ZEPHIR_RETURN_CALL_FUNCTION("get_object_vars", NULL, 78, obj);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2057
 */
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, getNewVars)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval className_zv, _0;
	zend_string *className = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&className_zv);
	ZVAL_STR_COPY(&className_zv, className);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance(&_0, &className_zv);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "getobjvars", NULL, 0, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

