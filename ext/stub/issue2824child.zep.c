
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824Child)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2824Child, stub, issue2824child, stub_issue2824pair_ce, stub_issue2824child_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2824Child, pair)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	zephir_array_update_string(&_1, SL("b"), b, PH_COPY);
	zephir_array_update_string(&_1, SL("a"), a, PH_COPY);
	ZEPHIR_CALL_CLASS_METHOD_NAMED(&_0, zephir_fcall_parent, stub_issue2824child_ce, getThis(), "pair", Z_ARRVAL_P(&_1));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_check_call_status();
	ZEPHIR_CONCAT_SV(return_value, "child:", &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Child, parentPair)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_parent, stub_issue2824child_ce, getThis(), "spair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

