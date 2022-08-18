
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


ZEPHIR_INIT_CLASS(Stub_Types_Obj)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, Obj, stub, types_obj, stub_types_obj_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Types_Obj, nullableObjectReturnNull)
{
	zval *this_ptr = getThis();



	RETURN_NULL();
}

PHP_METHOD(Stub_Types_Obj, nullableObjectReturnObj)
{
	zval *this_ptr = getThis();



	object_init(return_value);
	return;
}

