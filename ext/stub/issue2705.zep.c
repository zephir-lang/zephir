
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * `unset` of an offset reached through anything but a plain `this->prop`.
 *
 * Only `unset this->prop[k]` went through zephir_unset_property_array(), which
 * separates the array and writes it back. A brace property name, a nested
 * offset and a static property all took the generic branch, which fetched the
 * container into a temporary and unset the key there, so the key survived.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2705
 */
ZEPHIR_INIT_CLASS(Stub_Issue2705)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2705, stub, issue2705, stub_issue2705_method_entry, 0);

	/**
	 * Whatever a test wants to unset an offset on.
	 */
	zend_declare_property_null(stub_issue2705_ce, SL("container"), ZEND_ACC_PUBLIC);
	/**
	 * The static counterpart of `container`.
	 */
	zend_declare_property_null(stub_issue2705_ce, SL("staticContainer"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	/**
	 * Typed, so the default is persistent and the first unset really has to
	 * separate from it.
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_long_ex(&_zc0, SL("k"), 1);
		zval _zc1;
		array_init_size(&_zc1, 3);
		add_assoc_long_ex(&_zc1, SL("b"), 1);
		add_assoc_long_ex(&_zc1, SL("c"), 2);
		add_assoc_zval_ex(&_zc0, SL("a"), &_zc1);
		zephir_declare_typed_property(stub_issue2705_ce, SL("defaults"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	zend_declare_property_null(stub_issue2705_ce, SL("staticDefaults"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2705, unsetBraceLiteral)
{
	zval _0, _1, *_2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_memory_observe(&_1);
	_2 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_1);
	{ zval *zephir_unset_offsets[] = { &_0 }; zephir_array_unset_path(_2, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetBraceName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, _0, _1, *_2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_memory_observe(&_1);
	_2 = zephir_fetch_property_write_zval(this_ptr, name, &_1);
	{ zval *zephir_unset_offsets[] = { &_0 }; zephir_array_unset_path(_2, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetBraceNameAndKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, *key, key_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(name)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &name, &key);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write_zval(this_ptr, name, &_0);
	{ zval *zephir_unset_offsets[] = { key }; zephir_array_unset_path(_1, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetBraceNameLongKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, _0, _1, *_2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_memory_observe(&_1);
	_2 = zephir_fetch_property_write_zval(this_ptr, name, &_1);
	{ zval *zephir_unset_offsets[] = { &_0 }; zephir_array_unset_path(_2, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetNested)
{
	zval _0, _1, _2, *_3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(_3, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetNestedByVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *outer, outer_sub, *inner, inner_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&outer_sub);
	ZVAL_UNDEF(&inner_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(outer)
		Z_PARAM_ZVAL(inner)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &outer, &inner);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	{ zval *zephir_unset_offsets[] = { outer, inner }; zephir_array_unset_path(_1, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetDeep)
{
	zval _0, _1, _2, _3, *_4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "c");
	zephir_memory_observe(&_3);
	_4 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_3);
	{ zval *zephir_unset_offsets[] = { &_0, &_1, &_2 }; zephir_array_unset_path(_4, 3, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetBraceLiteralNested)
{
	zval _0, _1, _2, *_3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(_3, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetBraceNameNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name, name_sub, _0, _1, _2, *_3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &name);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write_zval(this_ptr, name, &_2);
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(_3, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

/**
 * The assignment must not run when the unset threw.
 */
PHP_METHOD(Stub_Issue2705, unsetNestedThenAssign)
{
	zval _0, _1, _2, *_3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(_3, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_4);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_STRING(&_4, "reached");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_4);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetPropertyOffsetThenAssign)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "reached");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_1);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetLocalOffsetThenAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *local, local_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&local_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(local)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &local);
	ZEPHIR_SEPARATE_PARAM(local);
	zephir_array_unset_string(local, SL("k"), PH_SEPARATE);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "reached");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetPropertyThenAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *target, target_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&target_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(target)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &target);
	zephir_unset_property(target, "item");
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "reached");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetNamedPropertyThenAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *target, target_sub, *name, name_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&target_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(target)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &target, &name);
	zephir_unset_property_zval(target, name);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "reached");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetLocalNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, _0, _1;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(container, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(container, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2705, unsetLocalDeep)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, _0, _1, _2;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "c");
	{ zval *zephir_unset_offsets[] = { &_0, &_1, &_2 }; zephir_array_unset_path(container, 3, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(container, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2705, unsetStatic)
{
	zval _0, _1, *_2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_memory_observe(&_1);
	_2 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticContainer"), &_1);
	{ zval *zephir_unset_offsets[] = { &_0 }; zephir_array_unset_path(_2, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2705, unsetStaticNested)
{
	zval _0, _1, _2, *_3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticContainer"), &_2);
	{ zval *zephir_unset_offsets[] = { &_0, &_1 }; zephir_array_unset_path(_3, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

/**
 * A copy taken before the unsets must keep both keys, and the copy taken
 * after must not alias the property: both only hold if every unset
 * separated rather than wrote through a shared array or a reference.
 */
PHP_METHOD(Stub_Issue2705, snapshotDefaults)
{
	zval after, _0, _1, _2, *_3, _4, _5, _6, *_7, _8, _9;
	zval ret;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("defaults", 8, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	array_init(&ret);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 75, PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/issue2705.zep", 145);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "k");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_2);
	{ zval *zephir_unset_offsets[] = { &_1 }; zephir_array_unset_path(_3, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "a");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_STRING(&_5, "b");
	zephir_memory_observe(&_6);
	_7 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_6);
	{ zval *zephir_unset_offsets[] = { &_4, &_5 }; zephir_array_unset_path(_7, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_memory_observe(&after);
	zephir_read_property_cached(&after, this_ptr, _zephir_prop_0, 75, PH_NOISY_CC);
	ZEPHIR_INIT_VAR(&_8);
	ZVAL_LONG(&_8, 9);
	zephir_array_update_multi(&after, &_8, SL("ss"), 4, SL("a"), SL("c"));
	zephir_read_property_cached(&_9, this_ptr, _zephir_prop_0, 75, PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_9, PH_SEPARATE, "stub/issue2705.zep", 150);
	zephir_array_append(&ret, &after, PH_SEPARATE, "stub/issue2705.zep", 151);
	RETURN_CTOR(&ret);
}

PHP_METHOD(Stub_Issue2705, snapshotStaticDefaults)
{
	zval _0, _1, _2, *_3, _4, _5, _6, *_7, _8;
	zval ret;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_8);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	array_init(&ret);
	zephir_memory_observe(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2705_ce, SL("staticDefaults"), PH_NOISY_CC);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/issue2705.zep", 160);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "k");
	zephir_memory_observe(&_2);
	_3 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticDefaults"), &_2);
	{ zval *zephir_unset_offsets[] = { &_1 }; zephir_array_unset_path(_3, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "a");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_STRING(&_5, "b");
	zephir_memory_observe(&_6);
	_7 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticDefaults"), &_6);
	{ zval *zephir_unset_offsets[] = { &_4, &_5 }; zephir_array_unset_path(_7, 2, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_memory_observe(&_8);
	zephir_read_static_property_ce(&_8, stub_issue2705_ce, SL("staticDefaults"), PH_NOISY_CC);
	zephir_array_append(&ret, &_8, PH_SEPARATE, "stub/issue2705.zep", 163);
	RETURN_CTOR(&ret);
}

PHP_METHOD(Stub_Issue2705, nestedProbe)
{
	zval _1$$3, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after, _3$$3, _4$$3, _5$$3, *_6$$3, _7$$3, _8$$3, _9$$3, *_10$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_9$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

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
		zephir_create_array(&_1$$3, 1, 0);
		ZEPHIR_INIT_NVAR(&_2$$3);
		zephir_create_array(&_2$$3, 2, 0);
		add_assoc_long_ex(&_2$$3, SL("b"), 1);
		add_assoc_long_ex(&_2$$3, SL("c"), 2);
		zephir_array_update_string(&_1$$3, SL("a"), &_2$$3, PH_COPY | PH_SEPARATE);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_1$$3);
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_STRING(&_3$$3, "a");
		ZEPHIR_INIT_NVAR(&_4$$3);
		ZVAL_STRING(&_4$$3, "b");
		ZEPHIR_OBS_NVAR(&_5$$3);
		_6$$3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_5$$3);
		{ zval *zephir_unset_offsets[] = { &_3$$3, &_4$$3 }; zephir_array_unset_path(_6$$3, 2, zephir_unset_offsets); }
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_7$$3);
		ZVAL_STRING(&_7$$3, "a");
		ZEPHIR_INIT_NVAR(&_8$$3);
		ZVAL_STRING(&_8$$3, "c");
		ZEPHIR_OBS_NVAR(&_9$$3);
		_10$$3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_9$$3);
		{ zval *zephir_unset_offsets[] = { &_7$$3, &_8$$3 }; zephir_array_unset_path(_10$$3, 2, zephir_unset_offsets); }
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2705, staticProbe)
{
	zval _1$$3, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after, _3$$3, _4$$3, *_5$$3, _6$$3, _7$$3, _8$$3, *_9$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
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
		zephir_create_array(&_1$$3, 2, 0);
		add_assoc_long_ex(&_1$$3, SL("k"), 1);
		ZEPHIR_INIT_NVAR(&_2$$3);
		zephir_create_array(&_2$$3, 1, 0);
		add_assoc_long_ex(&_2$$3, SL("b"), 1);
		zephir_array_update_string(&_1$$3, SL("a"), &_2$$3, PH_COPY | PH_SEPARATE);
		zephir_update_static_property_ce(stub_issue2705_ce, ZEND_STRL("staticContainer"), &_1$$3);
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_STRING(&_3$$3, "k");
		ZEPHIR_OBS_NVAR(&_4$$3);
		_5$$3 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticContainer"), &_4$$3);
		{ zval *zephir_unset_offsets[] = { &_3$$3 }; zephir_array_unset_path(_5$$3, 1, zephir_unset_offsets); }
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_6$$3);
		ZVAL_STRING(&_6$$3, "a");
		ZEPHIR_INIT_NVAR(&_7$$3);
		ZVAL_STRING(&_7$$3, "b");
		ZEPHIR_OBS_NVAR(&_8$$3);
		_9$$3 = zephir_fetch_static_property_write_ce(stub_issue2705_ce, SL("staticContainer"), &_8$$3);
		{ zval *zephir_unset_offsets[] = { &_6$$3, &_7$$3 }; zephir_array_unset_path(_9$$3, 2, zephir_unset_offsets); }
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2705, braceNameProbe)
{
	zval _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, *name, name_sub, before, after, _2$$3, _3$$3, *_4$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("container", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(iterations)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &iterations_param, &name);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		zephir_create_array(&_1$$3, 2, 0);
		add_assoc_long_ex(&_1$$3, SL("k"), 1);
		add_assoc_long_ex(&_1$$3, SL("j"), 2);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, &_1$$3);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "k");
		ZEPHIR_OBS_NVAR(&_3$$3);
		_4$$3 = zephir_fetch_property_write_zval(this_ptr, name, &_3$$3);
		{ zval *zephir_unset_offsets[] = { &_2$$3 }; zephir_array_unset_path(_4$$3, 1, zephir_unset_offsets); }
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

void zephir_init_static_properties_Stub_Issue2705()
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	add_assoc_long_ex(&_0, SL("k"), 1);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	add_assoc_long_ex(&_1, SL("b"), 1);
	add_assoc_long_ex(&_1, SL("c"), 2);
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(stub_issue2705_ce, ZEND_STRL("staticDefaults"), &_0);
	ZEPHIR_MM_RESTORE();
}

