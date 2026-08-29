
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/concat.h"


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
	zval _0, _1;
	zend_long abc;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	abc = 1;
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_8__closure_ce);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, abc);
	zephir_update_property_zval(&_0, SL("abc"), &_1);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_8__closure_ce, SL("__invoke"));
	RETURN_MM();
}

PHP_METHOD(Stub_Closures, issue1860)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *abc_param = NULL, _0;
	zval abc;

	ZVAL_UNDEF(&abc);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(abc, abc_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &abc_param);
	ZEPHIR_OBS_COPY_OR_DUP(&abc, abc_param);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_9__closure_ce);
	zephir_update_property_zval(&_0, SL("abc"), &abc);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_9__closure_ce, SL("__invoke"));
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
	ZEPHIR_RETURN_CALL_FUNCTION("call_user_func", NULL, 37, &_0, &_1);
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
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_12__closure_ce);
	zephir_update_property_zval(&_0, SL("name"), name);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_12__closure_ce, SL("__invoke"));
	RETURN_MM();
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
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *prefix, prefix_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&prefix_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(prefix)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &prefix);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_18__closure_ce);
	zephir_update_property_zval(&_0, SL("prefix"), prefix);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_18__closure_ce, SL("__invoke"));
	RETURN_MM();
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
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_19__closure_ce);
	zephir_update_property_zval(&_0, SL("name"), &name_zv);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_19__closure_ce, SL("__invoke"));
	RETURN_MM();
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
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "issue2321filterquery", NULL, 38, &value_zv);
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
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace_callback", NULL, 39, &_1, &_0, &value_zv);
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
	ZEPHIR_RETURN_CALL_FUNCTION("rawurlencode", NULL, 40, &_0);
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
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace_callback", NULL, 39, &_1, &_0, &value_zv);
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

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2638
 *
 * Capturing a `string` **local**. Locals never use the native
 * `zend_string *` strategy, so the capture is already a zval and must
 * not be boxed again — boxing it emitted `ZVAL_STRING` on a zval, which
 * does not compile.
 */
PHP_METHOD(Stub_Closures, issue2638StringLocalUse)
{
	zval _0;
	zval name;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&name);
	ZVAL_STRING(&name, "");
	ZEPHIR_INIT_NVAR(&name);
	ZVAL_STRING(&name, "abc");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_23__closure_ce);
	zephir_update_property_zval(&_0, SL("name"), &name);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_23__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2638
 *
 * Variant: a `string` **parameter** that is reassigned in the body. Two
 * mutations disqualify it from the native-string strategy, so it is held
 * as a zval exactly like a local — the same capture path, reachable
 * without changing any compiler option.
 */
PHP_METHOD(Stub_Closures, issue2638StringParamMutatedUse)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, _1;
	zval name, _0;

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name_param);
	zephir_get_strval(&name, name_param);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_VS(&_0, &name, "!");
	ZEPHIR_CPY_WRT(&name, &_0);
	ZEPHIR_INIT_VAR(&_1);
	object_init_ex(&_1, stub_24__closure_ce);
	zephir_update_property_zval(&_1, SL("name"), &name);
	zephir_create_closure_bound(return_value, &_1, NULL, stub_24__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * Captures used to live in `public static` properties on the synthetic
 * per-source-site closure class, so every closure created from one source
 * line shared a single slot and the newest creation overwrote the older
 * ones. Each of the methods below is called twice with different arguments
 * and both closures are kept alive, which is exactly the shape that fails.
 */
PHP_METHOD(Stub_Closures, issue2652Scalar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, _0, _1;
	zend_long n;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_25__closure_ce);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, n);
	zephir_update_property_zval(&_0, SL("n"), &_1);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_25__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * A native `zend_string *` parameter: the capture goes through the
 * companion `<name>_zv` zval rather than the boxing switch.
 */
PHP_METHOD(Stub_Closures, issue2652Str)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, _0;
	zend_string *name = NULL;

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_26__closure_ce);
	zephir_update_property_zval(&_0, SL("name"), &name_zv);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_26__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 */
PHP_METHOD(Stub_Closures, issue2652Arr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *items_param = NULL, _0;
	zval items;

	ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(items, items_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &items_param);
	zephir_get_arrval(&items, items_param);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_27__closure_ce);
	zephir_update_property_zval(&_0, SL("items"), &items);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_27__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * An object capture is by handle in PHP: mutating the object after the
 * closure was created is visible inside it.
 */
PHP_METHOD(Stub_Closures, issue2652Obj)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *box, box_sub, _0;

	ZVAL_UNDEF(&box_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(box)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &box);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_28__closure_ce);
	zephir_update_property_zval(&_0, SL("box"), box);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_28__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * Capture plus `this`: the closure's bound `$this` is the only
 * per-instance slot the engine gives an internal-function closure, so it
 * now holds the capture carrier and the enclosing object rides along on
 * it.
 */
PHP_METHOD(Stub_Closures, issue2652WithThis)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_29__closure_ce);
	zephir_update_property_zval(&_0, SL("name"), name);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_29__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * `return this->prop;` is emitted as `RETURN_MEMBER(getThis(), ...)`,
 * which does not follow the rebound `this_ptr`. Without the fix this
 * reads the property off the capture carrier instead of the enclosing
 * object.
 */
