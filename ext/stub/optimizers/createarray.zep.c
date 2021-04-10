
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
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_CreateArray)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, CreateArray, stub, optimizers_createarray, stub_optimizers_createarray_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_CreateArray, createNoSize)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();




	zephir_create_array(return_value, 0, 1);
	return;
}

PHP_METHOD(Stub_Optimizers_CreateArray, createSize)
{
	zval *n_param = NULL, _0;
	zend_long n, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	n = zephir_get_intval(n_param);


	ZVAL_LONG(&_0, n);
	zephir_create_array(return_value, zephir_get_intval(&_0), 1);
	return;
}

