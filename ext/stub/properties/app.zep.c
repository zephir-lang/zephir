
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
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Properties_App)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, App, stub, properties_app, stub_properties_app_method_entry, 0);

	/** @var \Stub\Properties\StaticPrivateProperties  */
	zend_declare_property_null(stub_properties_app_ce, SL("instance"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

/**
 */
PHP_METHOD(Stub_Properties_App, getInstance)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "instance");
}

PHP_METHOD(Stub_Properties_App, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_CE_STATIC(&_0, stub_properties_staticprivateproperties_ce, "getinstance", NULL, 0);
	zephir_check_call_status();
	zephir_update_property_zval(this_ptr, ZEND_STRL("instance"), &_0);
	ZEPHIR_MM_RESTORE();
}

