
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
 * Keeps the constructor arguments in its storage, so a test can read back
 * which value reached which parameter.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2839
 */
ZEPHIR_INIT_CLASS(Stub_Issue2839Pair)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2839Pair, stub, issue2839pair, spl_ce_ArrayObject, stub_issue2839pair_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2839Pair, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b = NULL, b_sub, __$null;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 1, &a, &b);
	if (!b) {
		b = &b_sub;
		b = &__$null;
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, a);
	zephir_array_fast_append(&_0, b);
	ZEPHIR_CALL_PARENT_CACHED(NULL, stub_issue2839pair_ce, getThis(), "__construct", &_zephir_call_cache[0], &_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

