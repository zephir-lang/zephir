
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
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Oo_DeprecatedMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, DeprecatedMethods, stub, oo_deprecatedmethods, stub_oo_deprecatedmethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo_DeprecatedMethods, publicDeprecated)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_Oo_DeprecatedMethods, normalMethod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "privatedeprecated", NULL, 67);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Oo_DeprecatedMethods, privateDeprecated)
{

	RETURN_BOOL(1);
}

