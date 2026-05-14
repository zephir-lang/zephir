
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
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &argument);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_argument"), argument);
}

PHP_METHOD(Stub_Closures, issue1036SetFunction)
{
	zval *func, func_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&func_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(func)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &func);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_function"), func);
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
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property(&_0, this_ptr, ZEND_STRL("_function"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("_argument"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_RETURN_CALL_FUNCTION("call_user_func", NULL, 29, &_0, &_1);
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
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_name"), &name_zv);
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
 * The captured variable is `var` (not `string`) because Zephir's
 * native-string parameter refactor (#2462) stores string params as
 * `zend_string *` which the `use()` plumbing in closure stubs doesn't
 * yet wrap to a zval — that's an orthogonal limitation worth tracking
 * separately.
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

