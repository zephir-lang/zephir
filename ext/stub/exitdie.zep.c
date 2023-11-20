
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
#include "kernel/operators.h"
#include "kernel/exit.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ExitDie)
{
	ZEPHIR_REGISTER_CLASS(Stub, ExitDie, stub, exitdie, stub_exitdie_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ExitDie, testExit)
{
	zval *param = NULL, param_sub, __$null;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(0, 1, &param);
	if (!param) {
		param = &param_sub;
		param = &__$null;
	}


	if (ZEPHIR_IS_EMPTY(param)) {
		zephir_exit_empty();
	}
	zephir_exit(param);
}

PHP_METHOD(Stub_ExitDie, testDie)
{
	zval *param = NULL, param_sub, __$null;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(0, 1, &param);
	if (!param) {
		param = &param_sub;
		param = &__$null;
	}


	if (ZEPHIR_IS_EMPTY(param)) {
		zephir_exit_empty();
	}
	zephir_exit(param);
}

