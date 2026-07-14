
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
	zval name_zv, *value, value_sub, _0, _1;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("setCount", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_zval(this_ptr, &name_zv, value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 76, PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, &_1);
}

PHP_METHOD(Stub_Oo_Scopes_AbstractClassMagic, __get)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	zephir_memory_observe(&_0);
	zephir_read_property_zval(&_0, this_ptr, &name_zv, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

