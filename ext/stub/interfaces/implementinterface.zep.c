
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Interfaces_ImplementInterface)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Interfaces, ImplementInterface, stub, interfaces_implementinterface, stub_interfaces_implementinterface_method_entry, 0);

	zend_declare_property_null(stub_interfaces_implementinterface_ce, SL("obj"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_interfaces_implementinterface_ce, 1, stub_interfaces_interfaceintsignature_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_Interfaces_ImplementInterface, get)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *obj, obj_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(obj, stub_interfaces_interfaceint_ce)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &obj);


	ZEPHIR_RETURN_CALL_METHOD(obj, "get", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Interfaces_ImplementInterface, getVoid)
{
	zval *obj, obj_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(obj, stub_interfaces_interfaceint_ce)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &obj);


	zephir_update_property_zval(this_ptr, ZEND_STRL("obj"), obj);
}

PHP_METHOD(Stub_Interfaces_ImplementInterface, getObj)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "obj");
}

