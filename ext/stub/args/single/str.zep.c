
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Args_Single_Str)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, Str, stub, args_single_str, stub_args_single_str_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_Str, argString)
{
	zval str_zv, *str_param = NULL;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &str_param);
	ZVAL_STR_COPY(&str_zv, str);
	RETURN_STR(zend_string_copy(str));
}

