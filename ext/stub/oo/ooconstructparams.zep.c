
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


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoConstructParams)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, OoConstructParams, stub, oo_ooconstructparams, stub_oo_ooconstructparams_method_entry, 0);

	zend_declare_property_null(stub_oo_ooconstructparams_ce, SL("a"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_oo_ooconstructparams_ce, SL("b"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_OoConstructParams, __construct)
{
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("a", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("b", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 68, a);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 69, b);
}

PHP_METHOD(Stub_Oo_OoConstructParams, getA)
{

	RETURN_MEMBER(getThis(), "a");
}

PHP_METHOD(Stub_Oo_OoConstructParams, getB)
{

	RETURN_MEMBER(getThis(), "b");
}

