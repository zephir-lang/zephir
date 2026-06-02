
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


ZEPHIR_INIT_CLASS(Stub_Args_Single_ObjNullable)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, ObjNullable, stub, args_single_objnullable, stub_args_single_objnullable_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_ObjNullable, argObjNull)
{
	zval *param = NULL, param_sub, __$null;

	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(param, stub_args_single_str_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param);
	if (!param) {
		param = &param_sub;
		param = &__$null;
	}
}

