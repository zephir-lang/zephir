
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_ArrayMerge)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, ArrayMerge, stub, optimizers_arraymerge, stub_optimizers_arraymerge_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_ArrayMerge, mergeTwoRequiredArrays)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *arr1_param = NULL, *arr2_param = NULL;
	zval arr1, arr2;

	ZVAL_UNDEF(&arr1);
	ZVAL_UNDEF(&arr2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(arr1)
		Z_PARAM_ARRAY(arr2)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr1_param, &arr2_param);
	zephir_get_arrval(&arr1, arr1_param);
	zephir_get_arrval(&arr2, arr2_param);
	zephir_fast_array_merge(return_value, &arr1, &arr2);
	RETURN_MM();
}

