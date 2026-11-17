
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
#include "ext/spl/spl_array.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"


/**
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824Ctor)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2824Ctor, stub, issue2824ctor, spl_ce_ArrayObject, stub_issue2824ctor_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2824Ctor, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a = NULL, a_sub, *b = NULL, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &a, &b);
	if (!a) {
		a = &a_sub;
		ZEPHIR_INIT_VAR(a);
		ZVAL_STRING(a, "x");
	}
	if (!b) {
		b = &b_sub;
		ZEPHIR_INIT_VAR(b);
		ZVAL_STRING(b, "y");
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, a);
	zephir_array_fast_append(&_0, b);
	ZEPHIR_CALL_PARENT_CACHED(NULL, stub_issue2824ctor_ce, getThis(), "__construct", &_zephir_call_cache[0], &_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

