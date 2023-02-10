
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
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Properties_GetObjectVars)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, GetObjectVars, stub, properties_getobjectvars, stub_properties_getobjectvars_method_entry, 0);

	zend_declare_property_long(stub_properties_getobjectvars_ce, SL("a"), 1, ZEND_ACC_PUBLIC);
	zend_declare_property_long(stub_properties_getobjectvars_ce, SL("b"), 2, ZEND_ACC_PROTECTED);
	zend_declare_property_long(stub_properties_getobjectvars_ce, SL("c"), 3, ZEND_ACC_PRIVATE);
	return SUCCESS;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1245
 */
PHP_METHOD(Stub_Properties_GetObjectVars, issue1245)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_FUNCTION("get_object_vars", NULL, 72, this_ptr);
	zephir_check_call_status();
	RETURN_MM();
}

