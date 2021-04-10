
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


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_HasPrivateMethod)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo\\Scopes, HasPrivateMethod, stub, oo_scopes_hasprivatemethod, stub_oo_scopes_hasprivatemethod_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo_Scopes_HasPrivateMethod, callPrivateMethod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "isprivate", NULL, 1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Oo_Scopes_HasPrivateMethod, isPrivate)
{
	zval *this_ptr = getThis();



	RETURN_STRING("isPrivate");
}

