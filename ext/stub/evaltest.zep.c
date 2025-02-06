
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_EvalTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, EvalTest, stub, evaltest, stub_evaltest_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_EvalTest, evalCode)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *code_param = NULL;
	zval code;

	ZVAL_UNDEF(&code);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(code)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &code_param);
	zephir_get_strval(&code, code_param);
	zephir_eval_php(&code, return_value, "stub/evaltest.zep:7");
	RETURN_MM();
}

