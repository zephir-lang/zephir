
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
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Args_Single_StrNullable)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, StrNullable, stub, args_single_strnullable, stub_args_single_strnullable_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_StrNullable, argStringNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval param_zv;
	zend_string *param = NULL;

	ZVAL_UNDEF(&param_zv);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (!param) {
		ZEPHIR_INIT_VAR(&param_zv);
	} else {
		zephir_memory_observe(&param_zv);
	ZVAL_STR_COPY(&param_zv, param);
	}
	if (param) {
		RETURN_MM_STR(zend_string_copy(param));
	}
	RETURN_MM_NULL();
}

