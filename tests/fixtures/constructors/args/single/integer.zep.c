
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


ZEPHIR_INIT_CLASS(Stub_Args_Single_Integer)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, Integer, stub, args_single_integer, stub_args_single_integer_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_Integer, argInt)
{
	zval *i_param = NULL;
	zend_long i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &i_param);
	RETURN_LONG(i);
}

