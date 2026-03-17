
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
	zval code_zv, *code_param = NULL;
	zend_string *code = NULL;

	ZVAL_UNDEF(&code_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(code)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &code_param);
	ZVAL_STR_COPY(&code_zv, code);
	zephir_eval_php(&code_zv, return_value, "stub/evaltest.zep:7");
	return;
}

