
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
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * The write context of a by-reference call argument.
 *
 * PHP fetches such an argument through its storage slot: `ZEND_FETCH_OBJ_W`
 * hands the VM an `IS_INDIRECT` to the property itself, `ZEND_FETCH_DIM_W`
 * separates the container before looking the element up (creating it when it is
 * missing), and `ZEND_SEND_REF` turns that slot into a reference. Zephir read a
 * borrowed copy of the property instead, which is a different thing in four
 * observable ways and a use-after-free in a fifth.
 *
 * Each method below is one of them, and Extension\Issue2691Test compares every
 * one against the same code run as plain PHP.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
ZEPHIR_INIT_CLASS(Stub_Issue2691)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2691, stub, issue2691, stub_issue2691_method_entry, 0);

	zend_declare_property_null(stub_issue2691_ce, SL("prefixes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2691_ce, SL("retained"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2691_ce, SL("rows"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2691_ce, SL("value"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2691_ce, SL("staticPrefixes"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_issue2691_ce, SL("readonlyRows"), &_zc0, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, MAY_BE_ARRAY, NULL, 0);
	}

	stub_issue2691_ce->create_object = zephir_init_properties_Stub_Issue2691;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2691, __construct)
{
	zval _2;
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("readonlyRows", 12, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "seed");
	zephir_array_fast_append(&_1, &_2);
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 67, &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * The container is a property, and an array returned by an earlier call is
 * still held. PHP separates the property's array on the second call, so the
 * array handed out by the first stops tracking it. Borrowing the property
 * left both pointing at one table, so a later push showed through the copy.
 */
PHP_METHOD(Stub_Issue2691, pushAndReturn)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, _0, _2, *_3, _4, _1$$3;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prefixes", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_value(&_0, &key_zv))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_property_array(this_ptr, SL("prefixes"), &key_zv, &_1$$3);
	}
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, _3, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 50);
	ZEPHIR_MAKE_WRITE_REF(&_4);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_4, &value_zv);
	zephir_check_call_status();
	RETURN_MM_MEMBER_TYPED(getThis(), "prefixes", IS_ARRAY);
}

/**
 * The property is named at runtime. `ZEND_FETCH_OBJ_W` does not care that
 * the name is a variable rather than a literal, so neither may this: the
 * write still has to reach the property and not a copy of it.
 */
PHP_METHOD(Stub_Issue2691, pushIntoDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval name_zv, key_zv, value_zv, _0, _2, *_3, _4, _1$$3;
	zend_string *name = NULL, *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prefixes", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(name)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_value(&_0, &key_zv))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_property_array(this_ptr, SL("prefixes"), &key_zv, &_1$$3);
	}
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write_zval(this_ptr, &name_zv, &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, _3, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 66);
	ZEPHIR_MAKE_WRITE_REF(&_4);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_4, &value_zv);
	zephir_check_call_status();
	RETURN_MM_MEMBER_TYPED(getThis(), "prefixes", IS_ARRAY);
}

/**
 * The whole dynamically named property as the by-reference argument.
 */
PHP_METHOD(Stub_Issue2691, shiftDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval name_zv, _0, *_1;
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
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write_zval(this_ptr, &name_zv, &_0);
	ZEPHIR_MAKE_WRITE_REF(_1);
	ZEPHIR_RETURN_CALL_FUNCTION("array_shift", NULL, 70, _1);
	ZEPHIR_UNREF_WRITE(_1);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * The name written as a literal in braces, which the compiler resolves the
 * same way it resolves an ordinary property access.
 */
PHP_METHOD(Stub_Issue2691, pushIntoBracedLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, _0, _2, *_3, _4, _1$$3;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prefixes", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_value(&_0, &key_zv))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_property_array(this_ptr, SL("prefixes"), &key_zv, &_1$$3);
	}
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, _3, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 89);
	ZEPHIR_MAKE_WRITE_REF(&_4);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_4, &value_zv);
	zephir_check_call_status();
	RETURN_MM_MEMBER_TYPED(getThis(), "prefixes", IS_ARRAY);
}

/**
 * There is no slot to hand out for a readonly property either, so the fetch
 * falls back to reading in write mode and the engine raises its own
 * `Cannot indirectly modify readonly property`. Letting it speak keeps that
 * message, and the version it arrived in, out of the kernel.
 */
PHP_METHOD(Stub_Issue2691, pushIntoReadonly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, _0, *_1, _2;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("readonlyRows", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	zephir_memory_observe(&_2);
	zephir_array_fetch(&_2, _1, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 102);
	ZEPHIR_MAKE_WRITE_REF(&_2);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_2, &value_zv);
	zephir_check_call_status();
	RETURN_MM_BOOL(1);
}

PHP_METHOD(Stub_Issue2691, getReadonlyRows)
{

	RETURN_MEMBER_TYPED(getThis(), "readonlyRows", IS_ARRAY);
}

