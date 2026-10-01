
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
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * PHP hands `__call()` and `__callStatic()` the method name exactly as it was
 * written at the call site. Codegen lower-cased the name to look the method up
 * in the class definition and emitted that lowered string as the C literal, so
 * a Zephir caller handed userland a name PHP would never have produced.
 *
 * The receivers are untyped on purpose: a magic method can only be reached
 * through a class the compiler cannot resolve, since a call to a method a known
 * class does not declare is a compile error.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2715
 */
ZEPHIR_INIT_CLASS(Stub_Issue2715)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2715, stub, issue2715, stub_issue2715_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2715, callMixedCase)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(receiver)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &receiver);
	ZEPHIR_RETURN_CALL_METHOD(receiver, "NoPeCaSe", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2715, callDynamicStringName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(receiver)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &receiver);
	ZEPHIR_RETURN_CALL_METHOD(receiver, "NoPeCaSe", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2715, callDynamicVariableName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub, *name, name_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(receiver)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &receiver, &name);
	ZEPHIR_RETURN_CALL_METHOD_ZVAL(receiver, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2715, callStaticMixedCase)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub;

	ZVAL_UNDEF(&className_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &className);
	_0 = zephir_fetch_class(className);
	ZEPHIR_RETURN_CALL_CE_STATIC(_0, "NoPeCaSe", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2715, callStaticDynamicName)
{
	zend_class_entry *_0;
	zval name;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &className);
	ZEPHIR_INIT_VAR(&name);
	ZVAL_STRING(&name, "NoPeCaSe");
	_0 = zephir_fetch_class(className);
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * The shape `Phalcon\Di::__call()` has: the service name is derived from
 * the spelling, so a lowered name looks up a service nobody registered.
 */
PHP_METHOD(Stub_Issue2715, callServiceGetter)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(receiver)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &receiver);
	ZEPHIR_RETURN_CALL_METHOD(receiver, "getMyService", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * Dispatch stays case-insensitive, as PHP's is.
 */
PHP_METHOD(Stub_Issue2715, callDeclaredMethod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(receiver)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &receiver);
	ZEPHIR_RETURN_CALL_METHOD(receiver, "DECLAREDmethod", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2715, staticControl)
{

	RETURN_STRING("self");
}

PHP_METHOD(Stub_Issue2715, callSelfMixedCase)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_SELF("STATICCONTROL", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

