
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
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	zephir_update_property_zval(this_ptr, ZEND_STRL("a"), a);
	zephir_update_property_zval(this_ptr, ZEND_STRL("b"), b);
}

PHP_METHOD(Stub_Oo_OoConstructParams, getA)
{

	RETURN_MEMBER(getThis(), "a");
}

PHP_METHOD(Stub_Oo_OoConstructParams, getB)
{

	RETURN_MEMBER(getThis(), "b");
}