/**
 * A read of an element an earlier write context turned into a reference.
 *
 * PHP's read context dereferences, `ZEND_FETCH_DIM_R`'s ZVAL_COPY_DEREF(),
 * so what comes back is the value and the copy is a copy. Handing back the
 * reference instead would make every later write to that copy show through
 * the property.
 */
PHP_METHOD(Stub_Issue2691, pushThenCopy)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, copy, fetched, _0, _2, *_3, _4, _5, _6, _7, _8, _9, _1$$3;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&copy);
	ZVAL_UNDEF(&fetched);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prefixes", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_value(&_0, &key_zv))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_property_array(this_ptr, SL("prefixes"), &key_zv, &_1$$3);
	}
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, _3, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 128);
	ZEPHIR_MAKE_WRITE_REF(&_4);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_4, &value_zv);
	zephir_check_call_status();
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&copy);
	zephir_array_fetch(&copy, &_5, &key_zv, PH_NOISY, "stub/issue2691.zep", 130);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_STRING(&_6, "copy only");
	zephir_array_append(&copy, &_6, PH_SEPARATE, "stub/issue2691.zep", 131);
	zephir_memory_observe(&fetched);
	zephir_read_property_cached(&_7, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_fetch(&fetched, &_7, &key_zv, 0))) {
		ZEPHIR_INIT_NVAR(&fetched);
		ZVAL_NULL(&fetched);
	}
	ZEPHIR_INIT_NVAR(&_6);
	ZVAL_STRING(&_6, "fetched only");
	zephir_array_append(&fetched, &_6, PH_SEPARATE, "stub/issue2691.zep", 137);
	zephir_create_array(return_value, 3, 0);
	zephir_read_property_cached(&_8, this_ptr, _zephir_prop_0, 68, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_9);
	zephir_array_fetch(&_9, &_8, &key_zv, PH_NOISY, "stub/issue2691.zep", 139);
	zephir_array_update_string(return_value, SL("property"), &_9, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("copy"), &copy, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("fetched"), &fetched, PH_COPY | PH_SEPARATE);
	RETURN_MM();
}

/**
 * The same construct on a static property, `ZEND_FETCH_STATIC_PROP_W`. Its
 * slot lives in the class rather than in an object, and everything else
 * about the write context is the same.
 */
PHP_METHOD(Stub_Issue2691, pushStatic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, _0, _2, *_3, _4, _5, _1$$3;
	zend_string *key = NULL, *value = NULL;

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_static_property_ce(&_0, stub_issue2691_ce, SL("staticPrefixes"), PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset_value(&_0, &key_zv))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_static_property_array_multi_ce(stub_issue2691_ce, SL("staticPrefixes"), &_1$$3, SL("z"), 1, &key_zv);
	}
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_static_property_write_ce(stub_issue2691_ce, SL("staticPrefixes"), &_2);
	zephir_memory_observe(&_4);
	zephir_array_fetch(&_4, _3, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 153);
	ZEPHIR_MAKE_WRITE_REF(&_4);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_4, &value_zv);
	zephir_check_call_status();
	zephir_read_static_property_ce(&_5, stub_issue2691_ce, SL("staticPrefixes"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_5);
}

PHP_METHOD(Stub_Issue2691, resetStatic)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2691_ce, ZEND_STRL("staticPrefixes"), &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * The same construct with a local container that someone else also holds.
 * PHP separates it and the write lands; the borrowed fetch could only turn
 * the element into a reference while the table was unshared, so the caller
 * got an owned copy and the write reached nothing.
 */
PHP_METHOD(Stub_Issue2691, pushIntoLocalShared)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, container, alias, _0, _1;
	zend_string *key = NULL, *value = NULL;

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&container);
	ZVAL_UNDEF(&alias);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_INIT_VAR(&container);
	array_init(&container);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_array_update_zval(&container, &key_zv, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_CPY_WRT(&alias, &container);
	zephir_memory_observe(&_1);
	zephir_array_fetch(&_1, &container, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 177);
	ZEPHIR_MAKE_WRITE_REF(&_1);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_1, &value_zv);
	zephir_check_call_status();
	zephir_create_array(return_value, 2, 0);
	zephir_array_update_string(return_value, SL("container"), &container, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("alias"), &alias, PH_COPY | PH_SEPARATE);
	RETURN_MM();
}

/**
 * The whole property is the by-reference argument, with no subscript. This
 * one never went through the write context at all: the property was read
 * borrowed and ZEPHIR_MAKE_REF() moved the borrow into a reference, so
 * array_shift()'s own separation ran GC_TRY_DELREF() on a reference the
 * zval never took. Wrong values, and a bad free under USE_ZEND_ALLOC=0.
 */
