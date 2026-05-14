
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Args_Single_StrMixed)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, StrMixed, stub, args_single_strmixed, stub_args_single_strmixed_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_StrMixed, argStringAndInt)
{
	zend_long position;
	zval str_zv, *position_param = NULL;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();
	position_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&str_zv, str);
	RETURN_STR(zend_string_copy(str));
}

