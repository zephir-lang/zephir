
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


ZEPHIR_INIT_CLASS(Stub_Args_Single_StrOptional)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Args\\Single, StrOptional, stub, args_single_stroptional, stub_args_single_stroptional_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Args_Single_StrOptional, argStringDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval param_zv;
	zend_string *param = NULL;

	ZVAL_UNDEF(&param_zv);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (!param) {
		param = zend_string_init(ZEND_STRL("test string"), 0);
		ZVAL_STR(&param_zv, param);
	} else {
		ZVAL_STR_COPY(&param_zv, param);
	}
	RETURN_MM_STR(zend_string_copy(param));
}