PHP_METHOD(Stub_Issue2691, shiftRows)
{
	zval _0, *_1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("rows", 4, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	ZEPHIR_MAKE_WRITE_REF(_1);
	ZEPHIR_RETURN_CALL_FUNCTION("array_shift", NULL, 70, _1);
	ZEPHIR_UNREF_WRITE(_1);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * A by-reference callee that replaces its argument rather than mutating it
 * in place. settype() destroys the old value first, which on a borrowed
 * property frees the array the property still points at.
 */
PHP_METHOD(Stub_Issue2691, retypeValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval type_zv, _0, *_1;
	zend_string *type = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&type_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(type)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&type_zv);
	ZVAL_STR_COPY(&type_zv, type);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	ZEPHIR_MAKE_WRITE_REF(_1);
	ZEPHIR_RETURN_CALL_FUNCTION("settype", NULL, 71, _1, &type_zv);
	ZEPHIR_UNREF_WRITE(_1);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * The element does not exist yet. A write context is a lookup-or-create in
 * PHP, with no diagnostic; the read context Zephir used raises "Undefined
 * index" and leaves the container alone.
 */
PHP_METHOD(Stub_Issue2691, pushMissingKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval key_zv, value_zv, _0, *_1, _2;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prefixes", 8, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	zephir_memory_observe(&_2);
	zephir_array_fetch(&_2, _1, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 211);
	ZEPHIR_MAKE_WRITE_REF(&_2);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_2, &value_zv);
	zephir_check_call_status();
	RETURN_MM_MEMBER_TYPED(getThis(), "prefixes", IS_ARRAY);
}

/**
 * The slot is not reachable for a magic property, so the fetch falls back
 * to reading an owned copy, exactly as PHP's `zend_fetch_property_address()`
 * does, and says so with the same notice. The holder is untyped so the
 * compiler cannot settle the property at compile time, which is also the
 * only way to write this: a declared property is required otherwise.
 */
PHP_METHOD(Stub_Issue2691, pushIntoOverloaded)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zend_string *key = NULL, *value = NULL;
	zval *holder, holder_sub, key_zv, value_zv, _0, *_1, _2;

	ZVAL_UNDEF(&holder_sub);
	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("missing", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(holder)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	holder = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&key_zv);
	ZVAL_STR_COPY(&key_zv, key);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(holder, _zephir_prop_0, &_0);
	zephir_memory_observe(&_2);
	zephir_array_fetch(&_2, _1, &key_zv, PH_NOISY | PH_WRITE, "stub/issue2691.zep", 225);
	ZEPHIR_MAKE_WRITE_REF(&_2);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 69, &_2, &value_zv);
	zephir_check_call_status();
	RETURN_MM_BOOL(1);
}

PHP_METHOD(Stub_Issue2691, setRows)
{
	zval *rows, rows_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&rows_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("rows", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(rows)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &rows);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 69, rows);
}

PHP_METHOD(Stub_Issue2691, getRows)
{

	RETURN_MEMBER(getThis(), "rows");
}

PHP_METHOD(Stub_Issue2691, setValue)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 70, value);
}

PHP_METHOD(Stub_Issue2691, getValue)
{

	RETURN_MEMBER(getThis(), "value");
}

PHP_METHOD(Stub_Issue2691, getPrefixes)
{

	RETURN_MEMBER_TYPED(getThis(), "prefixes", IS_ARRAY);
}

/**
 * The write context run `iterations` times over a property whose value is
 * replaced every round, so the size of what is reachable does not change
 * and anything the fetch fails to release shows up as growth.
 * leakProbeControl() below is the positive control: a flat reading is what
 * a probe that cannot measure anything reports too.
 */
PHP_METHOD(Stub_Issue2691, writeLeakProbe)
{
	zval _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_3 = NULL;
	zval *iterations_param = NULL, before, after, sink, _2$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&sink);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("rows", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&sink);
	ZVAL_NULL(&sink);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		zephir_create_array(&_1$$3, 2, 0);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "seed");
		zephir_array_fast_append(&_1$$3, &_2$$3);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "next");
		zephir_array_fast_append(&_1$$3, &_2$$3);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 69, &_1$$3);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "shiftrows", &_3, 0);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2691, leakProbeControl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_4 = NULL;
	zval *iterations_param = NULL, before, after, _1$$3, _2$$3, _3$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "x");
		ZVAL_LONG(&_2$$3, 64);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "str_repeat", &_4, 30, &_1$$3, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_3$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Issue2691(zend_class_entry *class_type)
{
		zval _0, _2, _4, _1$$3, _3$$4, _5$$5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_5$$5);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("rows"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("rows"), &_1$$3);
		}
		zephir_read_property_ex(&_2, this_ptr, ZEND_STRL("retained"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_2) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_3$$4);
			array_init(&_3$$4);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("retained"), &_3$$4);
		}
		zephir_read_property_ex(&_4, this_ptr, ZEND_STRL("prefixes"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_4) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_5$$5);
			array_init(&_5$$5);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("prefixes"), &_5$$5);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

void zephir_init_static_properties_Stub_Issue2691()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2691_ce, ZEND_STRL("staticPrefixes"), &_0);
	ZEPHIR_MM_RESTORE();
}

