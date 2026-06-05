
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * Holds an object implementing ArrayAccess in a property and writes to it
 * using array syntax.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2465
 */
ZEPHIR_INIT_CLASS(Stub_ArrayAccessProperty)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessProperty, stub, arrayaccessproperty, stub_arrayaccessproperty_method_entry, 0);

	zend_declare_property_null(stub_arrayaccessproperty_ce, SL("data"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayAccessProperty, __construct)
{
	zval *data, data_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&data_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(data, zend_ce_arrayaccess)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &data);
	zephir_update_property_zval(this_ptr, ZEND_STRL("data"), data);
}

PHP_METHOD(Stub_ArrayAccessProperty, set)
{
	zval *k, k_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &k, &v);
	zephir_update_property_array(this_ptr, SL("data"), k, v);
}

PHP_METHOD(Stub_ArrayAccessProperty, setNested)
{
	zval *a, a_sub, *b, b_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &a, &b, &v);
	zephir_update_property_array_multi(this_ptr, SL("data"), v, SL("zz"), 2, a, b);
}

