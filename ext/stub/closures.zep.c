
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Stub_Closures)
{
	ZEPHIR_REGISTER_CLASS(Stub, Closures, stub, closures, stub_closures_method_entry, 0);

	zend_declare_property_null(stub_closures_ce, SL("_argument"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_closures_ce, SL("_function"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_closures_ce, SL("_name"), "default", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_closures_ce, SL("property1873"), "call from closure", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Closures, simple1)
{

	zephir_create_closure_ex(return_value, NULL, stub_1__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple2)
{

	zephir_create_closure_ex(return_value, NULL, stub_2__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple3)
{

	zephir_create_closure_ex(return_value, NULL, stub_3__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple4)
{

	zephir_create_closure_ex(return_value, NULL, stub_4__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple5)
{

	zephir_create_closure_ex(return_value, NULL, stub_5__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, arrow1)
{

	zephir_create_closure_ex(return_value, NULL, stub_6__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, arrow2)
{

	zephir_create_closure_ex(return_value, NULL, stub_7__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, testUseCommand)
{
	zval _0;
	zend_long abc;

	ZVAL_UNDEF(&_0);
	abc = 1;
	zephir_create_closure_ex(return_value, NULL, stub_8__closure_ce, SL("__invoke"));
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, abc);
	zephir_update_static_property_ce(stub_8__closure_ce, ZEND_STRL("abc"), &_0);
	return;
}

PHP_METHOD(Stub_Closures, issue1860)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *abc_param = NULL;
	zval abc;

	ZVAL_UNDEF(&abc);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(abc, abc_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &abc_param);
	ZEPHIR_OBS_COPY_OR_DUP(&abc, abc_param);
	zephir_create_closure_ex(return_value, NULL, stub_9__closure_ce, SL("__invoke"));
	zephir_update_static_property_ce(stub_9__closure_ce, ZEND_STRL("abc"), &abc);
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/642
 */
PHP_METHOD(Stub_Closures, issue642)
{
	zval _0;
	zval obj, closure;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&closure);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&closure);
	zephir_create_closure_ex(&closure, NULL, stub_10__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_fast_append(&_0, &obj);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &closure, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue1036SetArgument)
{
	zval *argument, argument_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&argument_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_argument", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &argument);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 23, argument);
}

PHP_METHOD(Stub_Closures, issue1036SetFunction)
{
	zval *func, func_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&func_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_function", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(func)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &func);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 24, func);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1036
 */
PHP_METHOD(Stub_Closures, issue1036Call)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_function", 9, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("_argument", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 24, PH_NOISY_CC | PH_READONLY);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_1, 23, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_RETURN_CALL_FUNCTION("call_user_func", NULL, 36, &_0, &_1);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2497
 */
PHP_METHOD(Stub_Closures, issue2497Helper)
{

	RETURN_STRING("hello");
}

PHP_METHOD(Stub_Closures, issue2497ClosureThis)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_11__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, issue2497ClosureThisWithUse)
{
	zval *name, name_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &name);
	zephir_create_closure_ex(return_value, this_ptr, stub_12__closure_ce, SL("__invoke"));
	zephir_update_static_property_ce(stub_12__closure_ce, ZEND_STRL("name"), name);
	return;
}

PHP_METHOD(Stub_Closures, issue2497PropertyAccess)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_13__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, issue2497SetName)
{
	zval name_zv;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_name", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 25, &name_zv);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1873
 *
 * Original reporter's minimal repro: a closure returned from a method
 * that reads a protected property of the enclosing class via `this->`.
 * Note the return type is `<\Closure>`, not `string` — the PR draft
 * (#2203) declared it `-> string` which is itself a type bug under the
 * runtime-return-type enforcement added for #1991.
 */
PHP_METHOD(Stub_Closures, issue1873)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_14__closure_ce, SL("__invoke"));
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1873
 *
 * Variant: closure reads an array property and joins it inside the body.
 * Exercises the property-access + intra-closure expression combination.
 */
PHP_METHOD(Stub_Closures, issue1873ArrayProperty)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_15__closure_ce, SL("__invoke"));
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1873
 *
 * Variant: closure reads multiple properties and concatenates them.
 * Ensures multi-property reads inside a single closure compile cleanly.
 */
PHP_METHOD(Stub_Closures, issue1873MultipleProperties)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_16__closure_ce, SL("__invoke"));
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1873
 *
 * Variant: closure both reads and writes a property — the binding via
 * Closure::bindTo must preserve mutability of the enclosing instance.
 */
PHP_METHOD(Stub_Closures, issue1873PropertyWriter)
{
	zval *this_ptr = getThis();
	zephir_create_closure_ex(return_value, this_ptr, stub_17__closure_ce, SL("__invoke"));
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1873
 *
 * Variant: closure reads a property AND uses a captured local via
 * `use()`. Verifies #1873 (property) and #2497 (use) compose.
 *
 * String-typed captures are covered separately by #2562
 * (see issue2562StringUse).
 */
PHP_METHOD(Stub_Closures, issue1873PropertyAndUse)
{
	zval *prefix, prefix_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&prefix_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(prefix)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &prefix);
	zephir_create_closure_ex(return_value, this_ptr, stub_18__closure_ce, SL("__invoke"));
	zephir_update_static_property_ce(stub_18__closure_ce, ZEND_STRL("prefix"), prefix);
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2562
 *
 * Capturing a scalar `string`-typed variable in a closure via `use()`.
 * String params are stored as `zend_string *` (#2462), so the capture
 * must box them with ZVAL_STR, not ZVAL_STRING (which expects char *).
 */
PHP_METHOD(Stub_Closures, issue2562StringUse)
{
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_create_closure_ex(return_value, this_ptr, stub_19__closure_ce, SL("__invoke"));
	ZVAL_UNDEF(&_0);
	ZVAL_STR(&_0, name);
	zephir_update_static_property_ce(stub_19__closure_ce, ZEND_STRL("name"), &_0);
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2321
 *
 * Original reporter's symptom: `preg_replace_callback(..., [this, 'private'])`
 * from inside the same class produced "cannot access private method".
 *
 * Root cause sits in the PHP engine: when an internal function
 * (`preg_replace_callback`) validates a `callable` argument, it walks
 * back through `prev_execute_data` to the nearest **user-code** frame
 * to determine the visibility scope. Zephir-compiled methods register
 * as `ZEND_INTERNAL_FUNCTION` so they are skipped, leaving the check
 * with no scope and rejecting otherwise-valid private callbacks.
 *
 * Workaround: use a Zephir closure that calls the private method
 * directly. Closures capture `this` (per #2497) and PHP doesn't
 * re-validate visibility when the callable is already a Closure
 * object.
 */
PHP_METHOD(Stub_Closures, issue2321CallPrivateCallback)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval value_zv;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "issue2321filterquery", NULL, 37, &value_zv);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue2321filterQuery)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval value_zv, _0, _1;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_closure_ex(&_0, this_ptr, stub_20__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "/(?:[^%:!\\$&'\\(\\)\\*\\+,;=@\\/\\?]+|%(?![A-Fa-f0-9]{2}))/u");
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace_callback", NULL, 38, &_1, &_0, &value_zv);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue2321doUrlEncode)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *matches_param = NULL, _0;
	zval matches;

	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(matches, matches_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &matches_param);
	zephir_get_arrval(&matches, matches_param);
	zephir_array_fetch_long(&_0, &matches, 0, PH_NOISY | PH_READONLY, "stub/closures.zep", 259);
	ZEPHIR_RETURN_CALL_FUNCTION("rawurlencode", NULL, 39, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2321
 *
 * Variant: protected callback. Same idiom — wrap the method call in
 * a closure that captures `this`, then pass the closure as the
 * callback. PHP doesn't re-validate visibility on Closure callbacks.
 */
PHP_METHOD(Stub_Closures, issue2321ProtectedCallback)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval value_zv, _0, _1;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_closure_ex(&_0, this_ptr, stub_21__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "/[a-z]/");
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace_callback", NULL, 38, &_1, &_0, &value_zv);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue2321ProtectedUpper)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *matches_param = NULL, _0;
	zval matches;

	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(matches, matches_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &matches_param);
	zephir_get_arrval(&matches, matches_param);
	zephir_array_fetch_long(&_0, &matches, 0, PH_NOISY | PH_READONLY, "stub/closures.zep", 282);
	zephir_fast_strtoupper(return_value, &_0);
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2321
 *
 * Variant: array-map over a private method through a Zephir closure.
 * Covers a second common PHP-internal-callable consumer.
 */
PHP_METHOD(Stub_Closures, issue2321ArrayMapPrivate)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *values_param = NULL, _0;
	zval values;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&values);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(values, values_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &values_param);
	zephir_get_arrval(&values, values_param);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_closure_ex(&_0, this_ptr, stub_22__closure_ce, SL("__invoke"));
	ZEPHIR_RETURN_CALL_FUNCTION("array_map", NULL, 15, &_0, &values);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue2321Doubled)
{
	zval *val_param = NULL;
	zend_long val;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val_param);
	RETURN_LONG((val * 2));
}

