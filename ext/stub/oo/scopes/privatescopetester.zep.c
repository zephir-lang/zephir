
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
#include "kernel/operators.h"


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



	ZEPHIR_MM_GROW();

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
	zval property;
	zval *obj, obj_sub, *property_param = NULL, *value, value_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&property);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(obj)
		Z_PARAM_STR(property)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &obj, &property_param, &value);
	ZEPHIR_SEPARATE_PARAM(obj);
	zephir_get_strval(&property, property_param);


	zephir_update_property_zval_zval(obj, &property, value);
	ZEPHIR_OBS_VAR(&_0);
	zephir_read_property_zval(&_0, obj, &property, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2057
 */
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, setPropertyNew)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className_param = NULL, *property_param = NULL, *value, value_sub, obj;
	zval className, property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&obj);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(className)
		Z_PARAM_STR(property)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &className_param, &property_param, &value);
	zephir_get_strval(&className, className_param);
	zephir_get_strval(&property, property_param);


	ZEPHIR_INIT_VAR(&obj);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance(&obj, &className);
	zephir_check_call_status();
	zephir_update_property_zval_zval(&obj, &property, value);
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
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &obj);


	ZEPHIR_RETURN_CALL_FUNCTION("get_object_vars", NULL, 74, obj);
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
	zval *className_param = NULL, _0;
	zval className;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(className)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &className_param);
	zephir_get_strval(&className, className_param);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance(&_0, &className);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "getobjvars", NULL, 0, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

