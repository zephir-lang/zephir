
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/string.h"


/**
 * Regression coverage for issue #2167: a `[ClassName, "method"]` array
 * passed as a PHP callback (e.g. to array_reduce) fails PHP's callable
 * validation when the method is `protected` or `private`, because Zephir
 * methods are ZEND_INTERNAL_FUNCTION and PHP's scope-walker
 * (`zend_get_executed_scope()`) skips them when looking for the calling
 * class. Same root cause as #2321.
 *
 * The supported workaround is to wrap the callback in a Zephir closure
 * that delegates via a fully-qualified static-call expression. Closures
 * auto-bind to the enclosing class scope, so the delegating call passes
 * PHP's visibility check.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2167
 */
ZEPHIR_INIT_CLASS(Stub_Issue2167)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2167, stub, issue2167, stub_issue2167_method_entry, 0);

	zend_declare_property_null(stub_issue2167_ce, SL("a"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2167, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *data_param = NULL;
	zval data;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&data);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(data, data_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_arrval(&data, data_param);
	zephir_update_property_zval(this_ptr, ZEND_STRL("a"), &data);
	ZEPHIR_MM_RESTORE();
}

/**
 * The broken pattern from the issue: a [class, "method"] array passed
 * directly as a callback. PHP rejects this at the callable-validation
 * step before the callback ever runs. Kept here so the regression
 * test can lock down the documented failure mode.
 */
PHP_METHOD(Stub_Issue2167, reduceWithArrayCallable)
{
	zval _4;
	zval _1;
	zval _0, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property(&_0, this_ptr, ZEND_STRL("a"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "Stub\\Issue2167");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "implodeRow");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "");
	ZEPHIR_CALL_FUNCTION(&_3, "array_reduce", NULL, 63, &_0, &_1, &_2);
	zephir_check_call_status();
	zephir_cast_to_string(&_4, &_3);
	RETURN_CTOR(&_4);
}

/**
 * The supported workaround: a closure that calls the protected
 * static method via its fully-qualified absolute class name. The
 * closure is implicitly bound to the enclosing class, so visibility
 * checks see `Stub\Issue2167` as the caller.
 */
PHP_METHOD(Stub_Issue2167, reduceWithClosureWrapper)
{
	zval _4;
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property(&_0, this_ptr, ZEND_STRL("a"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_closure_ex(&_1, NULL, stub_24__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "");
	ZEPHIR_CALL_FUNCTION(&_3, "array_reduce", NULL, 63, &_0, &_1, &_2);
	zephir_check_call_status();
	zephir_cast_to_string(&_4, &_3);
	RETURN_CTOR(&_4);
}

PHP_METHOD(Stub_Issue2167, implodeRow)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval row;
	zval carry_zv, *row_param = NULL, _0;
	zend_string *carry = NULL;

	ZVAL_UNDEF(&carry_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&row);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(carry)
		ZEPHIR_Z_PARAM_ARRAY(row, row_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	row_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&carry_zv);
	ZVAL_STR_COPY(&carry_zv, carry);
	zephir_get_arrval(&row, row_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_join_str(&_0, SL(" "), &row);
	ZEPHIR_CONCAT_VSVS(return_value, &carry_zv, "[ ", &_0, " ]");
	RETURN_MM();
}

