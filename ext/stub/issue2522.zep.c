
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
#include "kernel/memory.h"
#include "Zend/zend_closures.h"


ZEPHIR_INIT_CLASS(Stub_Issue2522)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2522, stub, issue2522, stub_issue2522_method_entry, 0);

	return SUCCESS;
}

/**
 * Issue #2522 ask: inline closure init must compile. Comparing against
 * the two-step form (which has worked for years) for parity.
 */
PHP_METHOD(Stub_Issue2522, inlineClosureIsCallable)
{
	zval call;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&call);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&call);
	zephir_create_closure_ex(&call, NULL, stub_24__closure_ce, SL("__invoke"));
	RETURN_MM_BOOL(zephir_is_callable(&call));
}

PHP_METHOD(Stub_Issue2522, inlineClosureReturnsClosureInstance)
{
	zval call;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&call);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&call);
	zephir_create_closure_ex(&call, NULL, stub_25__closure_ce, SL("__invoke"));
	RETURN_MM_BOOL((zephir_is_instance_of(&call, SL("Closure"))));
}

PHP_METHOD(Stub_Issue2522, inlineFnKeywordIsCallable)
{
	zval call;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&call);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&call);
	zephir_create_closure_ex(&call, NULL, stub_26__closure_ce, SL("__invoke"));
	RETURN_MM_BOOL(zephir_is_callable(&call));
}

PHP_METHOD(Stub_Issue2522, twoStepClosureIsCallable)
{
	zval call;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&call);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&call);
	zephir_create_closure_ex(&call, NULL, stub_27__closure_ce, SL("__invoke"));
	RETURN_MM_BOOL(zephir_is_callable(&call));
}

