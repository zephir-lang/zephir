
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_AbstractClass)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo\\Scopes, AbstractClass, stub, oo_scopes_abstractclass, stub_oo_scopes_abstractclass_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_string(stub_oo_scopes_abstractclass_ce, SL("privateProperty2"), "private2", ZEND_ACC_PRIVATE);
	zend_declare_property_string(stub_oo_scopes_abstractclass_ce, SL("protectedProperty2"), "protected2", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, setProperty)
{
	zval name_zv, *name_param = NULL, *value, value_sub;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &name_param, &value);
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_zval(this_ptr, &name_zv, value);
	RETURN_THISW();
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getPrivateProperty2)
{

	RETURN_MEMBER(getThis(), "privateProperty2");
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getProtectedProperty2)
{

	RETURN_MEMBER(getThis(), "protectedProperty2");
}

