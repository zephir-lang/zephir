
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Issue1875)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1875, stub, issue1875, stub_issue1875_method_entry, 0);

	return SUCCESS;
}

/**
 * Case A: var declared but only conditionally assigned, then read.
 * Mirrors the cphalcon Phalcon\Validation\Validator\StringLength snippet
 * from issue #1875.
 */
PHP_METHOD(Stub_Issue1875, conditionalThenRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *options_param = NULL, included;
	zval options;

	ZVAL_UNDEF(&options);
	ZVAL_NULL(&included);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(options, options_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &options_param);
	ZEPHIR_OBS_COPY_OR_DUP(&options, options_param);
	if (zephir_array_isset_value_string(&options, SL("included"))) {
		zephir_memory_observe(&included);
		zephir_array_fetch_string(&included, &options, SL("included"), PH_NOISY, "stub/issue1875.zep", 15);
	} else if (zephir_array_isset_value_string(&options, SL("includedMinimum"))) {
		ZEPHIR_OBS_NVAR(&included);
		zephir_array_fetch_string(&included, &options, SL("includedMinimum"), PH_NOISY, "stub/issue1875.zep", 17);
	}
	RETURN_CCTOR(&included);
}

/**
 * Case B: declared, assigned once unconditionally, then reassigned in
 * both branches of an if/else. Mirrors Min::validate from #1875.
 */
PHP_METHOD(Stub_Issue1875, reassignInBranches)
{
	double _1$$3, _2$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *options_param = NULL, included, _0$$3;
	zval options;

	ZVAL_UNDEF(&options);
	ZVAL_NULL(&included);
	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(options, options_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &options_param);
	ZEPHIR_OBS_COPY_OR_DUP(&options, options_param);
	zephir_memory_observe(&included);
	zephir_array_fetch_string(&included, &options, SL("seed"), PH_NOISY, "stub/issue1875.zep", 31);
	if (Z_TYPE_P(&included) == IS_ARRAY) {
		zephir_memory_observe(&_0$$3);
		zephir_array_fetch_string(&_0$$3, &included, SL("x"), PH_NOISY, "stub/issue1875.zep", 34);
		_1$$3 = zephir_get_boolval(&_0$$3);
		ZEPHIR_INIT_NVAR(&included);
		ZVAL_BOOL(&included, _1$$3);
	} else {
		_2$$4 = zephir_get_boolval(&included);
		ZEPHIR_INIT_NVAR(&included);
		ZVAL_BOOL(&included, _2$$4);
	}
	RETURN_CCTOR(&included);
}

/**
 * Case C: declared but never assigned, returned directly. Should yield
 * null at runtime under PHP semantics.
 */
PHP_METHOD(Stub_Issue1875, neverAssigned)
{
	zval x;

	ZVAL_NULL(&x);
	RETURN_CCTORW(&x);
}

/**
 * Case D: control mirroring the original report exactly, returning the
 * variable inside a constructor of another class.
 */
PHP_METHOD(Stub_Issue1875, forwardedToNewInstance)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *options_param = NULL, included, validator, _0;
	zval options;

	ZVAL_UNDEF(&options);
	ZVAL_NULL(&included);
	ZVAL_NULL(&validator);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(options, options_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &options_param);
	ZEPHIR_OBS_COPY_OR_DUP(&options, options_param);
	if (zephir_array_isset_value_string(&options, SL("included"))) {
		zephir_memory_observe(&included);
		zephir_array_fetch_string(&included, &options, SL("included"), PH_NOISY, "stub/issue1875.zep", 61);
	}
	ZEPHIR_INIT_VAR(&validator);
	zephir_create_array(&validator, 2, 0);
	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, &options, SL("min"), PH_NOISY, "stub/issue1875.zep", 65);
	zephir_array_update_string(&validator, SL("min"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&validator, SL("included"), &included, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&validator);
}

