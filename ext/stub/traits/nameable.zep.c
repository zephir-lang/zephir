
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


/**
 * Trait exposed to PHP userland (zephir#504).
 */
ZEPHIR_INIT_CLASS(Stub_Traits_Nameable)
{
	ZEPHIR_REGISTER_TRAIT(Stub\\Traits, Nameable, stub, traits_nameable, stub_traits_nameable_method_entry);

	zend_declare_property_string(stub_traits_nameable_ce, SL("name"), "unnamed", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Traits_Nameable, setName)
{
	zval name_zv;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval(this_ptr, ZEND_STRL("name"), &name_zv);
}

PHP_METHOD(Stub_Traits_Nameable, getName)
{

	RETURN_MEMBER_TYPED(getThis(), "name", IS_STRING);
}

