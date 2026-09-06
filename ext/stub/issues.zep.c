
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Issues)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issues, stub, issues, stub_issues_method_entry, 0);

	zend_declare_property_null(stub_issues_ce, SL("adapter"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issues, setAdapter)
{
	zval *adapter, adapter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&adapter_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("adapter", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(adapter)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &adapter);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 63, adapter);
}

PHP_METHOD(Stub_Issues, someMethod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *methodName, methodName_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&methodName_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("adapter", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(methodName)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &methodName);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 63, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_RETURN_CALL_METHOD_ZVAL(&_0, methodName, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issues, test)
{

	php_printf("%s", "test");
}

