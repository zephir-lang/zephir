
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2469
 *
 * Reproducer for "Cannot pass literal char into a function expecting char".
 * Crashes Zephir compilation prior to the fix.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2469)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2469, stub, issue2469, stub_issue2469_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2469, peek)
{
	zval *ch_param = NULL;
	char ch;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);
	ch = zephir_get_charval(ch_param);
	RETURN_LONG(ch);
}

PHP_METHOD(Stub_Issue2469, callWithLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "peek", NULL, 58, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2469, callWithVariable)
{
	zval _0;
	char chTmp = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	chTmp = '>';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRINGL(&_0, &chTmp, 1);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "peek", NULL, 58, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2469, callWithLiteralUchar)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "Z");
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "peek", NULL, 58, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

