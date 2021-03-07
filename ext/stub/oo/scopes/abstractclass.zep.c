
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


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_AbstractClass) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo\\Scopes, AbstractClass, stub, oo_scopes_abstractclass, stub_oo_scopes_abstractclass_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_string(stub_oo_scopes_abstractclass_ce, SL("privateProperty2"), "private2", ZEND_ACC_PRIVATE);

	zend_declare_property_string(stub_oo_scopes_abstractclass_ce, SL("protectedProperty2"), "protected2", ZEND_ACC_PROTECTED);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, setProperty) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, *value, value_sub;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &name_param, &value);

	zephir_get_strval(&name, name_param);


	zephir_update_property_zval_zval(this_ptr, &name, value);
	RETURN_THIS();

}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getPrivateProperty2) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "privateProperty2");

}

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getProtectedProperty2) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "protectedProperty2");

}

