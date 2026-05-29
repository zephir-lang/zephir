
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
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * Regression coverage for the cross-class chained `<static>` resolution.
 *
 * A method declared `-> <static>` on `Issue2505` is called via a LOCAL
 * variable (not `this`), and the result is immediately chained. Before
 * the fix, `MethodCall.php` resolved `<static>` to the LEXICAL class of
 * the call site (i.e. `Stub\Issue2505CrossChain`) instead of the
 * receiver's class (`Stub\Issue2505`). Compile-time method lookup for
 * `checkPrivate()` then ran against `Issue2505CrossChain` — which has
 * no such method — and the build aborted with:
 *
 *     Class 'Stub\Issue2505CrossChain' does not implement method: 'checkPrivate'
 *
 * After the fix, `static`/`self`/`parent` resolve against the receiver's
 * class definition (the one already used to look up the called method),
 * so the chained lookup correctly finds `Issue2505::checkPrivate()`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
ZEPHIR_INIT_CLASS(Stub_Issue2505CrossChain)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2505CrossChain, stub, issue2505crosschain, stub_issue2505crosschain_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2505CrossChain, crossClassChain)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *instance, instance_sub, _0;

	ZVAL_UNDEF(&instance_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(instance, stub_issue2505_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &instance);
	ZEPHIR_CALL_METHOD(&_0, instance, "makestatic", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_0, "checkprivate", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2505CrossChain, crossClassChainSelf)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *instance, instance_sub, _0;

	ZVAL_UNDEF(&instance_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(instance, stub_issue2505_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &instance);
	ZEPHIR_CALL_METHOD(&_0, instance, "makeself", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_0, "checkprivate", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

