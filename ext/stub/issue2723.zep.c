
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * Static calls whose method name, or class, is only known at run time.
 *
 * The method name of `{className}::{name}()` was emitted as the bare C name and
 * read by value, so a parameter did not compile. A dynamic name on a known
 * class, and every `{"name"}` form, compiled to nothing. The class part
 * dispatched an object or a non-string on stdClass and turned a missing class
 * into an uncatchable fatal error.
 *
 * Methods only: a property would shift every later property cache slot.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
ZEPHIR_INIT_CLASS(Stub_Issue2723)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2723, stub, issue2723, stub_issue2723base_ce, stub_issue2723_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2723, target)
{

	RETURN_STRING("target");
}

PHP_METHOD(Stub_Issue2723, calledClass)
{

	zephir_get_called_class(return_value);
	return;
}

PHP_METHOD(Stub_Issue2723, withArgument)
{
	zval *argument, argument_sub;

	ZVAL_UNDEF(&argument_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &argument);
	ZEPHIR_CONCAT_SV(return_value, "argument ", argument);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2723, instanceTarget)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_get_class(&_0, this_ptr, 0);
	ZEPHIR_CONCAT_SV(return_value, "instance of ", &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

/**
 * The reported shape: the method name is a parameter.
 */
PHP_METHOD(Stub_Issue2723, viaParam)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name, name_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, viaLocal)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name, name_sub, local;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&local);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &name);
	ZEPHIR_CPY_WRT(&local, name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, &local, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, viaStringParam)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zend_string *name = NULL;
	zval *className, className_sub, name_zv;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	className = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, &name_zv, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, viaMixedParam)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name;
	zval *className, className_sub, name_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, viaMixedClass)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval className_sub;
	zval *className;

	ZVAL_UNDEF(&className_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &className);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_CACHED(_0, "target", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, viaParamWithArgument)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name, name_sub, *argument, argument_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&argument_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &className, &name, &argument);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0, argument);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, assignViaParam)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name, name_sub, result;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_CE_STATIC_ZVAL(&result, _0, name, NULL, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&result);
}

PHP_METHOD(Stub_Issue2723, statementDynamicName)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name, name_sub, *counter, counter_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&counter_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(counter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &className, &name, &counter);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_CE_STATIC_ZVAL(NULL, _0, name, NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD_CACHED(counter, "bump", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, statementLiteralName)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *counter, counter_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&counter_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(counter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &counter);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_CE_STATIC_CACHED(NULL, _0, "target", &_zephir_call_cache[0]);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD_CACHED(counter, "bump", &_zephir_call_cache[1]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, missingClassSkipsArguments)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *counter, counter_sub, _1;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&counter_sub);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(counter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &counter);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_METHOD_CACHED(&_1, counter, "bump", &_zephir_call_cache[0]);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_CE_STATIC_CACHED(_0, "withArgument", &_zephir_call_cache[1], &_1);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * The result variable is the variable holding the name.
 */
PHP_METHOD(Stub_Issue2723, assignToName)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, *name = NULL, name_sub;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(className)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &className, &name);
	ZEPHIR_SEPARATE_PARAM(name);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_CE_STATIC_ZVAL(name, _0, name, NULL, 0);
	zephir_check_call_status();
	RETVAL_ZVAL(name, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, assignToMethodName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *receiver, receiver_sub, *name = NULL, name_sub;

	ZVAL_UNDEF(&receiver_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(receiver)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &receiver, &name);
	ZEPHIR_SEPARATE_PARAM(name);
	ZEPHIR_CALL_METHOD_ZVAL(name, receiver, name, NULL, 0);
	zephir_check_call_status();
	RETVAL_ZVAL(name, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, methodViaParam)
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

PHP_METHOD(Stub_Issue2723, methodViaMixedParam)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name;
	zval *receiver, receiver_sub, name_sub;

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

PHP_METHOD(Stub_Issue2723, selfDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub;

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_RETURN_CALL_SELF_ZVAL(stub_issue2723_ce, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, selfDynamicStatic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub;

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_RETURN_CALL_SELF_ZVAL(stub_issue2723_ce, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, selfDynamicStatement)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub, *counter, counter_sub;

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&counter_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(counter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &name, &counter);
	ZEPHIR_CALL_SELF_ZVAL(NULL, stub_issue2723_ce, name, NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD_CACHED(counter, "bump", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, parentDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub;

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_RETURN_CALL_PARENT_ZVAL(stub_issue2723_ce, getThis(), name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, parentDynamicWithArgument)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub, *argument, argument_sub, result;

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&argument_sub);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &name, &argument);
	ZEPHIR_CALL_PARENT_ZVAL(&result, stub_issue2723_ce, getThis(), name, NULL, 0, argument);
	zephir_check_call_status();
	RETURN_CCTOR(&result);
}

PHP_METHOD(Stub_Issue2723, namedDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub;

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(stub_issue2723base_ce, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, namedDynamicStatic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub;

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(stub_issue2723base_ce, name, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, internalDynamic)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name, name_sub, *argument, argument_sub, _1;

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&argument_sub);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &name, &argument);
	_0 = zephir_fetch_class_str_ex(SL("DateTime"), ZEND_FETCH_CLASS_AUTO);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "!Y-m-d");
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0, &_1, argument);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, selfString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_SELF_CACHED("target", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, staticString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_STATIC_CACHED("calledClass", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, namedString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_CE_STATIC_CACHED(stub_issue2723base_ce, "baseTarget", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, constantString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_CE_STATIC_CACHED(stub_issue2723base_ce, "baseTarget", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, dynamicClassString)
{
	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub;

	ZVAL_UNDEF(&className_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &className);
	_0 = zephir_fetch_class(className);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_RETURN_CALL_CE_STATIC_CACHED(_0, "target", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2723, parentResultIntoParameter)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *result = NULL, result_sub;

	ZVAL_UNDEF(&result_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(result)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &result);
	ZEPHIR_SEPARATE_PARAM(result);
	ZEPHIR_CALL_PARENT_CACHED(result, stub_issue2723_ce, getThis(), "baseTarget", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETVAL_ZVAL(result, 1, 0);
	RETURN_MM();
}