PHP_METHOD(Stub_Closures, issue2652ReturnProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *suffix, suffix_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&suffix_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(suffix)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &suffix);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_30__closure_ce);
	zephir_update_property_zval(&_0, SL("suffix"), suffix);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_30__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * Same story for `return this;`, emitted as `RETURN_THIS()`.
 */
PHP_METHOD(Stub_Closures, issue2652ReturnThis)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *tag, tag_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&tag_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(tag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &tag);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_31__closure_ce);
	zephir_update_property_zval(&_0, SL("tag"), tag);
	zephir_update_property_zval(&_0, SL("__$zephir_this"), this_ptr);
	zephir_create_closure_bound(return_value, &_0, this_ptr, stub_31__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * Closures built in a loop each capture their own iteration value.
 */
PHP_METHOD(Stub_Closures, issue2652Loop)
{
	zval _0;
	zval result, i, _1, *_2, _3$$3, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 0);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_is_iterable(&_0, 0, "stub/closures.zep", 461);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _2)
	{
		ZEPHIR_INIT_NVAR(&i);
		ZVAL_COPY(&i, _2);
		ZEPHIR_INIT_NVAR(&_3$$3);
		object_init_ex(&_3$$3, stub_32__closure_ce);
		zephir_update_property_zval(&_3$$3, SL("i"), &i);
		ZEPHIR_INIT_NVAR(&_4$$3);
		ZEPHIR_INIT_NVAR(&_4$$3);
		zephir_create_closure_bound(&_4$$3, &_3$$3, NULL, stub_32__closure_ce, SL("__invoke"));
		zephir_array_append(&result, &_4$$3, PH_SEPARATE, "stub/closures.zep", 458);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&i);
	RETURN_CCTOR(&result);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * A by-value capture is a snapshot taken when the closure is created;
 * later writes to the enclosing variable are not visible inside it.
 */
PHP_METHOD(Stub_Closures, issue2652Snapshot)
{
	zval callback, _0, _1;
	zend_long value = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&callback);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	value = 5;
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_33__closure_ce);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, value);
	zephir_update_property_zval(&_0, SL("value"), &_1);
	ZEPHIR_INIT_VAR(&callback);
	zephir_create_closure_bound(&callback, &_0, NULL, stub_33__closure_ce, SL("__invoke"));
	value = 99;
	RETURN_CCTOR(&callback);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * Writing to a by-value capture inside the body is local to the call: PHP
 * re-seeds the bound variable on every invocation, so two calls of the
 * same closure both return the same value.
 */
PHP_METHOD(Stub_Closures, issue2652BodyMutation)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, _0, _1;
	zend_long n;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_34__closure_ce);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, n);
	zephir_update_property_zval(&_0, SL("n"), &_1);
	zephir_create_closure_bound(return_value, &_0, NULL, stub_34__closure_ce, SL("__invoke"));
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * `use (&x)` shares one storage slot: writes inside the closure are
 * visible to every other holder of the same capture, and persist across
 * calls.
 */
PHP_METHOD(Stub_Closures, issue2652ByRefShared)
{
	zval counter, bump, read, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&counter);
	ZVAL_UNDEF(&bump);
	ZVAL_UNDEF(&read);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&counter);
	zephir_make_local_reference(&counter);

	ZVAL_LONG(Z_REFVAL_P(&counter), 0);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_35__closure_ce);
	zephir_update_property_reference(&_0, SL("counter"), &counter);
	ZEPHIR_INIT_VAR(&bump);
	zephir_create_closure_bound(&bump, &_0, NULL, stub_35__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_1);
	object_init_ex(&_1, stub_36__closure_ce);
	zephir_update_property_reference(&_1, SL("counter"), &counter);
	ZEPHIR_INIT_VAR(&read);
	zephir_create_closure_bound(&read, &_1, NULL, stub_36__closure_ce, SL("__invoke"));
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &bump);
	zephir_array_fast_append(return_value, &read);
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * A by-reference write inside the closure is visible to the enclosing
 * scope.
 */
PHP_METHOD(Stub_Closures, issue2652ByRefWritesOut)
{
	zval value, bump, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&bump);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value);
	zephir_make_local_reference(&value);

	ZVAL_LONG(Z_REFVAL_P(&value), 0);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_37__closure_ce);
	zephir_update_property_reference(&_0, SL("value"), &value);
	ZEPHIR_INIT_VAR(&bump);
	zephir_create_closure_bound(&bump, &_0, NULL, stub_37__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CALL_USER_FUNC(&_1, &bump);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_2);
	ZEPHIR_CALL_USER_FUNC(&_2, &bump);
	zephir_check_call_status();
	RETURN_CCTOR(Z_REFVAL_P(&value));
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2652
 *
 * And a write in the enclosing scope after the closure was created is
 * visible inside it, which is what separates `use (&x)` from `use (x)`.
 */
PHP_METHOD(Stub_Closures, issue2652ByRefReadsLateWrite)
{
	zval value, reader, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&reader);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value);
	zephir_make_local_reference(&value);

	ZVAL_LONG(Z_REFVAL_P(&value), 1);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_38__closure_ce);
	zephir_update_property_reference(&_0, SL("value"), &value);
	ZEPHIR_INIT_VAR(&reader);
	zephir_create_closure_bound(&reader, &_0, NULL, stub_38__closure_ce, SL("__invoke"));
	ZVAL_LONG(Z_REFVAL_P(&value), 42);
	RETURN_CCTOR(&reader);
}

