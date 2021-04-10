
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


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_AbstractClassMagic)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo\\Scopes, AbstractClassMagic, stub, oo_scopes_abstractclassmagic, stub_oo_scopes_abstractclassmagic_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_long(stub_oo_scopes_abstractclassmagic_ce, SL("setCount"), 0, ZEND_ACC_PUBLIC);
	zend_declare_property_string(stub_oo_scopes_abstractclassmagic_ce, SL("privateProperty"), "private", ZEND_ACC_PRIVATE);
	zend_declare_property_string(stub_oo_scopes_abstractclassmagic_ce, SL("protectedProperty"), "protected", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_oo_scopes_abstractclassmagic_ce, SL("publicProperty"), "public", ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClassMagic, __set)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, *value, value_sub, _0, _1;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
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
	zephir_read_property(&_0, this_ptr, ZEND_STRL("setCount"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval(this_ptr, ZEND_STRL("setCount"), &_1);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClassMagic, __get)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, _0;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);
	zephir_get_strval(&name, name_param);


	ZEPHIR_OBS_VAR(&_0);
	zephir_read_property_zval(&_0, this_ptr, &name, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

