
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
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Interfaces_ImplementInt)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Interfaces, ImplementInt, stub, interfaces_implementint, stub_interfaces_implementint_method_entry, 0);

	zend_declare_property_null(stub_interfaces_implementint_ce, SL("val"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_interfaces_implementint_ce, 1, stub_interfaces_interfaceint_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_Interfaces_ImplementInt, set)
{
	zval *val_param = NULL, _0;
	zend_long val;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val_param);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, val);
	zephir_update_property_zval(this_ptr, ZEND_STRL("val"), &_0);
}

PHP_METHOD(Stub_Interfaces_ImplementInt, get)
{

	RETURN_MEMBER(getThis(), "val");
}

